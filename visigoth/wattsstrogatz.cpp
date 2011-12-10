#include "graphscene.h"
#include "wattsstrogatz.h"
#include "ui_wattscontrol.h"

#include <QtCore/qmath.h>
#include <cstdlib>

WattsStrogatz::WattsStrogatz(GraphScene* scene) :
    Algorithm(scene),
    scene(scene),
    ctlW(0),
    wattsCtl(0),
    size(START_SIZE),
    probability(START_PROBABILITY),
    degree(START_DEGREE)
{
}

WattsStrogatz::~WattsStrogatz() {
    if (ctlW != 0) {
        delete ctlW;
        delete wattsCtl;
    }
}

int WattsStrogatz::getNumNodes() const {
    return size;
}

int WattsStrogatz::getNodeDegree() const {
    return degree;
}

double WattsStrogatz::getProbability() const {
    return probability;
}

bool WattsStrogatz::canAddVertex() {
    return false;
}

void WattsStrogatz::addVertex() {
    qDebug("Watts Strogatz does not support adding new vertices");
}

QWidget* WattsStrogatz::controlWidget(QWidget *parent) {
    if (!ctlW) {
        ctlW = new QWidget(parent);
        wattsCtl = new Ui::WattsControl;
        wattsCtl->setupUi(ctlW);

        updateUI();

        connect(wattsCtl->nodeSpin, SIGNAL(valueChanged(int)), this, SLOT(onNodesChanged(int)));
        connect(wattsCtl->probSpin, SIGNAL(valueChanged(double)), this, SLOT(onProbabilityChanged(double)));
        connect(wattsCtl->degreeSpin, SIGNAL(valueChanged(int)), this, SLOT(onDegreeChanged(int)));
    }

    return ctlW;
}

void WattsStrogatz::reset() {
    QVector<Node*> nodeVec(size);

    for (int i(0); i < size; ++i) {
        Node *node = scene->newNode();
        nodeVec[i] = node;
    }

    // construct ring lattice
    for (int j(0); j < size; ++j) {
        // connecting right side
        for (int r(1); r<= degree/2; ++r) {
            int nodeToConnect = (j+r) % size;
            scene->newEdge(nodeVec[j], nodeVec[nodeToConnect]);
        }

        // connecting left side
        for (int l(1); l<= degree/2; ++l) {
            int nodeToConnect = (size+j-l)%size;
            scene->newEdge(nodeVec[j], nodeVec[nodeToConnect]);
        }
    }

    // rewire
    for (int n(0); n < size; ++n) {
        // only choose the right side, since we only select (ni,nj) with i < j
        for (int r(1); r <= degree / 2; ++r) {
            int nodeToSelect = (n+r) % size;

            if ((double)qrand() / RAND_MAX < probability) {
                scene->removeEdge(nodeVec[n], nodeVec[nodeToSelect]);
                int newNode = qrand() % size;

                for (int cutOff(0);
                     cutOff < 1000 && !scene->newEdge(nodeVec[n], nodeVec[newNode]);
                     ++cutOff)
                {
                    newNode = qrand() % size;
                }
            }
        }
    }
}

void WattsStrogatz::onNodesChanged(int newValue) {
    if (newValue < degree * 2) {
        newValue = degree * 2;
    }

    if (newValue == size) {
        return;
    }

    size = newValue;
    scene->repopulate();
}

void WattsStrogatz::onProbabilityChanged(double newValue) {
    if (newValue == probability) {
        return;
    }

    probability = newValue;
    scene->repopulate();
}

// The degree has to be smaller than the size. We should communicate the change
// to the user.
void WattsStrogatz::onDegreeChanged(int newValue) {
    if (newValue > size / 2) {
        newValue = size / 2;
    }

    if (newValue == degree) {
        return;
    }

    degree = newValue;
    scene->repopulate();
}

void WattsStrogatz::updateUI() {
    if (ctlW == 0)
        return;

    if (wattsCtl->nodeSpin->value() != size) {
        wattsCtl->nodeSpin->setValue(size);
    }
    if (wattsCtl->probSpin->value() != probability) {
        wattsCtl->probSpin->setValue(probability);
    }
    if (wattsCtl->degreeSpin->value() != degree) {
        wattsCtl->degreeSpin->setValue(degree);
    }
}
