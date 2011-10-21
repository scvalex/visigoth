#include "algorithms.h"
#include "edge.h"
#include "graphwidget.h"
#include "node.h"
#include "randomgenerator.h"
#include "francescogenerator.h"

#include <cmath>

#include <QAbstractAnimation>
#include <QDebug>
#include <QGraphicsScene>
#include <QKeyEvent>

GraphWidget::GraphWidget(QWidget *parent) :
    QGraphicsView(parent),
    helping(true),
    helpText(),
    isPlaying(true),
    isRunning(false),
    timerId(0)
{
    setMinimumSize(HELP_WIDTH + 10, HELP_HEIGHT + 10);
    myScene = new QGraphicsScene(this);
    myScene->setBackgroundBrush(Qt::black);
    myScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(myScene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    helpText.setText("<h3>Welcome to Visigoth Graph Visualisations</h3>"
                     "<p>Drag nodes to move. Press <em>ESC</em> to close this help.</p>"
                     "<p>Keybindings:"
                     "<ul>"
                     "<li><em>h</em> - show this text</li>"
                     "<li><em>g</em> - generate a new graph</li>"
                     "<li><em>r</em> - randomize node placement</li>"
                     "<li>&lt;<em>spc</em>&gt; - pause/play the animation</li>"
                     "<li>&lt;<em>esc</em>&gt; - return to graph view</li>"
                     "<li><em>0</em> - fit the graph to the screen</li>"
                     "<li><em>A</em> - add a new node using preferential selection with clustering</li>"
                     "</ul>"
                     "</p>"
                     );
    helpText.setTextWidth(HELP_WIDTH - 10);
}

void GraphWidget::populate() {
    generator = new FrancescoGenerator(this, 60);
    int numEdges = generator->populate();

    Algorithms::updatePreference(myScene->items(), 2 * numEdges);

    randomizePlacement();
}

void GraphWidget::itemMoved() {
    isRunning = true;
    setAnimationRunning();
}

void GraphWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_H:
        helping = !helping;
        viewport()->update();
        break;
    case Qt::Key_G:
        myScene->clear();
        hasEdge.clear();
        populate();
        break;
    case Qt::Key_Escape:
        helping = false;
        viewport()->update();
        break;
    case Qt::Key_Plus:
        scaleView(qreal(1.2));
        break;
    case Qt::Key_Minus:
        scaleView(1 / qreal(1.2));
        break;
    case Qt::Key_R:
        randomizePlacement();
        break;
    case Qt::Key_Space:
        playPause();
        break;
    case Qt::Key_0:
        fitToScreen();
        break;
    case Qt::Key_A:
        Algorithms::addVertex(this, (qrand() % 3 ) + 1, qrand() % 100);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *) {
    QPointF topLeft;
    QPointF bottomRight;

    foreach (QGraphicsItem *item, myScene->items()) {
        Node *node = qgraphicsitem_cast<Node*>(item);
        if (!node)
            continue;

        QPointF pos = node->calculateForces();
        if (pos.x() < topLeft.x()) {
            topLeft.setX(pos.x());
        }
        if (pos.y() < topLeft.y()) {
            topLeft.setY(pos.y());
        }
        if (pos.x() > bottomRight.x()) {
            bottomRight.setX(pos.x());
        }
        if (pos.y() > bottomRight.y()) {
            bottomRight.setY(pos.y());
        }
    }

    // Resize the scene to fit all the nodes
    QRectF sceneRect = myScene->sceneRect();
    sceneRect.setLeft(topLeft.x() - 10);
    sceneRect.setTop(topLeft.y() - 10);
    sceneRect.setRight(bottomRight.x() + 10);
    sceneRect.setBottom(bottomRight.y() + 10);

    isRunning = false;
    foreach (QGraphicsItem *item, myScene->items()) {
        Node *node = qgraphicsitem_cast<Node*>(item);
        if (!node)
            continue;

        if (node->advance()) {
            isRunning = true;
        }
    }
    setAnimationRunning();
}

void GraphWidget::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double)2, event->delta() / 240.0));
}

void GraphWidget::paintEvent(QPaintEvent *event) {
    QGraphicsView::paintEvent(event);

    if (helping) {
        QPainter painter(viewport());
        QRectF popup((width() - HELP_WIDTH) / 2, (height() - HELP_HEIGHT) / 2, HELP_WIDTH, HELP_HEIGHT);
        painter.setBrush(QBrush(QColor::fromRgb(140, 140, 255, 150)));
        painter.setPen(QPen(QColor::fromRgb(140, 140, 255, 200), 2));
        painter.drawRoundedRect(popup.adjusted(-2, -2, +2, +2), 4, 4);
        painter.setPen(QPen(QColor::fromRgb(0, 0, 20), 1));
        painter.drawStaticText(popup.left() + 5, popup.top() + 5, helpText);
    }
}

void GraphWidget::scaleView(qreal scaleFactor) {
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;
    scale(scaleFactor, scaleFactor);
}

void GraphWidget::playPause() {
    isPlaying = !isPlaying;
    if (!isPlaying) {
        setAnimationRunning();
    } else {
        setAnimationRunning();
    }
}

void GraphWidget::setAnimationRunning() {
    if (isPlaying && isRunning && !timerId) {
        timerId = startTimer(1000 / 25);
    } else if ((!isPlaying || !isRunning) && timerId) {
        killTimer(timerId);
        timerId = 0;
    }
}

void GraphWidget::randomizePlacement() {
    foreach (QGraphicsItem *item, myScene->items()) {
        if (Node *node = qgraphicsitem_cast<Node*>(item))
            node->setPos(10 + qrand() % 1000, 10 + qrand() % 600);
    }
    foreach (QGraphicsItem *item, myScene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge*>(item))
            edge->adjust();
    }
}

void GraphWidget::fitToScreen() {
    fitInView(myScene->sceneRect(), Qt::KeepAspectRatio);
}

bool GraphWidget::addNewEdge(Edge *edge) {
    if (doesEdgeExist(edge->sourceNode()->tag(), edge->destNode()->tag()))
        return false;
    if (edge->sourceNode()->tag() >= hasEdge.size()) {
        hasEdge.resize(edge->sourceNode()->tag() + 1);
    }
    if (edge->destNode()->tag() >= hasEdge.size()) {
        hasEdge.resize(edge->destNode()->tag() + 1);
    }
    hasEdge[edge->sourceNode()->tag()].insert(edge->destNode()->tag());
    myScene->addItem(edge);
    return true;
}

void GraphWidget::addNode(Node *node) {
    myScene->addItem(node);
}

bool GraphWidget::doesEdgeExist(int sourceTag, int destTag) {
    if (0 <= sourceTag && sourceTag < hasEdge.size() &&
        0 <= destTag && destTag < hasEdge.size())
    {
        return hasEdge[sourceTag].contains(destTag) ||
               hasEdge[destTag].contains(sourceTag);
    }
    return false;
}
