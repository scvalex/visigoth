#include "quadtree.h"

#include <QPointF>
#include <QRectF>

#include <cmath>


QuadTree::QuadTree(QRectF boundaries)
{
    edge = calculateEdge(boundaries);
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

//------ QuadTree::Quadrant

QuadTree::Quadrant::Quadrant(int level, int x, int y, int edge) :
    level(level),
    x(x),
    y(y),
    edge(edge),
    _weight(0)
{
}

int QuadTree::Quadrant::weight() const {
    return _weight;
}

QPointF QuadTree::Quadrant::center() const {
    if (!hasChildren()) {
        int quadrants = 1 << level;
        int quadrantSize = edge / quadrants;
        int halfQuadrantSize = quadrantSize / TREE_WAY;

        qreal x = (qreal) (quadrantSize * this->x + ((this->x / this->x) * halfQuadrantSize));
        qreal y = (qreal) (quadrantSize * this->y + ((this->y / this->y) * halfQuadrantSize));

        return QPointF(x, y);
    } else {
        return _center;
    }
}

bool QuadTree::Quadrant::hasChildren() const {
    return _children.size() > 0;
}

QVector<QuadTree::TreeNode*> QuadTree::Quadrant::children() const {
    return _children;
}

qreal QuadTree::Quadrant::width() const {
    int quadrants = 1 << level;
    int quadrantSize = edge / quadrants;

    return (qreal) quadrantSize;
}

QPointF QuadTree::Quadrant::addChild(QuadTree::TreeNode& node) {
    return QPointF(0, 0);
}
