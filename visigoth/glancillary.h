#ifndef _GLANCILLARY_H_
#define _GLANCILLARY_H_

#ifdef __cplusplus
extern "C" {
#endif

void glaInit(void);
void glaCameraRotatef(GLfloat *cmat, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void glaCameraTranslatef(GLfloat *cmat, GLfloat x, GLfloat y, GLfloat z);
void glaDrawExample(void);

#ifdef __cplusplus
}
#endif

#endif
