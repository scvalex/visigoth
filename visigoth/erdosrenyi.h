#ifndef ERDOSRENYI_H
#define ERDOSRENYI_H

#include "algorithm.h"

class GraphScene;
class QWidget;

namespace Ui {
    class ErdosControl;
};

class ErdosRenyi : public Algorithm {
    Q_OBJECT
public:
    ErdosRenyi(GraphScene *scene = 0);
    virtual ~ErdosRenyi();

    int getNumNodes() const;
    double getProbability() const;

    void reset();
    bool canAddVertex();
    void addVertex();
    void addVertex(bool saveSize);
    QWidget* controlWidget(QWidget *parent = 0);

private slots:
    void onNodesChanged(int newValue);
    void onProbabilityChanged(double newValue);

private:
    static const int START_SIZE = 60;
    static const double START_PROBABILITY = 0.1;

    GraphScene *scene;
    QWidget *ctlW;
    Ui::ErdosControl *erCtl;

    int size;
    double probability;

    void updateUI();
};

#endif // ERDOSRENYI_H
