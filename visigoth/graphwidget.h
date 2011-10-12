#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>

class QGraphicsScene;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);

    void populate();

private:
    QGraphicsScene *scene;
};

#endif // GRAPHWIDGET_H
