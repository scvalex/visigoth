#include "quadtree.h"

#include <cmath>
#include <stdexcept>
#include <iostream>

#include <QPointF>
#include <QRectF>

QuadTree::QuadTree(QRectF boundaries)
{
    edge = calculateEdge(boundaries);

    _root = new QuadTree::Quadrant(0, QPointF(0, 0), edge);
}

QuadTree::~QuadTree() {
    delete _root;
}

int QuadTree::calculateEdge(QRectF boundaries) const {
    qreal left = boundaries.left();
    qreal right = boundaries.right();
    qreal top = boundaries.top();
    qreal bottom = boundaries.bottom();

    qreal w = qMax(qAbs(left), qAbs(right)) * 2;
    qreal h = qMax(qAbs(top), qAbs(bottom)) * 2;
    qreal e = qMax(w, h);

    qreal baseQuadrants = e / (qreal) BASE_QUADRANT_SIZE;
    int edge = (1 << (int) ceil(log(baseQuadrants) / log(2.0))) * BASE_QUADRANT_SIZE;

    return edge;
}

QuadTree::TreeNode& QuadTree::root() const {
    return *_root;
}

void QuadTree::addNode(QuadTree::TreeNode& node) {
    _root->addChild(node);
}

void QuadTree::printTree(QuadTree::TreeNode* node) {
        QuadTree::Quadrant* q = dynamic_cast<QuadTree::Quadrant*>(node);

        if (q != NULL) {
        foreach (QuadTree::TreeNode* child, node->children()) {
                for (int i = 0; i < q->getLevel(); i++) {
                    std::cout << "\t";
                }

                std::cout << "Level: " << (q->getLevel() + 1) << ", width: " << child->width()
                          << ", size: " << child->size() << ", center: " << child->center().x()
                          << "," << child->center().y() << "\n";
                printTree(child);
            }
        }
}

//------ QuadTree::TreeNode

bool QuadTree::TreeNode::isFarEnough(qreal distance) {
    return (width() / distance) <= tolerance;
}

//------ QuadTree::Quadrant

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

int QuadTree::Quadrant::childIndex(QuadTree::TreeNode& node) const {
    QPointF ncenter = node.center();

    int row;
    int col;

    if (ncenter.x() > quadrantCenter.x()) {
        col = 0;
    } else {
        col = 1;
    }

    if (ncenter.y() > quadrantCenter.y()) {
        row = 0;
    } else {
        row = 1;
    }

    return row * 2 + col;
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

void QuadTree::Quadrant::addChild(QuadTree::TreeNode& node) {
    // Weigh the center
    QPointF nodeCenter = node.center();
    int nodeSize = node.size();
    qreal sizeSum = (qreal) (size() + nodeSize);
    _center = QPointF((size() * center().x() + nodeSize * nodeCenter.x()) / sizeSum,
                      (size() * center().y() + nodeSize * nodeCenter.y()) / sizeSum);

    // If it's not a terminal node, recurse down
    if (!isTerminal()) {
        // Create the nodes
        if (_children.size() < 4) {
            _children.resize(4);

            int diff = _width / 2;

            for (int i = 0; i < 4; ++i) {
                int row = i / 2;
                int col = i % 2;
                int xsign;
                int ysign;
                if (row == 0) {
                    ysign = 1;
                } else {
                    ysign = -1;
                }
                if (col == 0) {
                    xsign = 1;
                } else {
                    xsign = -1;
                }

                QPointF childCenter(quadrantCenter.x() + (diff * xsign),
                                    quadrantCenter.y() + (diff * ysign));
                _children[i] = new QuadTree::Quadrant(level + 1, childCenter, diff);
            }
        }


        // Add the node to the appropriate child
        castAndAddChild(_children[childIndex(*this)], node);
    } else {
        // Just add the node to the list of children
        _children.append(&node);
    }

    ++_size;
}

int QuadTree::Quadrant::getLevel() {
    return level;
}
