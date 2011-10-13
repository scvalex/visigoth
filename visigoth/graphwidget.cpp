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

QVector<Node*> GraphWidget::nodes() const {
    return nodeVector;
}

void GraphWidget::populate() {
    const int NUM_NODES = 60;
    const int NUM_EDGES = 200;
    nodeVector.clear();
    for (int i(0); i < NUM_NODES; ++i) {
        Node *node = new Node(this);
        nodeVector << node;
        scene->addItem(node);
    }
    edges.clear();
    for (int i(0); i < NUM_EDGES; ++i) {
        int a = qrand() % NUM_NODES;
        int b = a;
        while (a == b)
            b = qrand() % NUM_NODES;
        Edge *edge = new Edge(nodeVector[a], nodeVector[b]);
        edges << edge;
        scene->addItem(edge);
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
    foreach (Node *node, nodeVector)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodeVector) {
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
    foreach (Node *node, nodeVector) {
        node->setPos(10 + qrand() % 1000, 10 + qrand() % 600);
    }
    foreach (Edge *edge, edges) {
        edge->adjust();
    }
}
