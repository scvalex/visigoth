#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QList>
#include <QStaticText>
#include <QVector>

class Edge;
class GraphGenerator;
class Node;
class QGraphicsScene;
class QPaintEvent;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);

    QVector<Node*> nodes() const;

    void populate();

    void itemMoved();
    QVector<Node*>* getNodeVector();
    QList<Edge*>* getEdgeList();

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

    void scaleView(qreal scaleFactor);
    void randomizePlacement();
    void playPause();
    void fitToScreen();

private:
    static const float HELP_WIDTH = 300;
    static const float HELP_HEIGHT = 500;

    GraphGenerator *generator;
    // use this to get the total edges of the Graph
    QList<Edge*> edges;
    bool helping;
    QStaticText helpText;
    // stores the nodes of the graph
    QVector<Node*> nodeVector;
    QGraphicsScene *scene;
    int timerId;
    bool isPlaying;
};

#endif // GRAPHWIDGET_H
