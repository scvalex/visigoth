#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QList>
#include <QSet>
#include <QStaticText>
#include <QVector>

class GraphScene;
class Preferential;
<<<<<<< HEAD
class Bipartite;
class QGraphicsScene;
=======
>>>>>>> origin/master
class QPaintEvent;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);
    virtual ~GraphWidget();

    void populate();

    void itemMoved();

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

    void fitToScreen();
    void playPause();
    void scaleView(qreal scaleFactor);
    void setAnimationRunning();

private:
    static const float HELP_WIDTH = 300;
    static const float HELP_HEIGHT = 500;

    Preferential *algo;
    Bipartite * bip;
    QVector<QSet<int> > hasEdge;
    bool helping;
    QStaticText helpText;
    bool isPlaying;
    bool isRunning;
    GraphScene *myScene;
    int timerId;
};

#endif // GRAPHWIDGET_H
