#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glgraphwidget.h"

#include <QComboBox>
#include <QGraphicsView>
#include <QMainWindow>

class Algorithm;
class GraphScene;
class QDockWidget;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;

public slots:
    void exportTo();

private slots:
    void onAlgorithmChanged(Algorithm *newAlgo);

private:
    GLGraphWidget *view;
    GraphScene *scene;
    QDockWidget *algoCtl;
};

#endif // MAINWINDOW_H
