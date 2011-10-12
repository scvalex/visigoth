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

    void itemMoved();

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);

    void scaleView(qreal scaleFactor);
    void randomizePlacement();

private:
    QGraphicsScene *scene;
    int timerId;
};

#endif // GRAPHWIDGET_H
