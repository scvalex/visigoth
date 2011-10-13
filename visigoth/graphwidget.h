#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QList>
#include <QVector>

class Edge;
class Node;
class QGraphicsScene;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);

    QVector<Node*> nodes() const;

    void populate();

    void itemMoved();

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);

    void scaleView(qreal scaleFactor);
    void randomizePlacement();

private:
    QList<Edge*> edges;
    QVector<Node*> nodeVector;
    QGraphicsScene *scene;
    int timerId;
};

#endif // GRAPHWIDGET_H
