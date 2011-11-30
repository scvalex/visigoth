/* Thank you Nokia:
 *     http://doc.qt.nokia.com/latest/graphicsview-elasticnodes.html
 */

#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QBrush>

#include "vtools.h"
#include "graphscene.h"
#include "quadtree.h"

class Edge;
class QGraphicsSceneHoverEvent;

class Node : public QObject, public QuadTree::TreeNode
{
Q_OBJECT
public:
    /* Only GraphScene can construct Nodes. */
    friend class GraphScene;

    void addEdge(Edge *edge);

    int tag() const;

    VPointF pos() const;
    void setPos(VPointF pos, bool silent = false);

    /* Return the new position. */
    VPointF calculatePosition(QuadTree::TreeNode& treeNode);

    bool advance();

    QList<Edge*>& edges();
    QVector<Node*> neighbours() const;

    // QuadTree
    int size() const;
    VPointF center() const;
    bool hasChildren() const;
    const QVector<TreeNode*>& children() const;
    vreal width() const;

    QColor& colour();
    void setColour(const QColor &b);

    static void reset();

signals:
    void nodeMoved();

protected:
    explicit Node(GraphScene *graph);
    virtual ~Node();

private:
    static int ALL_NODES;

    QColor myColour;
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
