#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QColor>

#include "graphscene.h"

class Node;

class Edge
{
public:
    /* Only GraphScene can construct edges */
    friend class GraphScene;

    Node* sourceNode() const;
    Node* destNode() const;

    QColor& colour();
    void setColour(const QColor &c);

    bool highlighted() const;
    void highlight();
    void unHighlight();

protected:
    explicit Edge(Node *sourceNode, Node *destNode);

private:
    Node *dest;
    Node *source;
    QColor myColour;
    bool isHighlighted;
};

#endif // EDGE_H
