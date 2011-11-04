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

    void populate();

public slots:
    void exportTo();
    void toggleShowControl(bool enabled);

private slots:
    void onAlgorithmChanged(Algorithm *newAlgo);
    void addNewNode();
    void randomizeGraph();
    void generateNewGraph();

    void onComboBoxActivated(const QString &text);

private:
    Ui::MainWindow *ui;
    GLGraphWidget *view;
    QDockWidget *algoCtl;
    Algorithm *algo;
    QComboBox *myComboBox;
};

#endif // MAINWINDOW_H
