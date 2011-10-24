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

protected slots:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    GraphScene *myScene;

    GLfloat cameramat[16];
    void drawGraphGL();
};

#endif // GLGRAPHWIDGET_H
