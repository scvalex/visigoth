#include "edge.h"
#include "graphwidget.h"
#include "node.h"
#include "treenode.h"

#include <QGraphicsScene>
#include <QPainter>
#include <cmath>
#include <stdexcept>

#include <iostream>

Node::Node(int tag, GraphWidget *graph, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    brush(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256, 180)),
    graph(graph),
    hovering(false),
    myTag(tag)
{
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

QPointF Node::calculatePosition(TreeNode* treeNode) {
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return newPos;
    }

    // Calculate non-edge forces
    QPointF nonEdge = calculateNonEdgeForces(treeNode);
    std::cout << "-----------------------------\n";

    qreal xvel = nonEdge.x();
    qreal yvel = nonEdge.y();

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

QPointF Node::calculateNonEdgeForces(TreeNode* treeNode)
{
    /*
    std::cout << "size: " << treeNode->getSize() << "\n";
    if (treeNode->getSize() < 1) {
        return QPointF(0, 0);
    }

    QPointF vec(this->pos().x() - treeNode->getCenter().x(),
                this->pos().y() - treeNode->getCenter().y());
    qreal dx = vec.x();
    qreal dy = vec.y();

    qreal distance = sqrt(dx*dx + dy*dy);
    if (distance < minDistance || treeNode->getSize() == 1) {
        std::cout << "stop recursing\n";
        double l = 2.0 * (dx*dx + dy*dy);
        QPointF vel;

        if (l > 0) {
            vel = QPointF((dx * 150.0) / l, (dy * 150.0) / l);
        } else {
            vel = QPointF(0, 0);
        }

        return vel;
    } else {
        qreal xvel = 0;
        qreal yvel = 0;
        std::cout << "recursing in\n";
        foreach (TreeNode* child, treeNode->getChildren()) {
            QPointF vel = calculateNonEdgeForces(child);
            xvel += vel.x();
            yvel += vel.y();
        }
        std::cout << "recursing out\n";
        return QPointF(xvel, yvel);
    }
    */
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
        foreach (Edge *edge, edgeList)
            edge->adjust();
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

int Node::getSize()
{
    return 1;
}

QPointF Node::getCenter()
{
    return pos();
}

QVector<TreeNode*>& Node::getChildren() {
    throw std::runtime_error("Node: calling getChildren() on a terminal node");
}
