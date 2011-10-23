#include "node.h"
#include "edge.h"
#include <QPainter>

Edge::Edge(Node *sourceNode, Node *destNode, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    setAcceptedMouseButtons(0);
    dest = destNode;
    source = sourceNode;
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
}

Node* Edge::sourceNode() const {
    return source;
}

Node* Edge::destNode() const {
    return dest;
}

void Edge::adjust() {
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const {
    if (!source || !dest)
        return QRectF();

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
            .normalized();
}


void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.0)))
        return;

    if (source->isHighlighted() && dest->isHighlighted()) {
        painter->setPen(QPen(QColor::fromRgb(247, 196, 31, 255), 1, Qt::SolidLine));
    } else {
        painter->setPen(QPen(QColor::fromRgb(167, 219, 216, 200), 1, Qt::SolidLine));
    }

    painter->drawLine(line);
}
