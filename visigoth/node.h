/* Thank you Nokia:
 *     http://doc.qt.nokia.com/latest/graphicsview-elasticnodes.html
 */

#ifndef NODE_H
#define NODE_H

#include <QBrush>
#include <QGraphicsItem>
#include <QVariant>
#include <QToolTip>
#include <sstream>

#include "treenode.h"

class Edge;
class GraphWidget;
class QGraphicsSceneHoverEvent;

class Node : public QGraphicsItem, public TreeNode
{
public:
    explicit Node(GraphWidget *graph, QGraphicsItem *parent = 0);

    void addEdge(Edge *edge);

    int tag() const;

    /* Return the new position. */
    QPointF calculatePosition(QVector<Node*> nodeVector);

    bool advance();

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QList<Edge*>& edges();
    int getSize() const;
    QPointF getCenter() const;
    QVector<TreeNode*>& getChildren();
    qreal getWidth() const;

    static void reset();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    static int ALL_NODES;

    QBrush brush;
    QList<Edge*> edgeList;
    GraphWidget *graph;
    bool hovering;
    int myTag;
    QPointF newPos;

    QPointF mapPoint(QPointF source, QPointF dest);
    QPointF calculateNonEdgeForces(TreeNode* treeNode);
};

#endif // NODE_H
