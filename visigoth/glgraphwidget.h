#ifndef GLGRAPHWIDGET_H
#define GLGRAPHWIDGET_H

#include <QGLWidget>
#include <QList>

class GraphScene;
class Node;
class Algorithm;
class Node;


class GLGraphWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLGraphWidget(QWidget *parent = 0);

    virtual ~GLGraphWidget();

    void setScene(GraphScene *newScene);

    enum MOUSE_MODES {
        MOUSE_IDLE,
        MOUSE_ROTATING,
        MOUSE_TRANSLATING_2D,
        MOUSE_TRANSLATING_XY,
        MOUSE_TRANSLATING_Z,
        MOUSE_DRAGGING
    };

signals:
    void algorithmChanged(Algorithm *newAlgo);
    void hoveringOnNode(Node *node);

protected:
    bool animationRunning();
    void setAnimation(bool enable);

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

public slots:
    void onNodeMoved();
    void set3DMode(bool enabled);

private:
    void drawGraphGL();
    void drawNode(Node *node);
    void initProjection();
    Node *selectGL(int x, int y);

    GraphScene *myScene;
    GLfloat cameramat[16];
    GLfloat projmat[16];
    GLint viewmat[4];
    GLfloat zoom;
    int mouseX, mouseY;
    enum MOUSE_MODES mouseMode;
    Node *draggedNode;

    bool mode3d;
    bool running;
    int animTimerId;
};

#endif // GLGRAPHWIDGET_H
