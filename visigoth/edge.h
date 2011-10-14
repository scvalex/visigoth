#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
public:
    explicit Edge(Node *sourceNode, Node *destNode, QGraphicsItem *parent = 0);

    Node* sourceNode() const;
    Node* destNode() const;

    enum { Type = UserType + 2 };
    int type() const { return Type; }

    void adjust();

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Node *dest;
    QPointF destPoint;
    Node *source;
    QPointF sourcePoint;
};

#endif // EDGE_H
