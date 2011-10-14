#include "edge.h"
#include "graphwidget.h"
#include "node.h"

#include <QGraphicsScene>
#include <QPainter>

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

QPointF Node::calculateForces() {
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return newPos;
    }

    // Sum up all the forces pushing away.
    qreal xvel = 0;
    qreal yvel = 0;

    foreach (Node *node, graph->nodes()) {
        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx*dx + dy*dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    double weight = (edgeList.size() + 1) * 10;
    foreach (Edge *edge, edgeList) {
        QPointF vec;
        if (edge->sourceNode() == this)
            vec = mapToItem(edge->destNode(), 0, 0);
        else
            vec = mapToItem(edge->sourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    newPos = pos() + QPointF(xvel, yvel);
    return newPos;
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
