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

class Node : public QuadTree::TreeNode
{
public:
    /* Only GraphScene can construct Nodes. */
    friend class GraphScene;

    void addEdge(Edge *edge);

    int tag() const;

    QPointF pos() const;
    void setPos(QPointF pos);
    void setPos(qreal x, qreal y);

    /* Return the new position. */
    QPointF calculatePosition(QuadTree::TreeNode& treeNode);

    bool advance();

    QRectF boundingRect() const;
    QPainterPath shape() const;

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

    static void reset();

    // 3rd dimension.
    void setZ(float z);
    float getZ();
    void setPos3(float x, float y, float z);

protected:
    explicit Node(GraphScene *graph);

private:
    static int ALL_NODES;

    QBrush myBrush;
    QList<Edge*> edgeList;
    GraphScene *graph;
    bool hovering;
    int myTag;

    QPointF curPos;
    QPointF newPos;

    // vars for average length
    bool visited;
    int distance;

    // 3rd dimension.
    float z;
    float newZ;

    QPointF mapPoint(QPointF source, QPointF dest);
    QPointF calculateNonEdgeForces(TreeNode* treeNode);
};

#endif // NODE_H
