#include "edge.h"
#include "graphwidget.h"
#include "node.h"

#include <cmath>
#include <QGraphicsScene>
#include <QKeyEvent>

GraphWidget::GraphWidget(QWidget *parent) :
    QGraphicsView(parent),
    timerId(0)
{
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    //FIXME: look into scene index
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
}

void GraphWidget::populate() {
    QVector<Node*> nodes;
    const int NUM_NODES = 10;
    const int NUM_EDGES = 60;
    for (int i(0); i < NUM_NODES; ++i) {
        Node *node = new Node(this);
        nodes << node;
        scene->addItem(node);
    }
    for (int i(0); i < NUM_EDGES; ++i) {
        int a = qrand() % NUM_NODES;
        int b = a;
        while (a == b)
            b = qrand() % NUM_NODES;
        Edge *edge = new Edge(nodes[a], nodes[b]);
        nodes[a]->addEdge(edge);
        nodes[b]->addEdge(edge);
    }
    randomizePlacement();
}

void GraphWidget::itemMoved() {
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Plus:
        scaleView(qreal(1.2));
        break;
    case Qt::Key_Minus:
        scaleView(1 / qreal(1.2));
        break;
    case Qt::Key_Space:
        randomizePlacement();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *) {
    QList<Node*> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (Node *node = qgraphicsitem_cast<Node*>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double)2, event->delta() / 240.0));
}

void GraphWidget::scaleView(qreal scaleFactor) {
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;
    scale(scaleFactor, scaleFactor);
}

void GraphWidget::randomizePlacement() {
    foreach (QGraphicsItem *item, scene->items()) {
        if (qgraphicsitem_cast<Node*>(item))
            item->setPos(10 + qrand() % 1000, 10 + qrand() % 600);
    }
}
