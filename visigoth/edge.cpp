#include "node.h"
#include "edge.h"
#include <QPainter>

Edge::Edge(Node *sourceNode, Node *destNode) :
    dest(destNode),
    source(sourceNode),
    myColour(QColor::fromRgbF(0.0, 0.0, 1.0, 0.5)),
    isHighlighted(false)
{
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

bool Edge::highlighted() const {
    return isHighlighted;
}

void Edge::highlight() {
    isHighlighted = true;
}

void Edge::unHighlight() {
    isHighlighted = false;
}
