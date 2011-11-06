#include "algorithm.h"
#include "glgraphwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
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
    algoCtl(0)
{
    ui->setupUi(this);

    view = new GLGraphWidget(this);

    connect(ui->exportToAct, SIGNAL(triggered()), this, SLOT(exportTo()));
    connect(ui->toggleControlAct, SIGNAL(toggled(bool)), this, SLOT(toggleShowControl(bool)));
    connect(ui->newNodeAct, SIGNAL(triggered()), this, SLOT(addNewNode()));
    connect(ui->randomizeAct, SIGNAL(triggered()), view, SLOT(randomizePlacement()));
    connect(ui->generateAct, SIGNAL(triggered()), view, SLOT(populate()));

    setCentralWidget(view);

    qsrand(23);

    myComboBox = new QComboBox(this);
    QStringList text;
    text.append("Bipartite Model");
    text.append("Preferential Attachment");
    myComboBox->addItems(text);

    QDockWidget *dock = new QDockWidget(this);
    dock->setWidget(myComboBox);
    dock->setWindowTitle("Algorithm Chooser");
    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(view, SIGNAL(algorithmChanged(Algorithm*)), this, SLOT(onAlgorithmChanged(Algorithm*)));
    view->init();

    connect(myComboBox, SIGNAL(currentIndexChanged(const QString &)) , this, SLOT(onComboBoxActivated(const QString &)));

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

void MainWindow::addNewNode() {
    //view->scene()->addVertex();
    if (myComboBox->currentText() == "Bipartite Model") {
        QMessageBox msgBox;
        msgBox.setText("Bipartite model does not support adding new vertices");
        msgBox.exec();
    } else if (myComboBox->currentText() == "Preferential Attachment") {
        algo->addVertex();
    }

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


void MainWindow::onComboBoxActivated(const QString &text)
{
    if (text == "Bipartite Model") {
        view->scene()->reset();
        algo = new Bipartite(view->scene());
        algo->reset();
        view->randomizePlacement();
        onAlgorithmChanged(algo);
    } else if (text == "Preferential Attachment") {
        view->scene()->reset();
        algo = new Preferential(view->scene());
        algo->reset();
        view->randomizePlacement();
        onAlgorithmChanged(algo);

    }
}
