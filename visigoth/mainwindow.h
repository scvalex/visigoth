#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphwidget.h"
#include "glgraphwidget.h"

#include <QMainWindow>
#include <QGraphicsView>

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

private slots:
    void on_actionPrint_to_PDF_triggered();

private:
    Ui::MainWindow *ui;
    //GraphWidget *view;
    GLGraphWidget *view;
};

#endif // MAINWINDOW_H
