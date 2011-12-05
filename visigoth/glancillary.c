#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "glancillary.h"
#include "math.h"



/****************************
 * GL init
 ***************************/

void glaInit(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    //glShadeModel(GL_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Init Model/view transformation
    glMatrixMode(GL_MODELVIEW);

    // Reset MV matrix
    glLoadIdentity();

    // Configure glColor()
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

/****************************
 * GL camera simulation helpers
 ***************************/

void glaCameraRotatef(GLfloat *cmat, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    glPushMatrix();
        glLoadIdentity();
        glRotatef(angle, x, y, z);
        glMultMatrixf(cmat);
        glGetFloatv(GL_MODELVIEW_MATRIX, cmat);
    glPopMatrix();
}

void glaCameraTranslatef(GLfloat *cmat, GLfloat x, GLfloat y, GLfloat z)
{
    glPushMatrix();
        glLoadIdentity();
        glTranslatef(x, y, z);
        glMultMatrixf(cmat);
        glGetFloatv(GL_MODELVIEW_MATRIX, cmat);
    glPopMatrix();
}




/****************************
 * GL box drawing example
 ***************************/

void glaDrawExample(void)
{
    // Draw the corner dots
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f(1.0, -1.0, -1.0);
        glVertex3f(1.0, 1.0, -1.0);
        glVertex3f(-1.0, 1.0, -1.0);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f(-1.0, -1.0, 1.0);
        glVertex3f(-1.0, 1.0, 1.0);
    glEnd();

    // Draw a line with fixed vertices
    glColor3f(0.5, 1.0, 0.5);
    glBegin(GL_LINE_STRIP);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
        glVertex3f(0.5, 0.5, 0.5);
    glEnd();

    // Draw the box boundaries
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f(1.0, -1.0, -1.0);
        glVertex3f(1.0, 1.0, -1.0);
        glVertex3f(-1.0, 1.0, -1.0);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f(-1.0, -1.0, 1.0);
        glVertex3f(-1.0, 1.0, 1.0);
        glVertex3f(-1.0, 1.0, -1.0);
    glEnd();
}


void glaDrawSphere(GLfloat r, int lats, int longs) {
    int i, j;

    for (i = 0; i <= lats; i++) {
        GLfloat lat0 = M_PI * (-0.5 + (GLfloat) (i - 1) / lats);
        GLfloat z0  = sin(lat0);
        GLfloat zr0 =  cos(lat0);

        GLfloat lat1 = M_PI * (-0.5 + (GLfloat) i / lats);
        GLfloat z1 = sin(lat1);
        GLfloat zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
            for(j = 0; j <= longs; j++) {
                GLfloat lng = 2 * M_PI * (GLfloat) (j - 1) / longs;
                GLfloat x = cos(lng);
                GLfloat y = sin(lng);

                glNormal3f(x * zr0, y * zr0, z0);
                glVertex3f(x * zr0 * r, y * zr0 * r, z0 * r);
                glNormal3f(x * zr1, y * zr1, z1);
                glVertex3f(x * zr1 * r, y * zr1 * r, z1 * r);
            }
        glEnd();
    }
}
