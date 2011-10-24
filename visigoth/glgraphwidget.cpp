#include "glgraphwidget.h"
#include "glancillary.h"        // gla*()

#include <GL/gl.h>

#include "edge.h"
#include "node.h"


GLGraphWidget::GLGraphWidget(QWidget *parent) :
    QGLWidget(parent)
{
    myScene = new GraphScene(this);
    myScene->setBackgroundBrush(Qt::black);
    myScene->setItemIndexMethod(QGraphicsScene::NoIndex);
}

void GLGraphWidget::populate() {
    myScene->populate();
    myScene->randomizePlacement();
}

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
    GLfloat aspect = (GLfloat)w/(GLfloat)h;

    // Set up the Viewport transformation
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    // Set up the Projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0.0, (GLfloat)w, (GLfloat)h, 0.0);

    // Switch to Model/view transformation for drawing objects
    glMatrixMode(GL_MODELVIEW);
}

void GLGraphWidget::itemMoved() {
    // Nothing.
}



void GLGraphWidget::drawGraphGL() {
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
