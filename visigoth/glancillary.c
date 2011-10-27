#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "glancillary.h"



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
