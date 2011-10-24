#include "glgraphwidget.h"
#include "glancillary.h"        // gla*()

#include <GL/gl.h>


GLGraphWidget::GLGraphWidget(QWidget *parent) :
    QGLWidget(parent)
{
    //myScene = new GraphScene(this);
    //myScene->setBackgroundBrush(Qt::black);
    //myScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //setScene(myScene);
}

void GLGraphWidget::populate() {
}

void GLGraphWidget::initializeGL() {
    glaInit();

    // Init camera matrix
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
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

    // Set up the Projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (GLfloat)w/(GLfloat)h, 0.0, 1000.0);

    // Switch to Model/view transformation for drawing objects
    glMatrixMode(GL_MODELVIEW);
}




void GLGraphWidget::drawGraphGL() {
    int i;

    // Draw edges
    glColor4f(0.0, 0.0, 1.0, 0.5);
    //foreach (Edge* edge, edgeVector)
    {
        glBegin(GL_LINE_STRIP);
            //glVertex3f((GLfloat)n1->x, (GLfloat)n1->y, 0.0);
            //glVertex3f((GLfloat)n2->x, (GLfloat)n2->y, 0.0);
        glEnd();
    }

    // Draw nodes
    glPointSize(5.0);
    //glBegin(GL_POINTS);
        //foreach (Node* node, nodeVector)
        {
            //grNode *n = &g->nodes[i];
            //glLoadName(i);    // Load point number into depth buffer for selection
            //glColor4f(n->color, 1.0, 0.3, 0.7);
            //glVertex3f((GLfloat)n->x, (GLfloat)n->y, 0.0);
        }
    glEnd();
}
