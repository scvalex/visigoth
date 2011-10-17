/* Thank you Nokia:
 *     http://doc.qt.nokia.com/latest/graphicsview-elasticnodes.html
 */

#ifndef NODE_H
#define NODE_H

#include "treeobject.h"

#include <QBrush>
#include <QGraphicsItem>
#include <QVariant>

class Edge;
class GraphWidget;
class QGraphicsSceneHoverEvent;

class Node : public QGraphicsItem, public TreeObject
{
public:
    explicit Node(int tag, GraphWidget *graph, QGraphicsItem *parent = 0);

    void addEdge(Edge *edge);

    int tag() const;

    /* Return the new position. */
    QPointF calculateForces();

    bool advance();

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	// Tree object functions
	int getSize();
	QPointF getCenter();
	QVector<TreeObject*>* getChildren();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    QBrush brush;
    QList<Edge*> edgeList;
    GraphWidget *graph;
    bool hovering;
    int myTag;
    QPointF newPos;
};

#endif // NODE_H
