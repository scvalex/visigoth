#include "graphwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QPrinter>

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
    delete view;
}

void MainWindow::on_actionPrint_to_PDF_triggered()
{
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
