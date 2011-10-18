#include "edge.h"
#include "graphwidget.h"
#include "node.h"
#include "randomgenerator.h"
#include "francescogenerator.h"
#include "treecode.h"

#include <QAbstractAnimation>
#include <QDebug>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <cmath>

GraphWidget::GraphWidget(QWidget *parent) :
    QGraphicsView(parent),
    helping(true),
    helpText(),
    isPlaying(true),
    isRunning(false),
    timerId(0)
{
    setMinimumSize(HELP_WIDTH + 10, HELP_HEIGHT + 10);
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
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
                     "</ul>"
                     "</p>"
                     );
    helpText.setTextWidth(HELP_WIDTH - 10);
}

QVector<Node*> GraphWidget::nodes() const {
    return nodeVector;
}

void GraphWidget::populate() {
    generator = new FrancescoGenerator(this, 60);
    generator->populate(nodeVector, edges);

    foreach (Node *node, nodeVector) {
        scene->addItem(node);
    }
    foreach (Edge *edge, edges) {
        scene->addItem(edge);
    }

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
        scene->clear();
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
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *) {
    QPointF topLeft;
    QPointF bottomRight;

    TreeCode treeCode(nodeVector, scene->sceneRect());

    foreach (Node *node, nodeVector) {
        QPointF pos = node->calculatePosition(treeCode.getRoot());
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
    QRectF sceneRect = scene->sceneRect();
    sceneRect.setLeft(topLeft.x() - 10);
    sceneRect.setTop(topLeft.y() - 10);
    sceneRect.setRight(bottomRight.x() + 10);
    sceneRect.setBottom(bottomRight.y() + 10);

    isRunning = false;
    foreach (Node *node, nodeVector) {
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
    foreach (Node *node, nodeVector) {
        node->setPos(10 + qrand() % 1000, 10 + qrand() % 600);
    }
    foreach (Edge *edge, edges) {
        edge->adjust();
    }
}

void GraphWidget::fitToScreen() {
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}
