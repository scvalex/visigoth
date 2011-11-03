#include "algorithm.h"
#include "glgraphwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QPrinter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    controlVisible(true),
    ui(new Ui::MainWindow),
    algoCtl(0)
{
    ui->setupUi(this);
    connect(ui->exportToAct, SIGNAL(triggered()), this, SLOT(exportTo()));
    connect(ui->toggleControlAct, SIGNAL(toggled(bool)), this, SLOT(toggleShowControl(bool)));

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

void MainWindow::toggleShowControl(bool enabled) {
    if (controlVisible == enabled)
        return;

    if (!controlVisible) {
        addDockWidget(Qt::RightDockWidgetArea, algoCtl);
    } else {
        removeDockWidget(algoCtl);
    }

    controlVisible = enabled;
}

void MainWindow::onAlgorithmChanged(Algorithm *newAlgo) {
    QWidget *ctl = newAlgo->controlWidget(this);
    if (algoCtl) {
        if (controlVisible)
            removeDockWidget(algoCtl);
        delete algoCtl->widget();
        delete algoCtl;
        algoCtl = 0;
    }
    if (ctl) {
        QDockWidget *dock = new QDockWidget(this);
        dock->setWidget(ctl);
        algoCtl = dock;
        if (controlVisible)
            addDockWidget(Qt::RightDockWidgetArea, algoCtl);
    }
}
