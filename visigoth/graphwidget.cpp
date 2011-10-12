#include "graphwidget.h"
#include "node.h"

#include <QGraphicsScene>

GraphWidget::GraphWidget(QWidget *parent) :
    QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    setScene(scene);
}

void GraphWidget::populate() {
    for (int i(0); i < 1000; ++i) {
        Node *node = new Node();
        node->setPos(10 + qrand() % 1000, 10 + qrand() % 1000);
        scene->addItem(node);
    }
}
