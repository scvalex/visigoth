#include "node.h"
#include "edge.h"
#include <QPainter>

Edge::Edge(Node *sourceNode, Node *destNode) :
    myColour(QColor::fromRgbF(0.0, 0.0, 1.0, 0.5)),
    isHighlighted(false)
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

QColor& Edge::colour() {
    return myColour;
}

void Edge::setColour(const QColor &c) {
    myColour = c;
}

void Edge::highlight() {
    isHighlighted = true;
}

void Edge::unHighlight() {
    isHighlighted = false;
}
