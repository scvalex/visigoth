#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QList>
#include <QSet>
#include <QStaticText>
#include <QVector>

#include "abstractgraphwidget.h"
#include "node.h"

class GraphScene;
class Preferential;
class Bipartite;
class QGraphicsScene;
class QPaintEvent;

class GraphWidget : public QGraphicsView, public AbstractGraphWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);

    QVector<Node*> nodes() const;

    virtual ~GraphWidget();

    void populate();

    void itemMoved();

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

    void getUserInput();
    void fitToScreen();
    void playPause();
    void scaleView(qreal scaleFactor);
    void setAnimationRunning();

private:
    static const float HELP_WIDTH = 300;
    static const float HELP_HEIGHT = 500;
    static const int BASE_NODES_NUMBER = 100;

    bool helping;
    QStaticText helpText;
    bool isPlaying;
    bool isRunning;
    GraphScene *myScene;
    int timerId;
    int nodesNumber;
};

#endif // GRAPHWIDGET_H
