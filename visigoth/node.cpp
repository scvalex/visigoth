#include "edge.h"
#include "graphscene.h"
#include "node.h"
#include "quadtree.h"

#include <QPainter>

#include <cmath>
#include <stdexcept>

int Node::ALL_NODES(0);

Node::Node(GraphScene *graph, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    //myBrush(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256, 180)),
    myBrush(QColor::fromRgbF(0.0, 1.0, 0.3, 0.7)),
    graph(graph),
    hovering(false),
    visited(false),
    distance(0)
{
    myTag = ALL_NODES++;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(100);
    setAcceptHoverEvents(true);
}

int Node::tag() const {
    return myTag;
}

void Node::addEdge(Edge *edge) {
    edgeList << edge;
}

QPointF Node::calculatePosition(TreeNode &treeNode) {
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return newPos;
    }

    // Calculate non-edge forces
    QPointF vel = calculateNonEdgeForces(&treeNode);

    qreal xvel = vel.x();
    qreal yvel = vel.y();

    // Now all the forces that pulling items together
    double weight = (edgeList.size() + 1) * 10;

    foreach (Edge *edge, edgeList) {
        QPointF vec;
        if (edge->sourceNode() == this) {
            vec = mapToItem(edge->destNode(), 0, 0);
        } else {
            vec = mapToItem(edge->sourceNode(), 0, 0);
        }
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1) {
        xvel = yvel = 0;
    }

    newPos = pos() + QPointF(xvel, yvel);

    return newPos;
}

QPointF Node::calculateNonEdgeForces(QuadTree::TreeNode* treeNode) {
    if (treeNode->size() < 1) {
        return QPointF(0, 0);
    }

    QPointF vec(this->pos().x() - treeNode->center().x(),
                this->pos().y() - treeNode->center().y());
    qreal dx = vec.x();
    qreal dy = vec.y();
    qreal distance = sqrt(dx*dx + dy*dy);

    QPointF vel;

    if (treeNode->isFarEnough(distance) || treeNode->size() == 1) {
        double l = 2.0 * (dx*dx + dy*dy);

        if (l > 0) {
            vel = QPointF((dx * 150.0) / l, (dy * 150.0) / l);
        } else {
            vel = QPointF(0, 0);
        }
    } else {
        qreal xvel = 0;
        qreal yvel = 0;
        foreach (TreeNode* child, treeNode->children()) {
            QPointF velCh = calculateNonEdgeForces(child);
            xvel += velCh.x();
            yvel += velCh.y();
        }
        vel = QPointF(xvel, yvel);
    }
    return vel;
}

/* Called by GraphWidget repeatedly. */
bool Node::advance() {
    if (newPos == pos())
        return false;
    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const {
    return QRectF(-10, -10, 20, 20);
}

QPainterPath Node::shape() const {
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(Qt::NoPen);
    if (!hovering) {
        painter->setBrush(myBrush);
    } else {
        QColor lighter = myBrush.color();
        lighter.setAlpha(255);
        painter->setBrush(QBrush(lighter));
    }
    painter->drawEllipse(-10, -10, 20, 20);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList) {
            edge->adjust();
        }
        graph->itemMoved();
        break;
    default:
        break;
    }

    return QGraphicsItem::itemChange(change, value);
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    hovering = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    hovering = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

QList<Edge*>& Node::edges() {
    return edgeList;
}

QVector<Node*> Node::neighbours() const {
    QVector<Node*> ns;
    foreach (Edge *e, edgeList) {
        if (e->sourceNode()->tag() == this->tag())
            ns << e->destNode();
        if (e->destNode()->tag() == this->tag())
            ns << e->sourceNode();
    }
    return ns;
}

void Node::reset() {
    ALL_NODES = 0;
}

// needed for bipartite since we have to delete the Vs
void Node::setAllNodes(int i){
    ALL_NODES = i;
}

int Node::getAllNodes(){
    return ALL_NODES;
}

int Node::size() const {
    return 1;
}

QPointF Node::center() const {
    return pos();
}

bool Node::hasChildren() const {
    return false;
}

const QVector<QuadTree::TreeNode*>& Node::children() const {
    throw std::runtime_error("Node: calling children() on a terminal node");
}

qreal Node::width() const {
    return 0;
}

bool Node::getVisited(){
    return visited;
}

void Node::setVisited(bool v){
    visited = v ;
}
int Node::getDistance(){
    return distance;
}

void Node::setDistance(int d){
    distance = d;
}

QBrush& Node::brush() {
    return myBrush;
}

void Node::setBrush(const QBrush &b) {
    myBrush = b;
}
