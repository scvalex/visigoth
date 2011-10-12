#include "graphwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef USE_OPENGL
#include <QGLWidget>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view = new GraphWidget(this);
#ifdef USE_OPENGL
    view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#endif
    setCentralWidget(view);
    qsrand(23);
}

void MainWindow::populate() {
    view->populate();
}

MainWindow::~MainWindow()
{
    delete ui;
}
