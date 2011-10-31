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

#include "graphscene.h"
#include "quadtree.h"

class Edge;
class QGraphicsSceneHoverEvent;

class Node : public QGraphicsItem, public QuadTree::TreeNode
{
public:
    /* Only GraphScene can construct Nodes. */
    friend class GraphScene;

    void addEdge(Edge *edge);

    int tag() const;

    /* Return the new position. */
    QPointF calculatePosition(QuadTree::TreeNode& treeNode);

    bool advance();

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QList<Edge*>& edges();
    QVector<Node*> neighbours() const;

    int size() const;
    QPointF center() const;
    bool hasChildren() const;
    const QVector<TreeNode*>& children() const;
    qreal width() const;

    void highlight();
    void unHighlight();
    bool isHighlighted() const;

    static void reset();
protected:
    explicit Node(GraphScene *graph, QGraphicsItem *parent = 0);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    static int ALL_NODES;

    QBrush brush;
    QList<Edge*> edgeList;
    GraphScene *graph;
    bool hovering;
    int myTag;
    QPointF newPos;

    QPointF mapPoint(QPointF source, QPointF dest);
    QPointF calculateNonEdgeForces(TreeNode* treeNode);

    bool highlighted;
};

#endif // NODE_H
