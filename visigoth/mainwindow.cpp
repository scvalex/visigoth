#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef USE_OPENGL
#include <QGLWidget>
#endif

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    view = new QGraphicsView(scene, this);
#ifdef USE_OPENGL
    view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#endif
    setCentralWidget(view);
    qsrand(23);
}

void MainWindow::populate() {
    QPen pen(Qt::black);
    pen.setWidthF(0.0);
    QBrush brush(Qt::SolidPattern);
    for (int i(0); i < 1000; ++i) {
        brush.setColor(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256, 180));
        scene->addEllipse(qrand() % 1000, qrand() % 1000, 10, 10, pen, brush);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
