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
    QColor myColour;
    bool isHighlighted;

    void setColour(const QColor &c);
    void highlight();
    void unHighlight();


protected:
    explicit Edge(Node *sourceNode, Node *destNode);

private:
    Node *dest;
    Node *source;


};

#endif // EDGE_H
