#include "algorithm.h"
#include "graphscene.h"
#include "glgraphwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_helpWidget.h"
#include "ui_statistics.h"
#include "preferential.h"
#include "graphscene.h"
#include "bipartite.h"
#include "statistics.h"

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
    algoCtl(0),
    helpWidget(0),
    focusedNode(0)
{
    qsrand(23);

    QCoreApplication::setOrganizationName("Visigoth");
    QCoreApplication::setApplicationName("Visigoth");

    ui->setupUi(this);

    connect(ui->exportToAct, SIGNAL(triggered()), this, SLOT(exportTo()));
    connect(ui->helpAct, SIGNAL(toggled(bool)), this, SLOT(toggleHelp(bool)));

    view = new GLGraphWidget(this);
    scene = new GraphScene();
    connect(scene, SIGNAL(itemMovedSignal()), view, SLOT(itemMoved()));
    view->setScene(scene);

    statsUi->setupUi(ui->statsWidget);

    setCentralWidget(view);

    connect(ui->newNodeAct, SIGNAL(triggered()), scene, SLOT(addVertex()));
    connect(ui->randomizeAct, SIGNAL(triggered()), scene, SLOT(randomizePlacement()));
    connect(ui->generateAct, SIGNAL(triggered()), scene, SLOT(repopulate()));
    connect(scene, SIGNAL(repopulated()), this, SLOT(onGenerate()));
    connect(view, SIGNAL(algorithmChanged(Algorithm*)), this, SLOT(onAlgorithmChanged(Algorithm*)));
    connect(view, SIGNAL(hoveringOnNode(Node*)), this, SLOT(onFocusedNodeChanged(Node*)));

    ui->chooserCombo->addItems(scene->algorithms());
    connect(ui->chooserCombo, SIGNAL(currentIndexChanged(const QString &)), scene, SLOT(chooseAlgorithm(const QString &)));

    scene->chooseAlgorithm(ui->chooserCombo->currentText());

    scene->repopulate();
}

MainWindow::~MainWindow() {
    delete view;
    delete ui;
}

void MainWindow::toggleHelp(bool enabled) {
    if (!helpWidget) {
        helpDock = new QDockWidget(this);
        helpWidget = new QWidget(this);
        Ui::helpWidget *helpWid = new Ui::helpWidget();
        helpWid->setupUi(helpWidget);
        helpWid->text->setText("blah");
        helpWid->text->isReadOnly();
        helpWid->text->setFrameShape(QFrame::NoFrame);
        helpWid->text->viewport()->setAutoFillBackground(false);
        helpDock->setWidget(helpWidget);
        helpDock->setWindowTitle("Help Manual");
        helpDock->setFixedWidth((this->width())/3);
        helpDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
        addDockWidget(Qt::LeftDockWidgetArea, helpDock);
    } else {
        helpDock->setVisible(enabled);
    }

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

void MainWindow::onGenerate() {
    focusedNode = 0;
    Statistics *stats = scene->getStatistics();
    statsUi->lengthLabel->setText(QString::number(stats->lengthAvg()));
    statsUi->degreeLabel->setText(QString::number(stats->degreeAvg()));
    statsUi->clusteringLabel->setText(QString::number(stats->clusteringAvg()));
    statsUi->coeffLabel->setText("");
}

void MainWindow::onFocusedNodeChanged(Node *node) {
    if (node == focusedNode)
        return;
    focusedNode = node;

    Statistics *stats = scene->getStatistics();
    statsUi->coeffLabel->setText(QString::number(stats->clusteringCoeff(focusedNode)));
}
