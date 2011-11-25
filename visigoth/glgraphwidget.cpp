#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <cmath>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "glgraphwidget.h"
#include "glancillary.h"        // gla*()
#include "edge.h"
#include "node.h"
#include "quadtree.h"


/****************************
 * GraphWidget imitation code (public)
 ***************************/

GLGraphWidget::GLGraphWidget(QWidget *parent) :
    QGLWidget(parent),
    zoom(1.0),
    mouseMode(MOUSE_IDLE),
    helping(true),
    isPlaying(true),
    timerId(0)
{
    setFocusPolicy(Qt::StrongFocus);
    myScene = new GraphScene(this);
    myScene->setBackgroundBrush(Qt::black);
    myScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    connect(myScene, SIGNAL(algorithmChanged(Algorithm*)), this, SIGNAL(algorithmChanged(Algorithm*)));
}

GLGraphWidget::~GLGraphWidget() {
    delete myScene;
}

QList<QString> GLGraphWidget::algorithms() const {
    return myScene->algorithms();
}

void GLGraphWidget::populate() {
    myScene->repopulate();
}

void GLGraphWidget::itemMoved() {
    setAnimationRunning();
}

void GLGraphWidget::randomizePlacement() {
    myScene->randomizePlacement();
}

void GLGraphWidget::addVertex() {
    myScene->addVertex();
}

void GLGraphWidget::chooseAlgorithm(const QString &name) {
    myScene->chooseAlgorithm(name);
}

/****************************
 * GraphWidget imitation code (protected)
 ***************************/

void GLGraphWidget::setAnimationRunning() {
    if (isPlaying && myScene->isRunning() && !timerId)
        timerId = startTimer(1000 / 25);
    else if ((!isPlaying || !myScene->isRunning()) && timerId) {
        killTimer(timerId);
        timerId = 0;
    }
}

void GLGraphWidget::playPause() {
    isPlaying = !isPlaying;
    setAnimationRunning();

    this->repaint();
}

void GLGraphWidget::scaleView(qreal scaleFactor) {
    zoom *= scaleFactor;
    this->initProjection();
}

void GLGraphWidget::fitToScreen() {
    float aspectWidget = (float)width()/(float)height();
    float aspectGraph = (float)myScene->width()/(float)myScene->height();

    if (aspectGraph >= aspectWidget)
        scaleView((GLfloat)width() / (GLfloat)myScene->width() / zoom);
    else
        scaleView((GLfloat)height() / (GLfloat)myScene->height() / zoom);
}

void GLGraphWidget::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double)2, event->delta() / 240.0));

    this->repaint();
}

void GLGraphWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        Node *hitNode = selectGL(event->x(), event->y());

        if (hitNode) {
            hitNode->setBrush(QColor::fromRgbF(1.0, 0.0, 0.0, 1.0));
        }
    }

    this->repaint();
}

void GLGraphWidget::mousePressEvent(QMouseEvent *event) {
    if (mouseMode != MOUSE_IDLE)
        return;

    if (event->button() == Qt::LeftButton) {
        switch (0) { // FIXME: Should get modifier key status here
            case 0:  // When no modifiers are pressed
                mouseMode = MOUSE_TRANSLATING;
                break;
            //case GLUT_ACTIVE_SHIFT:
                mouseMode = MOUSE_ROTATING;
                break;
            //case GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL:
                mouseMode = MOUSE_TRANSLATING;
                break;
            //case GLUT_ACTIVE_CTRL:
                mouseMode = MOUSE_TRANSLATING2;
                break;
        }
    } else if (event->button() == Qt::RightButton) {
        Node *hitNode = selectGL(event->x(), event->y());

        if (hitNode) {
            draggedNode = hitNode;
            mouseMode = MOUSE_DRAGGING;
        }
    }

    mouseX = event->x();
    mouseY = event->y();
}

void GLGraphWidget::mouseReleaseEvent(QMouseEvent *event) {
    (void) event;

    mouseMode = MOUSE_IDLE;
}

void GLGraphWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx, dy;

    if (mouseMode == MOUSE_IDLE)
        return;

    dx = event->x() - mouseX;
    dy = event->y() - mouseY;
    mouseX = event->x();
    mouseY = event->y();

    switch(mouseMode) {
        case MOUSE_TRANSLATING:
            //glaCameraTranslatef(cameramat, (0.1) * dx, (-0.1) * dy, 0.0);
            // Modified for 2D projection and zoom
            glaCameraTranslatef(cameramat, (GLfloat)dx/zoom, (GLfloat)dy/zoom, 0.0);
            break;
        case MOUSE_TRANSLATING2:
            //glaCameraRotatef(cameramat, dx, 0.0, 1.0, 0.0);
            //glaCameraTranslatef(cameramat, 0.0, 0.0, (-0.1) * dy);
            break;
        case MOUSE_ROTATING:
            //glaCameraRotatef(cameramat, dx, 0.0, 1.0, 0.0);
            //glaCameraRotatef(cameramat, dy, 1.0, 0.0, 0.0);
            break;
        case MOUSE_DRAGGING:
        case MOUSE_IDLE:
            break;
    }

    this->repaint();
}

void GLGraphWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_H:
        helping = !helping;
        this->repaint();
        break;
    case Qt::Key_G:
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
        randomizePlacement();
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
        glaCameraTranslatef(cameramat, (-20.0)/zoom, 0.0, 0.0);
        break;
    case Qt::Key_Up:
        glaCameraTranslatef(cameramat, 0.0, (-20.0)/zoom, 0.0);
        break;
    case Qt::Key_Right:
        glaCameraTranslatef(cameramat, 20.0/zoom, 0.0, 0.0);
        break;
    case Qt::Key_Down:
        glaCameraTranslatef(cameramat, 0.0, 20.0/zoom, 0.0);
        break;
    case Qt::Key_S:
        myScene->calculateMetrics();
        break;
    default:
        QGLWidget::keyPressEvent(event);
    }

    this->repaint();
}

void GLGraphWidget::timerEvent(QTimerEvent *) {
    myScene->calculateForces();

    setAnimationRunning();

    this->repaint();
}





/****************************
 * GL related QT event handlers (protected)
 ***************************/

void GLGraphWidget::initializeGL() {
    glaInit();

    // Init camera matrix
    glMatrixMode(GL_MODELVIEW);
    // Warning: Do not set the camera far away when using small
    //     zNear, zFar values. Darkness awaits.
    //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, cameramat);
    glLoadIdentity();
}

void GLGraphWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the camera
    glLoadMatrixf(cameramat);

    // Draw the old example objects
    glaDrawExample();

    // Draw the graph
    drawGraphGL();

    glFlush();
}

void GLGraphWidget::resizeGL(int w, int h) {
    // Set up the Viewport transformation
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    this->initProjection();
}



/****************************
 * GL graph drawing and projection setup (private)
 ***************************/

inline void GLGraphWidget::drawNode(Node* node) {
    QColor c = node->brush().color();
    glColor4f(c.redF(), c.greenF(), c.blueF(), c.alphaF());

    float radius = (log(node->edges().size()) / log(2)) + 1.0;
    QPointF p = node->pos();

    glBegin(GL_TRIANGLE_FAN);
    // glBegin(GL_LINE_LOOP);
        // int step = 180 / (radius > 50 ? 50 : (int) radius);
        int step = 30;
        for (int angle(0); angle < 360; angle += step) {
            GLfloat rangle = (GLfloat) angle * (3.1415926 / 180.0);
            glVertex3f((GLfloat)p.x() + sin(rangle) * radius,
                       (GLfloat)p.y() + cos(rangle) * radius,
                       0.0);
        }
    glEnd();
}

void GLGraphWidget::drawGraphGL() {
    // If there is a dragged node, draw it where the mouse is.
    if (mouseMode == MOUSE_DRAGGING) {
        GLdouble model[16], proj[16];
        GLint view[4];
        GLdouble newX, newY, newZ;

        glGetDoublev(GL_MODELVIEW_MATRIX, model);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetIntegerv(GL_VIEWPORT, view);
        gluUnProject((GLdouble)mouseX, (GLdouble)(view[3] - mouseY), 0.0,
                    model, proj, view,
                    &newX, &newY, &newZ);

        draggedNode->setPos(newX, newY);
    }

    // Draw edges
    foreach (Edge* edge, myScene->edges()) {
        const QColor c = edge->colour();
        glColor4f(c.redF(), c.greenF(), c.blueF(), c.alphaF());
        //glColor4f(0.0, 0.0, 1.0, 0.5);
        glBegin(GL_LINE_STRIP);
            QPointF p = edge->sourceNode()->pos();
            glVertex3f((GLfloat)p.x(), (GLfloat)p.y(), 0.0);
            p = edge->destNode()->pos();
            glVertex3f((GLfloat)p.x(), (GLfloat)p.y(), 0.0);
        glEnd();
    }

    // Draw nodes
    foreach (Node* node, myScene->nodes()) {
        drawNode(node);
    }
}

void GLGraphWidget::initProjection() {
    // Set up the Projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Zooming
    glScalef(zoom, zoom, 1.0/zoom);

    // Flat projection
    gluOrtho2D(0.0, (GLfloat)width(), (GLfloat)height(), 0.0);

    // Switch to Model/view transformation for drawing objects
    glMatrixMode(GL_MODELVIEW);
}

Node* GLGraphWidget::selectGL(int x, int y)
{
    Node* hitNode = NULL;

    GLuint namebuf[64] = {0};
    GLint hits;
    GLint view[4];
    GLfloat projmat[16];


    // Account for inverse Y coordinate
    glGetIntegerv(GL_VIEWPORT, view);
    y = view[3] - y;

    glSelectBuffer(64, namebuf);

    // Restrict projection matrix to selection area
    glGetFloatv(GL_PROJECTION_MATRIX, projmat);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluPickMatrix(x, y, 1.0, 1.0, view);
        glMultMatrixf(projmat);

        // Redraw points to fill selection buffer
        glMatrixMode(GL_MODELVIEW);

        QVector<Node*> &nodes = myScene->nodes();
        for (int i = nodes.size() - 1; i >= 0; --i) {
            glSelectBuffer(64, namebuf);
            glRenderMode(GL_SELECT);

            // Reset name stack
            glInitNames();
            glPushName(0);

            // Draw the node
            drawNode(nodes[i]);

            hits = glRenderMode(GL_RENDER);

            if (hits) {
                hitNode = nodes[i];
                break;
            }
        }

  // Reset projection
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  this->repaint();

  return hitNode;
}
