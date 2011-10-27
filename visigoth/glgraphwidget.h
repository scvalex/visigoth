#ifndef GLGRAPHWIDGET_H
#define GLGRAPHWIDGET_H

#include <QGLWidget>

#include "abstractgraphwidget.h"
#include "graphscene.h"

class GLGraphWidget : public QGLWidget, public AbstractGraphWidget
{
    Q_OBJECT
public:
    explicit GLGraphWidget(QWidget *parent = 0);

    void populate();
    void itemMoved();

    enum MOUSE_MODES {
        MOUSE_IDLE,
        MOUSE_ROTATING,
        MOUSE_TRANSLATING,
        MOUSE_TRANSLATING2,
        MOUSE_DRAGGING
    };

protected:
    void setAnimationRunning();
    void playPause();
    void getUserInput();
    void scaleView(qreal scaleFactor);
    void fitToScreen();

    void wheelEvent(QWheelEvent *event);
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
    void initProjection();

    GraphScene *myScene;
    GLfloat cameramat[16];
    GLfloat zoom;
    int mouseX, mouseY;
    enum MOUSE_MODES mouseMode;
    static const int BASE_NODES_NUMBER = 100;

    bool helping;
    bool isPlaying;
    bool isRunning;
    int timerId;
};

#endif // GLGRAPHWIDGET_H
