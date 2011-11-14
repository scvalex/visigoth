/* Thank you Nokia:
 *     http://doc.qt.nokia.com/latest/graphicsview-elasticnodes.html
 */

#ifndef NODE_H
#define NODE_H

#include <QBrush>
#include <QGraphicsItem>
#include <QVariant>
#include <QToolTip>

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
    int getSize() const;
    QPointF getCenter() const;
    QVector<TreeNode*>& getChildren();
    qreal getWidth() const;
    bool getVisited();
    void setVisited(bool v);
    int getDistance();
    void setDistance(int v);
    int size() const;
    QPointF center() const;
    bool hasChildren() const;
    const QVector<TreeNode*>& children() const;
    qreal width() const;

    QBrush& brush();
    void setBrush(const QBrush &b);
    static void setAllNodes(int i);
    static int getAllNodes();

    static void reset();

protected:
    explicit Node(GraphScene *graph, QGraphicsItem *parent = 0);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    static int ALL_NODES;

    QBrush myBrush;
    QList<Edge*> edgeList;
    GraphScene *graph;
    bool hovering;
    int myTag;
    QPointF newPos;

    // vars for average length
    bool visited;
    int distance;

    QPointF mapPoint(QPointF source, QPointF dest);
    QPointF calculateNonEdgeForces(TreeNode* treeNode);
};

#endif // NODE_H
