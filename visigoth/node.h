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
    VPointF calculatePosition3D(QVector<Node*>& nodes);

    bool advance();
    void setAllowAdvance(bool allow);

    QList<Edge*>& edges();
    QVector<Node*> neighbours() const;

    // QuadTree
    int size() const;
    VPointF center() const;
    const QVector<TreeNode*>& children() const;
    vreal width() const;

    QColor& colour();
    void setColour(const QColor &b);

    bool highlighted() const;
    void setHighlight(bool enabled);

    static void reset();

signals:
    void nodeMoved();

protected:
    explicit Node(GraphScene *graph);
    virtual ~Node();

private:
    static int ALL_NODES;

    int myTag;

    GraphScene *graph;
    QList<Edge*> edgeList;

    VPointF curPos;
    VPointF newPos;
    bool allowAdvance;

    // vars for average length
    bool visited;
    int distance;

    QColor myColour;
    bool isHighlighted;

    VPointF calculateNonEdgeForces(TreeNode* treeNode);
};

#endif // NODE_H
