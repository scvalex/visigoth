#include "quadtree.h"

#include <cmath>
#include <stdexcept>
#include <iostream>

#include <QPointF>
#include <QRectF>

inline int pow2(int exp) {
    return 1 << exp;
}

// ---------------------------------------------------------------------------
// QuadTree

QuadTree::QuadTree(QRectF boundaries)
{
    int width = calculateWidth(boundaries);

    // First node, level 0, coords 0,0, width equals to the width of the entire space
    _root = new QuadTree::Quadrant(0, QPointF(0, 0), width);
}

QuadTree::~QuadTree() {
    delete _root;
}

int QuadTree::calculateWidth(QRectF boundaries) const {
    // We want to get a square starting from 0,0 that is
    // 1) Big enough to contain the whole graph
    // 2) It must contain a number of terminal quadrants which is a power of 4,
    //    since we're recursively subdividing the space splitting the square in
    //    4 squares

    qreal left = boundaries.left();
    qreal right = boundaries.right();
    qreal top = boundaries.top();
    qreal bottom = boundaries.bottom();

    // Calculate how big the square has to be to contain the whole graph
    qreal w = qMax(qAbs(left), qAbs(right)) * 2;
    qreal h = qMax(qAbs(top), qAbs(bottom)) * 2;
    qreal e = qMax(w, h);

    // Make the width so that it will contain a number of squares of base size which is a
    // power of 2 (so that the square will contain a number of terminal quadrants which is a
    // power of 4).
    qreal baseQuadrants = e / (qreal) BASE_QUADRANT_SIZE;
    int width = pow2((int) ceil(log(baseQuadrants) / log(2.0))) * BASE_QUADRANT_SIZE;

    return width;
}

QuadTree::TreeNode& QuadTree::root() const {
    return *_root;
}

void QuadTree::addNode(QuadTree::TreeNode& node) {
    _root->addChild(node);
}


// ---------------------------------------------------------------------------
// QuadTree::TreeNode

bool QuadTree::TreeNode::isFarEnough(qreal distance) {
    return (width() / distance) <= tolerance;
}

// ---------------------------------------------------------------------------
// QuadTree::Quadrant

QuadTree::Quadrant::Quadrant(int level, QPointF center, int width) :
    level(level),
    quadrantCenter(center),
    _width(width),
    _size(0),
    _center(center)
{
}

QuadTree::Quadrant::~Quadrant() {
    if (!isTerminal()) {
        foreach (TreeNode* node, _children) {
            delete node;
        }
    }
}

int QuadTree::Quadrant::size() const {
    return _size;
}

QPointF QuadTree::Quadrant::center() const {
    return _center;
}

bool QuadTree::Quadrant::hasChildren() const {
    return _children.size() > 0;
}

const QVector<QuadTree::TreeNode*>& QuadTree::Quadrant::children() const {
    return _children;
}

qreal QuadTree::Quadrant::width() const {
    return (qreal) _width;
}

void QuadTree::Quadrant::castAndAddChild(QuadTree::TreeNode* node, QuadTree::TreeNode& child) const {
    QuadTree::Quadrant* q = dynamic_cast<QuadTree::Quadrant*>(node);

    if (q == NULL) {
        throw std::runtime_error("QuadTree::Quadrant::castAndAddChild: trying to cast a non-Quadrant.");
    }

    q->addChild(child);
}

bool QuadTree::Quadrant::isTerminal() {
    return width() <= BASE_QUADRANT_SIZE;
}

inline QPointF QuadTree::Quadrant::weightedMiddle(QuadTree::TreeNode& node1, QuadTree::TreeNode& node2) const {
    // If node1 and node2 are two vectors w and v with respective weights a and b, we want
    // (a*w + b*v) / (a + b)

    QPointF node1Center = node1.center();
    int node1Size = node1.size();

    QPointF node2Center = node2.center();
    int node2Size = node2.size();

    qreal sizeSum = (qreal) (node1Size + node2Size);
    return QPointF((node1Size * node1Center.x() + node2Size * node2Center.x()) / sizeSum,
                   (node1Size * node1Center.y() + node2Size * node2Center.y()) / sizeSum);

}

void QuadTree::Quadrant::addChild(QuadTree::TreeNode& node) {
    // Weigh the center with the new node
    _center = weightedMiddle(*this, node);

    // If it's not a terminal node, recurse down
    if (!isTerminal()) {
        // If the node has no children, create them
        if (_children.size() < 4) {
            // We'll have 4 children, since we're splitting the current quadrant in 4 squares
            _children.resize(4);

            // This will be the width of the children nodes
            int childWidth = _width / 2;

            for (int i = 0; i < 4; ++i) {

                // The various children will have quadrant centers translated by childWidth / 2 from the
                // parent node's quadrant center, so we'll need to add/subtract childWidth / 2 to the current
                // quadrant center, depending on the position.
                int xsign;
                int ysign;
                switch (i) {
                // Top left
                case 0:
                    xsign = -1;
                    ysign = 1;
                    break;
                // Top right
                case 1:
                    xsign = 1;
                    ysign = 1;
                    break;
                // Bottom left
                case 2:
                    xsign = -1;
                    ysign = -1;
                    break;
                // Bottom right
                case 3:
                    xsign = 1;
                    ysign = -1;
                    break;
                default:
                    break;
                }

                QPointF childCenter(quadrantCenter.x() + ((childWidth / 2) * xsign),
                                    quadrantCenter.y() + ((childWidth / 2) * ysign));
                _children[i] = new QuadTree::Quadrant(level + 1, childCenter, childWidth);
            }


        }

        // Add the node recursively, inspecting which child it belongs to.
        if (node.center().x() < quadrantCenter.x() && node.center().y() >= quadrantCenter.y()) {
            // Top left
            castAndAddChild(_children[0], node);
        } else if (node.center().x() >= quadrantCenter.x() && node.center().y() >= quadrantCenter.y()) {
            // Top right
            castAndAddChild(_children[1], node);
        } else if (node.center().x() < quadrantCenter.x() && node.center().y() < quadrantCenter.y()) {
            // Bottom left
            castAndAddChild(_children[2], node);
        } else {
            // Bottom right
            castAndAddChild(_children[3], node);
        }
    } else {
        // If it's a terminal node, just add the node to the list of children
        _children.append(&node);
    }

    ++_size;
}

// ---------------------------------------------------------------------------
// Debug functions

// Prints the tree in a nice way.
void QuadTree::printTree(QuadTree::TreeNode* node) const {
        QuadTree::Quadrant* q = dynamic_cast<QuadTree::Quadrant*>(node);

        if (q != NULL) {
        foreach (QuadTree::TreeNode* child, node->children()) {
                for (int i = 0; i < q->getLevel(); i++) {
                    std::cout << "\t";
                }

                std::cout << "Level: " << (q->getLevel() + 1) << ", width: " << child->width()
                          << ", size: " << child->size() << ", center: " << child->center().x()
                          << "," << child->center().y();

                QuadTree::Quadrant* qchild = dynamic_cast<QuadTree::Quadrant*>(child);
                if (qchild) {
                    std::cout << ", quadcenter: " << qchild->getQuadrantCenter().x() << "," << qchild->getQuadrantCenter().y();
                }

                std::cout << "\n";

                printTree(child);
            }
        }
}

int QuadTree::Quadrant::getLevel() const {
    return level;
}

QPointF QuadTree::Quadrant::getQuadrantCenter() const {
    return quadrantCenter;
}
