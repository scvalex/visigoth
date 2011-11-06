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

public slots:
    void exportTo();
private slots:
    void onAlgorithmChanged(Algorithm *newAlgo);
private:
    Ui::MainWindow *ui;
    GLGraphWidget *view;
    QDockWidget *algoCtl;
};

#endif // MAINWINDOW_H
