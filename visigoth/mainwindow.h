#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glgraphwidget.h"

#include <QComboBox>
#include <QGraphicsView>
#include <QMainWindow>

class Algorithm;
class GraphScene;
class Node;
class QDockWidget;

namespace Ui {
    class MainWindow;
    class Statistics;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //functions for testing
    Ui::MainWindow* getUi();
    Ui::Statistics* getStatsUi();
    QDockWidget* getAlgoCtl();
    QWidget* getHelpWidget();
    QDockWidget* getHelpDock();
    GraphScene* getScene();
    GLGraphWidget* getGlWidget();

public slots:
    void exportTo();
    void toggleHelp(bool enabled);

private slots:
    void onAlgorithmChanged(Algorithm *newAlgo);
    void onGenerate();
    void onFocusedNodeChanged(Node *node);

private:
    Ui::MainWindow *ui;
    Ui::Statistics *statsUi;
    GLGraphWidget *view;
    GraphScene *scene;
    QDockWidget *algoCtl;
    QWidget *helpWidget;
    QDockWidget *helpDock;
    Node *focusedNode;
};

#endif // MAINWINDOW_H
