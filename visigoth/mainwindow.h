#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glgraphwidget.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QComboBox>

class Algorithm;
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

   // QMainWindow* controlWindow(QMainWindow* parent = 0);

public slots:
    void exportTo();
  //  void openHelpManual();
    void controlWindow();

private slots:
    void onAlgorithmChanged(Algorithm *newAlgo);

private:

    GLGraphWidget *view;
    QDockWidget *algoCtl;
    //QMainWindow *helpWindow;
    QWidget *helpWidget;
    QDockWidget *helpDock;
};

#endif // MAINWINDOW_H
