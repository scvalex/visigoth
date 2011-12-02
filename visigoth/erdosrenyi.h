#ifndef ERDOSRENYI_H
#define ERDOSRENYI_H

#include "algorithm.h"
#include "ui_erdoscontrol.h"

class GraphScene;
class QWidget;

class ErdosRenyi : public Algorithm {
    Q_OBJECT
public:
    ErdosRenyi(GraphScene *scene = 0);
    virtual ~ErdosRenyi();

    void reset();
    bool canAddVertex();
    void addVertex();
    QWidget* controlWidget(QWidget *parent = 0);
    Ui::ErdosControl* getUi();

private slots:
    void onNodesChanged(int newValue);
    void onProbabilityChanged(double newValue);

private:
    static const int START_SIZE = 60;
    static const double START_PROBABILITY = 0.1;

    GraphScene *scene;
    QWidget *ctlW;
    Ui::ErdosControl* erCtrl;

    int size;
    double probability;
};

#endif // ERDOSRENYI_H
