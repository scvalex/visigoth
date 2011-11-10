#include "node.h"
#include "edge.h"
#include <QPainter>

Edge::Edge(Node *sourceNode, Node *destNode, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    //brush(QColor::fromRgb(0, 165, 211, 238))
    myColour(QColor::fromRgbF(0.0, 0.0, 1.0, 0.5))
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

    painter->setPen(QPen(myColour, 1, Qt::SolidLine));
    painter->drawLine(line);
}

QColor& Edge::colour() {
    return myColour;
}
