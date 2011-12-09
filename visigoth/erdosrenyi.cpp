#include "graphscene.h"
#include "erdosrenyi.h"
#include "ui_erdoscontrol.h"

#include <QtCore/qmath.h>
#include <cstdlib>

ErdosRenyi::ErdosRenyi(GraphScene *scene) :
    Algorithm(scene),
    scene(scene),
    ctlW(0),
    erCtl(0),
    size(START_SIZE),
    probability(START_PROBABILITY)
{
}

ErdosRenyi::~ErdosRenyi() {
    if (ctlW != 0) {
        delete ctlW;
        delete erCtl;
    }
}

int ErdosRenyi::getNumNodes() const {
    return size;
}

double ErdosRenyi::getProbability() const {
    return probability;
}

bool ErdosRenyi::canAddVertex() {
    return true;
}

void ErdosRenyi::addVertex() {
    ErdosRenyi::addVertex(true);
}

void ErdosRenyi::addVertex(bool saveSize) {
    Node *node = scene->newNode();

    foreach (Node *other, scene->nodes()) {
        if ((double)qrand() / RAND_MAX < probability)
            scene->newEdge(node, other);
    }

    if (saveSize) {
        ++size;
    }

    updateUI();
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
    if (ctlW == 0) {
        ctlW = new QWidget(parent);
        erCtl = new Ui::ErdosControl();
        erCtl->setupUi(ctlW);

        updateUI();

        connect(erCtl->nodesSpin, SIGNAL(valueChanged(int)), this, SLOT(onNodesChanged(int)));
        connect(erCtl->probabilitySpin, SIGNAL(valueChanged(double)), this, SLOT(onProbabilityChanged(double)));
    }

    return ctlW;
}

void ErdosRenyi::onNodesChanged(int newValue) {
    if (newValue == size)
        return;

    size = newValue;
    updateUI();
    scene->repopulate();
}

void ErdosRenyi::onProbabilityChanged(double newValue) {
    if (newValue == probability)
        return;

    probability = newValue;
    updateUI();
    scene->repopulate();
}

void ErdosRenyi::updateUI() {
    if (!erCtl)
        return;

    if (erCtl->nodesSpin->value() != size) {
        erCtl->nodesSpin->setValue(size);
    }
    if (erCtl->probabilitySpin->value() != probability) {
        erCtl->probabilitySpin->setValue(probability);
    }
}
