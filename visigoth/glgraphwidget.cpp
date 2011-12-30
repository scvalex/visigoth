#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <cmath>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "vtools.h"
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
    myScene(0),
    zoom(1.0),
    mouseMode(MOUSE_IDLE),
    mode3d(false),
    isHighlighted(false),
    animTimerId(0)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

void GLGraphWidget::setScene(GraphScene *newScene) {
    if (myScene != 0)
        return;
    myScene = newScene;
    connect(myScene, SIGNAL(algorithmChanged(Algorithm*)), this, SIGNAL(algorithmChanged(Algorithm*)));
    connect(myScene, SIGNAL(nodeMoved()), this, SLOT(onNodeMoved()));
    setAnimation(true);
}

GLGraphWidget::~GLGraphWidget() {
    delete myScene;
}

bool GLGraphWidget::animationRunning() {
    return (animTimerId != 0);
}

void GLGraphWidget::setAnimation(bool enable) {
    if (enable && !animTimerId) {
        animTimerId = startTimer(1000 / 25);
    } else if (!enable && animTimerId) {
        killTimer(animTimerId);
        animTimerId = 0;
    }
}

void GLGraphWidget::onNodeMoved() {
    setAnimation(true);
}

void GLGraphWidget::scaleView(qreal scaleFactor) {
    zoom *= scaleFactor;
}

void GLGraphWidget::fitToScreen() {
  // FIXME: fitToScreen needs to be completely redone for 3D.
}

void GLGraphWidget::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double)2, event->delta() / 240.0));

    this->repaint();
}

void GLGraphWidget::highlightNeighbours() {
    isHighlighted = true;
}

void GLGraphWidget::notHighlightNeighbours() {
    isHighlighted = false;
    rebornGraph();
}

void GLGraphWidget::rebornGraph() {
    foreach(Edge* edge, myScene->edges()) {
        if (edge->myColour != myScene->myEdgeColor)
            edge->setColour(myScene->myEdgeColor);
    }
    foreach(Node* node, myScene->nodes()) {
        if (node->myColour != myScene->myNodeColor)
            node->setColour(myScene->myNodeColor);
        }
    this->repaint();
}

void GLGraphWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        Node *hitNode = selectGL(event->x(), event->y());
        bool realNode;

        foreach(Node* node, myScene->nodes()) {
            if (hitNode == node) {
                realNode = true;
            }
        }

        if (hitNode && !isHighlighted && realNode) {
            hitNode->unHighlight();
            if (hitNode->colour() != Qt::red)
            //hitNode->setColour(QColor::fromRgbF(1.0, 0.0, 0.0, 1.0));
                hitNode->setColour(Qt::red);
            else
                hitNode->setColour(Qt::blue);
        } else if (hitNode && isHighlighted && realNode){
            hitNode->highlight();
            hitNode->setColour(QColor::fromRgbF(1.0,0.0,0.0,1.0));
            QVector<Node*> neighbs = hitNode->neighbours();
            QList<Edge*> edges = hitNode->edges();
            foreach(Node* node, neighbs) {
                foreach(Edge* edge, edges) {
                    if ((edge->sourceNode() == hitNode && edge->destNode() == node) || (edge->sourceNode() == node && edge->destNode() == hitNode) ) {
                        edge->highlight();
                        if (edge->colour() != Qt::yellow)
                            edge->setColour(Qt::yellow);
                        else
                            edge->setColour(Qt::green);
                    }
                }
            }
        }
    }

    this->repaint();
}

void GLGraphWidget::mousePressEvent(QMouseEvent *event) {
    if (mouseMode != MOUSE_IDLE)
        return;

    if (event->button() == Qt::LeftButton) {
        switch (event->modifiers()) {
            case 0:  // When no modifiers are pressed
            {
                Node *hitNode = selectGL(event->x(), event->y());
                if (hitNode) {
                    draggedNode = hitNode;
                    draggedNode->setAllowAdvance(false);
                    mouseMode = MOUSE_DRAGGING;
                } else {
                    if (mode3d)
                        mouseMode = MOUSE_TRANSLATING_XY;
                    else
                        mouseMode = MOUSE_TRANSLATING_2D;
                }
                break;
            }
            case Qt::ShiftModifier:
                if (mode3d)
                    mouseMode = MOUSE_ROTATING;
                break;
            case Qt::ControlModifier:
                if (mode3d)
                    mouseMode = MOUSE_TRANSLATING_Z;
                break;
        }
    }

    mouseX = event->x();
    mouseY = event->y();
}

void GLGraphWidget::mouseReleaseEvent(QMouseEvent *event) {
    (void) event;

    if (mouseMode == MOUSE_DRAGGING && draggedNode)
      draggedNode->setAllowAdvance(true);

    mouseMode = MOUSE_IDLE;
}

void GLGraphWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx, dy;

    if (mouseMode == MOUSE_IDLE) {
        Node *node = selectGL(event->x(), event->y());
        if (node != 0) {
            emit hoveringOnNode(node);
        }
        return;
    }

    dx = event->x() - mouseX;
    dy = event->y() - mouseY;
    mouseX = event->x();
    mouseY = event->y();

    switch(mouseMode) {
        case MOUSE_TRANSLATING_2D:
            glaCameraTranslatef(cameramat, (GLfloat)dx/zoom, (GLfloat)dy/zoom, 0.0);
            break;
        case MOUSE_TRANSLATING_XY:
            glaCameraTranslatef(cameramat, (3.0) * dx, (-3.0) * dy, 0.0);
            break;
        case MOUSE_TRANSLATING_Z:
            glaCameraRotatef(cameramat, dx, 0.0, 1.0, 0.0);
            glaCameraTranslatef(cameramat, 0.0, 0.0, (-3.0) * dy);
            break;
        case MOUSE_ROTATING:
            glaCameraRotatef(cameramat, dx, 0.0, 1.0, 0.0);
            glaCameraRotatef(cameramat, dy, 1.0, 0.0, 0.0);
            break;
        case MOUSE_DRAGGING:
            GLdouble newX, newY, newZ;
            GLdouble model[16], proj[16];

            glPushMatrix();
              glLoadMatrixf(cameramat);
              glGetDoublev(GL_MODELVIEW_MATRIX, model);
              glLoadMatrixf(projmat);
              glGetDoublev(GL_PROJECTION_MATRIX, proj);
            glPopMatrix();

            gluUnProject((GLdouble)mouseX, (GLdouble)(viewmat[3] - mouseY), 0.0,
                        model, proj, viewmat,
                        &newX, &newY, &newZ);

            draggedNode->setPos(VPointF(newX, newY, 0.0));
            break;
        default:
            break;
    }

    this->repaint();
}

void GLGraphWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_G:
        myScene->repopulate();
        break;
    case Qt::Key_Escape:
        //helping = false;
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
        setAnimation(!animationRunning());
        break;
    case Qt::Key_0:
        fitToScreen();
        break;
    case Qt::Key_A:
    case Qt::Key_N:
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
    default:
        QGLWidget::keyPressEvent(event);
    }

    this->repaint();
}

void GLGraphWidget::timerEvent(QTimerEvent *) {
    bool somethingMoved = myScene->calculateForces();

    if (!somethingMoved) {
        // setAnimation(true) would recreate the timer though it is
        // already running (this is a timer event). So don't do it.
        setAnimation(false);
    }

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

    // Reinit the projection for drawing the graph
    this->initProjection();

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

    // Save the viewport, e.g. for mouse interaction
    glGetIntegerv(GL_VIEWPORT, viewmat);
}



/****************************
 * GL graph drawing and projection setup (private)
 ***************************/

inline void GLGraphWidget::drawNode(Node *node) {
    QColor c = node->colour();
    glColor4f(c.redF(), c.greenF(), c.blueF(), c.alphaF());

    float radius = (log(node->edges().size()) / log(2)) + 1.0;
    VPointF p = node->pos();

    // FIXME: Draw spheres instead of flat circles.
    glBegin(GL_TRIANGLE_FAN);
    // glBegin(GL_LINE_LOOP);
        // int step = 180 / (radius > 50 ? 50 : (int) radius);
        int step = 30;
        for (int angle(0); angle < 360; angle += step) {
            GLfloat rangle = (GLfloat) angle * (3.1415926 / 180.0);
            glVertex3f((GLfloat)p.x + sin(rangle) * radius,
                       (GLfloat)p.y + cos(rangle) * radius,
                       //(GLfloat)p.z + cos(rangle) * radius);
                       0.0);
        }
    glEnd();
}

void GLGraphWidget::drawGraphGL() {
    // Draw edges
    foreach (Edge* edge, myScene->edges()) {
        const QColor c = edge->colour();
        glColor4f(c.redF(), c.greenF(), c.blueF(), c.alphaF());

        glBegin(GL_LINE_STRIP);
            VPointF p = edge->sourceNode()->pos();
            glVertex3f((GLfloat)p.x, (GLfloat)p.y, (GLfloat)p.z);
            p = edge->destNode()->pos();
            glVertex3f((GLfloat)p.x, (GLfloat)p.y, (GLfloat)p.z);
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

    // Zooming, only in 2D
    if (!mode3d)
        glScalef(zoom, zoom, 1.0/zoom);

    if (mode3d)
        // Persective projection
        gluPerspective(90, (GLfloat)width()/(GLfloat)height(), 0.0001, 100000.0);
    else
        // Flat projection
        gluOrtho2D((GLfloat)width() / -2, (GLfloat)width() / 2, (GLfloat)height() / 2, (GLfloat)height() / -2);

    // Save the projection matrix for later use, e.g. mouse interaction
    glGetFloatv(GL_PROJECTION_MATRIX, projmat);

    // Switch to Model/view transformation for drawing objects
    glMatrixMode(GL_MODELVIEW);
}

Node* GLGraphWidget::selectGL(int x, int y)
{
    Node* hitNode = NULL;

    GLuint namebuf[64] = {0};
    GLint hits;


    // Account for inverse Y coordinate
    y = viewmat[3] - y;

    glSelectBuffer(64, namebuf);

    // Restrict projection matrix to selection area
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluPickMatrix(x, y, 1.0, 1.0, viewmat);
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
