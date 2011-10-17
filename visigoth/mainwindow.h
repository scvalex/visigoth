#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class GraphWidget;
class QGraphicsView;

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
    GraphWidget *view;
};

#endif // MAINWINDOW_H
