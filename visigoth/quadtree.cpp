#include "quadtree.h"

#include <QPointF>
#include <QRectF>

#include <cmath>
#include <stdexcept>

QuadTree::QuadTree(QRectF boundaries)
{
    edge = calculateEdge(boundaries);

    _root = new QuadTree::Quadrant(0, 0, 0, edge);
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

    // The number of quadrants will be lb(e / BASE_QUADRANT_SIZE)
    int baseQuadrants = (int) ceil(log(e / (qreal) BASE_QUADRANT_SIZE) / log(2));

    int edge = baseQuadrants * BASE_QUADRANT_SIZE;

    return edge;
}

QuadTree::TreeNode& QuadTree::root() const {
    return *_root;
}

void QuadTree::addNode(QuadTree::TreeNode& node) {
    _root->addChild(node);
}

//------ QuadTree::Quadrant

QuadTree::Quadrant::Quadrant(int level, int x, int y, int edge) :
    level(level),
    x(x),
    y(y),
    edge(edge),
    _size(0)
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

QPointF QuadTree::Quadrant::quadrantCenter() const {
    int quadrants = 1 << level;
    int quadrantSize = edge / quadrants;
    int halfQuadrantSize = quadrantSize / 2;

    int xsign;
    int ysign;
    if (this->x == 0) {
        xsign = -1;
    } else {
        xsign = this->x / this->x;
    }
    if (this->y == 0) {
        ysign = -1;
    } else {
        ysign = this->y / this->y;
    }

    qreal x = (qreal) (quadrantSize * this->x + (xsign * halfQuadrantSize));
    qreal y = (qreal) (quadrantSize * this->y + (ysign * halfQuadrantSize));

    return QPointF(x, y);
}

QPointF QuadTree::Quadrant::center() const {
    if (!hasChildren()) {
        return quadrantCenter();
    } else {
        return _center;
    }
}

bool QuadTree::Quadrant::hasChildren() const {
    return _children.size() > 0;
}

const QVector<QuadTree::TreeNode*>& QuadTree::Quadrant::children() const {
    return _children;
}

qreal QuadTree::Quadrant::width() const {
    int quadrants = 1 << level;
    int quadrantSize = edge / quadrants;

    return (qreal) quadrantSize;
}

int QuadTree::Quadrant::childIndex(QuadTree::TreeNode& node) const {
    QPointF qcenter = quadrantCenter();
    QPointF ncenter = node.center();

    int row;
    int col;

    if (ncenter.x() < qcenter.x()) {
        row = 0;
    } else {
        row = 1;
    }

    if (ncenter.y() < qcenter.y()) {
        col = 0;
    } else {
        col = 1;
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
    return width() > BASE_QUADRANT_SIZE;
}

void QuadTree::Quadrant::addChild(QuadTree::TreeNode& node) {
    if (size() < 1) {
        _center = node.center();
    } else {
        // Weigh the center
        QPointF nodeCenter = node.center();
        int nodeSize = node.size();
        _center = QPointF((size() * center().x() + nodeSize * nodeCenter.x()) / (size() + nodeSize),
                          (size() * center().y() + nodeSize * nodeCenter.y()) / (size() + nodeSize));

        // If it's not a terminal node, recurse down
        if (!isTerminal()) {
            if (size() == 1) {
                // Create the nodes, add itself to the appropriate node
                _children.resize(4);

                for (int i = 0; i < 4; ++i) {
                    _children[i] = new QuadTree::Quadrant(level + 1, i / 2, i % 2, edge);
                }

                castAndAddChild(_children[childIndex(*this)], *this);
            }


            // Add the node to the appropriate child
            castAndAddChild(_children[childIndex(*this)], node);
        } else {
            // Just add the node to the list of children
            _children.append(&node);
        }
    }

    ++_size;
}
