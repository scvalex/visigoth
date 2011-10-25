#include <QKeyEvent>
#include <cmath>
#include <GL/gl.h>

#include "glgraphwidget.h"
#include "glancillary.h"        // gla*()

#include "edge.h"
#include "node.h"
#include "treecode.h"


/****************************
 * GraphWidget imitation code (public)
 ***************************/

GLGraphWidget::GLGraphWidget(QWidget *parent) :
    QGLWidget(parent),
    zoom(1.0),
    isPlaying(true),
    isRunning(false),
    helping(true),
    timerId(0)
{
    setFocusPolicy(Qt::StrongFocus);

    myScene = new GraphScene(this);
    myScene->setBackgroundBrush(Qt::black);
    myScene->setItemIndexMethod(QGraphicsScene::NoIndex);
}

void GLGraphWidget::populate()
{
    myScene->populate();
    myScene->randomizePlacement();
}

void GLGraphWidget::itemMoved()
{
    isRunning = true;
    setAnimationRunning();
}



/****************************
 * GraphWidget imitation code (protected)
 ***************************/

void GLGraphWidget::setAnimationRunning()
{
    if (isPlaying && isRunning && !timerId)
        timerId = startTimer(1000 / 25);
    else if ((!isPlaying || !isRunning) && timerId) {
        killTimer(timerId);
        timerId = 0;
    }
}

void GLGraphWidget::playPause()
{
    isPlaying = !isPlaying;
    setAnimationRunning();

    this->repaint();
}

void GLGraphWidget::scaleView(qreal scaleFactor)
{
    zoom *= scaleFactor;
    this->initProjection();
}

void GLGraphWidget::fitToScreen()
{
    // FIXME: Implement GLGraphWidget::fitToScreen()
}

void GLGraphWidget::wheelEvent(QWheelEvent *event)
{
    // FIXME: Implement GLGraphWidget::wheelEvent()
    scaleView(pow((double)2, event->delta() / 240.0));

    this->repaint();
}

void GLGraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_H:
        helping = !helping;
        this->repaint();
        break;
    case Qt::Key_G:
        myScene->reset();
        populate();
        break;
    case Qt::Key_Escape:
        helping = false;
        this->repaint();
        break;
    case Qt::Key_Equal:
    case Qt::Key_Plus:
        scaleView(1.2);
        break;
    case Qt::Key_Minus:
        scaleView(1.0/1.2);
        break;
    case Qt::Key_R:
        myScene->randomizePlacement();
        break;
    case Qt::Key_Space:
        playPause();
        break;
    case Qt::Key_0:
        fitToScreen();
        break;
    case Qt::Key_A:
        myScene->addVertex();
        break;
    case Qt::Key_Left:
        glaCameraTranslatef(cameramat, -20.0, 0.0, 0.0);
        break;
    case Qt::Key_Up:
        glaCameraTranslatef(cameramat, 0.0, -20.0, 0.0);
        break;
    case Qt::Key_Right:
        glaCameraTranslatef(cameramat, 20.0, 0.0, 0.0);
        break;
    case Qt::Key_Down:
        glaCameraTranslatef(cameramat, 0.0, 20.0, 0.0);
        break;
    default:
        QGLWidget::keyPressEvent(event);
    }

    this->repaint();
}

void GLGraphWidget::timerEvent(QTimerEvent *)
{
    QPointF topLeft;
    QPointF bottomRight;

    TreeCode treeCode(myScene->sceneRect());

    QVector<Node*> nodeVector;
    foreach (QGraphicsItem* item, myScene->items()) {
        Node* node = qgraphicsitem_cast<Node*>(item);
        if (node)
            nodeVector.append(node);
    }

    foreach (Node* node, nodeVector) {
        QPointF pos = node->calculatePosition(nodeVector);

        if (pos.x() < topLeft.x())
            topLeft.setX(pos.x());
        if (pos.y() < topLeft.y())
            topLeft.setY(pos.y());
        if (pos.x() > bottomRight.x())
            bottomRight.setX(pos.x());
        if (pos.y() > bottomRight.y())
            bottomRight.setY(pos.y());
    }

    // Resize the scene to fit all the nodes
    QRectF sceneRect = myScene->sceneRect();
    sceneRect.setLeft(topLeft.x() - 10);
    sceneRect.setTop(topLeft.y() - 10);
    sceneRect.setRight(bottomRight.x() + 10);
    sceneRect.setBottom(bottomRight.y() + 10);

    isRunning = false;
    foreach (Node *node, myScene->nodes()) {
        if (node->advance())
            isRunning = true;
    }
    setAnimationRunning();

    this->repaint();
}





/****************************
 * GL related QT event handlers (protected)
 ***************************/

void GLGraphWidget::initializeGL()
{
    glaInit();

    // Init camera matrix
    glMatrixMode(GL_MODELVIEW);
    // Warning: Do not set the camera far away when using small
    //     zNear, zFar values. Darkness awaits.
    //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, cameramat);
    glLoadIdentity();
}

void GLGraphWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the camera
    glLoadMatrixf(cameramat);

    // Draw the old example objects
    glaDrawExample();

    // Draw the graph
    drawGraphGL();

    glFlush();
}

void GLGraphWidget::resizeGL(int w, int h)
{
    //GLfloat aspect = (GLfloat)w/(GLfloat)h;

    // Set up the Viewport transformation
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    this->initProjection();
}



/****************************
 * GL graph drawing and projection setup (private)
 ***************************/

void GLGraphWidget::drawGraphGL()
{
    QPointF p;

    // Attention, references abound!
    QVector<Node*> &nodeVector = myScene->nodes();
    QList<Edge*> &edgeList= myScene->edges();

    // Draw edges
    glColor4f(0.0, 0.0, 1.0, 0.5);
    foreach (Edge* edge, edgeList)
    {
        glBegin(GL_LINE_STRIP);
            p = edge->sourceNode()->pos();
            glVertex3f((GLfloat)p.x(), (GLfloat)p.y(), 0.0);
            p = edge->destNode()->pos();
            glVertex3f((GLfloat)p.x(), (GLfloat)p.y(), 0.0);
        glEnd();
    }

    // Draw nodes
    glPointSize(5.0);
    glBegin(GL_POINTS);
        foreach (Node* node, nodeVector)
        {
            //glLoadName(i);    // Load point number into depth buffer for selection
            //glColor4f(n->color, 1.0, 0.3, 0.7);
            glColor4f(0.0, 1.0, 0.3, 0.7);
            p = node->pos();
            glVertex3f((GLfloat)p.x(), (GLfloat)p.y(), 0.0);
        }
    glEnd();
}

void GLGraphWidget::initProjection()
{
  //GLfloat aspect = (GLfloat)width()/(GLfloat)height();

  // Set up the Projection transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0.0, (GLfloat)width()/zoom, (GLfloat)height()/zoom, 0.0);

  // Switch to Model/view transformation for drawing objects
  glMatrixMode(GL_MODELVIEW);
}
