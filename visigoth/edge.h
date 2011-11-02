#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QColor>

#include "graphscene.h"

class Node;

class Edge : public QGraphicsItem
{
public:
    /* Only GraphScene can construct edges */
    friend class GraphScene;

    Node* sourceNode() const;
    Node* destNode() const;

    enum { Type = UserType + 2 };
    int type() const { return Type; }

    void adjust();

    QColor& colour();

protected:
    explicit Edge(Node *sourceNode, Node *destNode, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Node *dest;
    QPointF destPoint;
    Node *source;
    QPointF sourcePoint;

    QColor myColour;
};

#endif // EDGE_H
