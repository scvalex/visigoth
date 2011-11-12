#include "graphscene.h"
#include "erdosrenyi.h"
#include "ui_erdoscontrol.h"

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
        Ui::ErdosControl *erCtl = new Ui::ErdosControl();
        erCtl->setupUi(ctlW);
        connect(erCtl->nodesSpin, SIGNAL(valueChanged(int)), this, SLOT(onNodesChanged(int)));
        //connect(erCtl->nodesSpin, SIGNAL(editingFinished()), this, SLOT(repopulate()));
        connect(erCtl->probabilitySpin, SIGNAL(valueChanged(double)), this, SLOT(onProbabilityChanged(double)));
        //connect(erCtl->probabilitySpin, SIGNAL(editingFinished()), this, SLOT(repopulate()));
    }

    return ctlW;
}

void ErdosRenyi::repopulate() {
    scene->repopulate();
    scene->randomizePlacement();
}

void ErdosRenyi::onNodesChanged(int newValue) {
    size = newValue;
    repopulate();
}

void ErdosRenyi::onProbabilityChanged(double newValue) {
    probability = newValue;
    repopulate();
}
