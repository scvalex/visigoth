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
