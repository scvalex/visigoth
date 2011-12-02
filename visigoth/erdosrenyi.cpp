#include "graphscene.h"
#include "erdosrenyi.h"

#include <QtCore/qmath.h>
#include <cstdlib>

ErdosRenyi::ErdosRenyi(GraphScene *scene) :
    Algorithm(scene),
    scene(scene),
    ctlW(0),
    size(START_SIZE),
    probability(START_PROBABILITY)
{
}

ErdosRenyi::~ErdosRenyi() {
}

bool ErdosRenyi::canAddVertex() {
    return true;
}

void ErdosRenyi::addVertex() {
    Node *node = scene->newNode();

    foreach (Node *other, scene->nodes()) {
        if ((double)qrand() / RAND_MAX < probability)
            scene->newEdge(node, other);
    }
}

void ErdosRenyi::reset() {
    QVector<Node*> nodesVector(size);

    for (int i(0); i < size; ++i) {
        Node *node = scene->newNode();
        nodesVector[i] = node;
    }

    for (int i(0); i < size; ++i) {
        for (int j(i+1); j < size; ++j) {
            if ((double)qrand() / RAND_MAX < probability) {
                scene->newEdge(nodesVector[i], nodesVector[j]);
            } else {
                continue;
            }
        }
    }
}

QWidget* ErdosRenyi::controlWidget(QWidget *parent) {
    if (!ctlW) {
        ctlW = new QWidget(parent);
        erCtrl = new Ui::ErdosControl();
        erCtrl->setupUi(ctlW);
        connect(erCtrl->nodesSpin, SIGNAL(valueChanged(int)), this, SLOT(onNodesChanged(int)));
        connect(erCtrl->probabilitySpin, SIGNAL(valueChanged(double)), this, SLOT(onProbabilityChanged(double)));
    }

    return ctlW;
}

void ErdosRenyi::onNodesChanged(int newValue) {
    size = newValue;
    scene->repopulate();
}

void ErdosRenyi::onProbabilityChanged(double newValue) {
    probability = newValue;
    scene->repopulate();
}

Ui::ErdosControl* const ErdosRenyi::getUi() {
    return erCtrl;
}

double ErdosRenyi::getProb() {
    return probability;
}
