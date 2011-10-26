#include "edge.h"
#include "graphscene.h"
#include "node.h"
#include "treenode.h"
#include "treecode.h"

#include <QPainter>

#include <cmath>
#include <stdexcept>

int Node::ALL_NODES(0);

Node::Node(GraphScene *graph, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    brush(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256, 180)),
    graph(graph),
    hovering(false)
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

QPointF Node::calculatePosition() {
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return newPos;
    }

    // Calculate non-edge forces
    qreal xvel = 0;
    qreal yvel = 0;

    foreach (Node* node, graph->nodes()) {
        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();

        qreal l = 2 * (dx*dx + dy*dy);

        if (l > 0) {
           xvel += (dx * 150) / l;
           yvel += (dy * 150) / l;
        }

    }

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

/*
QPointF Node::calculateNonEdgeForces(TreeNode* treeNode)
{
    if (treeNode->getSize() < 1)
        return QPointF(0, 0);

    QPointF vec(this->pos().x() - treeNode->getCenter().x(),
                this->pos().y() - treeNode->getCenter().y());
    qreal dx = vec.x();
    qreal dy = vec.y();

    qreal distance = sqrt(dx*dx + dy*dy);

    QPointF vel;
    if (treeNode->isFarEnough(distance) || treeNode->getSize() == 1) {
        double l = 2.0 * (dx*dx + dy*dy);

        if (l > 0) {
            vel = QPointF((dx * 150.0) / l, (dy * 150.0) / l);
        } else {
            vel = QPointF(0, 0);
        }
    } else {
        qreal xvel = 0;
        qreal yvel = 0;
        foreach (TreeNode* child, treeNode->getChildren()) {
            QPointF velCh = calculateNonEdgeForces(child);
            xvel += velCh.x();
            yvel += velCh.y();
        }
        vel = QPointF(xvel, yvel);
    }
    return vel;
}
*/

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
        painter->setBrush(brush);
    } else {
        QColor lighter = brush.color();
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
        if (e->sourceNode() == this)
            ns << e->destNode();
        if (e->destNode() == this)
            ns << e->sourceNode();
    }
    return ns;
}

void Node::reset() {
    ALL_NODES = 0;
}

int Node::getSize() const {
    return 1;
}

QPointF Node::getCenter() const {
    return pos();
}

QVector<TreeNode*>& Node::getChildren() {
    throw std::runtime_error("Node: calling getChildren() on a terminal node");
}

qreal Node::getWidth() const {
    return 0;
}
