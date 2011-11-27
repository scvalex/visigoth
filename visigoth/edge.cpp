#include "node.h"
#include "edge.h"
#include <QPainter>

Edge::Edge(Node *sourceNode, Node *destNode) :
    //myColour(QColor::fromRgb(0, 165, 211, 238))
    myColour(QColor::fromRgbF(0.0, 0.0, 1.0, 0.5))
{
    dest = destNode;
    source = sourceNode;
    source->addEdge(this);
    dest->addEdge(this);
}

Node* Edge::sourceNode() const {
    return source;
}

Node* Edge::destNode() const {
    return dest;
}

QRectF Edge::boundingRect() const {
    if (!source || !dest)
        return QRectF();

    return QRectF(source->pos(),
                QSizeF(dest->pos().x() - source->pos().x(),
                        dest->pos().y() - source->pos().y())
                ).normalized();
}

QColor& Edge::colour() {
    return myColour;
}
