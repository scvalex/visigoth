#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode, QGraphicsItem *parent = 0);

    Node* sourceNode() const;
    Node* destNode() const;

    void adjust();

private:
    Node *dest;
    Node *source;
};

#endif // EDGE_H
