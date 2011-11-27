/* Thank you Nokia:
 *     http://doc.qt.nokia.com/latest/graphicsview-elasticnodes.html
 */

#ifndef NODE_H
#define NODE_H

#include <QBrush>

#include "vtools.h"
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

    VPointF pos() const;
    void setPos(VPointF pos);

    /* Return the new position. */
    VPointF calculatePosition(QuadTree::TreeNode& treeNode);

    bool advance();

    QList<Edge*>& edges();
    QVector<Node*> neighbours() const;

    // Statistics
    bool getVisited();
    void setVisited(bool v);
    int getDistance();
    void setDistance(int v);

    // QuadTree
    int size() const;
    VPointF center() const;
    bool hasChildren() const;
    const QVector<TreeNode*>& children() const;
    vreal width() const;

    QBrush& brush();
    void setBrush(const QBrush &b);

    static void reset();

protected:
    explicit Node(GraphScene *graph);

private:
    static int ALL_NODES;

    QBrush myBrush;
    QList<Edge*> edgeList;
    GraphScene *graph;
    int myTag;

    VPointF curPos;
    VPointF newPos;

    // vars for average length
    bool visited;
    int distance;

    VPointF calculateNonEdgeForces(TreeNode* treeNode);
};

#endif // NODE_H
