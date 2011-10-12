#include "mainwindow.h"
#include "node.h"
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
    for (int i(0); i < 1000; ++i) {
        Node *node = new Node();
        node->setPos(10 + qrand() % 1000, 10 + qrand() % 1000);
        scene->addItem(node);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
