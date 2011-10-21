#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QList>
#include <QSet>
#include <QStaticText>
#include <QVector>

class Edge;
class Node;
class Preferential;
class QGraphicsScene;
class QPaintEvent;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);
    virtual ~GraphWidget();

    void populate();

    void itemMoved();

    Node* addNode(Node *n);
    bool addNewEdge(Edge *e);
    bool doesEdgeExist(int sourceTag, int destTag);

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

    void fitToScreen();
    void playPause();
    void randomizePlacement();
    void scaleView(qreal scaleFactor);
    void setAnimationRunning();

private:
    static const float HELP_WIDTH = 300;
    static const float HELP_HEIGHT = 500;

    Preferential *algo;
    QVector<QSet<int> > hasEdge;
    bool helping;
    QStaticText helpText;
    bool isPlaying;
    bool isRunning;
    QGraphicsScene *myScene;
    int timerId;
};

#endif // GRAPHWIDGET_H
