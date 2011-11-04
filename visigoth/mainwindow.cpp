#include "algorithm.h"
#include "glgraphwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferential.h"
#include "graphscene.h"

#include <QDesktopWidget>
#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QPrinter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    algoCtl(0)
{
    ui->setupUi(this);


    connect(ui->exportToAct, SIGNAL(triggered()), this, SLOT(exportTo()));
    connect(ui->toggleControlAct, SIGNAL(toggled(bool)), this, SLOT(toggleShowControl(bool)));
    connect(ui->newNodeAct, SIGNAL(triggered()), this, SLOT(addNewNode()));
    connect(ui->randomizeAct, SIGNAL(triggered()), this, SLOT(randomizeGraph()));
    connect(ui->generateAct, SIGNAL(triggered()), this, SLOT(generateNewGraph()));
    connect(ui->ChangeAlgorithmAct, SIGNAL(triggered()), this, SLOT(changeAlgorithm()));

    view = new GLGraphWidget(this);
    setCentralWidget(view);
    qsrand(23);

    connect(view, SIGNAL(algorithmChanged(Algorithm*)), this, SLOT(onAlgorithmChanged(Algorithm*)));
    view->init();
}

void MainWindow::populate() {
    view->populate();
}

MainWindow::~MainWindow() {
    delete ui;
    delete view;
}

void MainWindow::exportTo() {
    //first commented version took a screenshot of the whole screen, compared to
    //the 2nd version which now takes only the Widget;

    //QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());

    int vWidth = view->width()-15;

    QPixmap pixmap = QPixmap::grabWidget(view, 0, 0, vWidth, -1);

    QString format = "png";
    QString initialPath = QDir::currentPath() + "/untitled." + format;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                    initialPath,
                                    tr("PNG (*.png);;JPG (*.jpg);;All Files (*)"));
    //tr("%1 Files (*.%2);;All Files (*);;JPG (*.jpg)")
    //.arg(format.toUpper())
    //.arg(format));
    if (!fileName.isEmpty()) {
        pixmap.save(fileName, format.toAscii());
    }
}

void MainWindow::generateNewGraph() {
    populate();
}

void MainWindow::changeAlgorithm() {
    view->scene()->nextAlgorithm();
    view->scene()->randomizePlacement();
    onAlgorithmChanged(view->scene()->algorithm());
}

void MainWindow::addNewNode() {
    view->scene()->addVertex();
}

void MainWindow::randomizeGraph() {
    view->scene()->randomizePlacement();
}

void MainWindow::toggleShowControl(bool enabled) {
    if (algoCtl)
        algoCtl->setVisible(enabled);
}

void MainWindow::onAlgorithmChanged(Algorithm *newAlgo) {
    QWidget *ctl = newAlgo->controlWidget(this);
    if (algoCtl) {
        removeDockWidget(algoCtl);
        delete algoCtl->widget();
        delete algoCtl;
        algoCtl = 0;
    }
    if (ctl) {
        QDockWidget *dock = new QDockWidget(this);
        dock->setWidget(ctl);
        dock->setWindowTitle("Algorithm Control");
        algoCtl = dock;
        addDockWidget(Qt::RightDockWidgetArea, algoCtl);
    }
}

