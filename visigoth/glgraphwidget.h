#ifndef GLGRAPHWIDGET_H
#define GLGRAPHWIDGET_H

#include <QGLWidget>

#include "abstractgraphwidget.h"
#include "graphscene.h"

class Algorithm;

class GLGraphWidget : public QGLWidget, public AbstractGraphWidget
{
    Q_OBJECT
public:
    explicit GLGraphWidget(QWidget *parent = 0);

    void init();
    void populate();
    void itemMoved();

    enum MOUSE_MODES {
        MOUSE_IDLE,
        MOUSE_ROTATING,
        MOUSE_TRANSLATING,
        MOUSE_TRANSLATING2,
        MOUSE_DRAGGING
    };

signals:
    void algorithmChanged(Algorithm *newAlgo);

protected:
    void setAnimationRunning();
    void playPause();
    void scaleView(qreal scaleFactor);
    void fitToScreen();

    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    void drawGraphGL();
    void drawNode(Node *node);
    void initProjection();
    Node *selectGL(int x, int y);

    GraphScene *myScene;
    GLfloat cameramat[16];
    GLfloat zoom;
    int mouseX, mouseY;
    enum MOUSE_MODES mouseMode;
    Node *draggedNode;

    bool helping;
    bool isPlaying;
    bool isRunning;
    int timerId;
};

#endif // GLGRAPHWIDGET_H
