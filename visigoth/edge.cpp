#include "edge.h"

Edge::Edge(Node *sourceNode, Node *destNode, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    dest(destNode),
    source(sourceNode)
{
}

Node* Edge::sourceNode() const {
    return source;
}

Node* Edge::destNode() const {
    return dest;
}

void Edge::adjust() {
}

QRectF Edge::boundingRect() const {
    if (!source || !dest)
        return QRectF();
    return QRectF();
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!source || !dest)
        return;
    return;
}
