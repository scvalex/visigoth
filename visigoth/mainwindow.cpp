#include "algorithm.h"
#include "graphscene.h"
#include "glgraphwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_statistics.h"
#include "preferential.h"
#include "graphscene.h"
#include "bipartite.h"

#include <QDesktopWidget>
#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QPrinter>
#include <QComboBox>
#include <QStringList>
#include <QObject>
#include <QAction>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    statsUi(new Ui::Statistics),
    algoCtl(0)
{
    qsrand(23);

    ui->setupUi(this);

    connect(ui->exportToAct, SIGNAL(triggered()), this, SLOT(exportTo()));

    view = new GLGraphWidget(this);
    scene = new GraphScene(view);
    view->setScene(scene);

    statsUi->setupUi(ui->statsWidget);

    setCentralWidget(view);

    connect(ui->newNodeAct, SIGNAL(triggered()), scene, SLOT(addVertex()));
    connect(ui->randomizeAct, SIGNAL(triggered()), scene, SLOT(randomizePlacement()));
    connect(ui->generateAct, SIGNAL(triggered()), scene, SLOT(repopulate()));
    connect(view, SIGNAL(algorithmChanged(Algorithm*)), this, SLOT(onAlgorithmChanged(Algorithm*)));

    ui->chooserCombo->addItems(scene->algorithms());
    connect(ui->chooserCombo, SIGNAL(currentIndexChanged(const QString &)), scene, SLOT(chooseAlgorithm(const QString &)));

    scene->chooseAlgorithm(ui->chooserCombo->currentText());

    scene->repopulate();
}

MainWindow::~MainWindow() {
    delete ui;
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

    ui->newNodeAct->setEnabled(newAlgo->canAddVertex());
}
