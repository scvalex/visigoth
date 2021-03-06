#ifndef WATTSSTROGATZ_H
#define WATTSSTROGATZ_H

#include "algorithm.h"

class GraphScene;
class QWidget;

namespace Ui {
    class WattsControl;
};

class WattsStrogatz : public Algorithm {
   Q_OBJECT
public:
    WattsStrogatz(GraphScene* scene = 0);
    virtual ~WattsStrogatz();

    int getNumNodes() const;
    int getNodeDegree() const;
    double getProbability() const;

    void reset();
    bool canAddVertex();
    void addVertex();
    QWidget* controlWidget(QWidget *parent = 0);

    void updateUI();

private slots:
    void onNodesChanged(int newValue);
    void onProbabilityChanged(double newValue);
    void onDegreeChanged(int newValue);

private:
    static const int START_SIZE = 20;
    static const double START_PROBABILITY = 0.2;
    static const int START_DEGREE = 4;

    GraphScene *scene;
    QWidget *ctlW;
    Ui::WattsControl *wattsCtl;

    int size;
    double probability;
    int degree;
};

#endif // WATTSSTROGATZ_H
