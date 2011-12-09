#include "graphscene.h"
#include "wattsstrogatz.h"
#include "ui_wattscontrol.h"

#include <QtCore/qmath.h>
#include <cstdlib>

WattsStrogatz::WattsStrogatz(GraphScene* scene) :
    Algorithm(scene),
    scene(scene),
    ctlW(0),
    size(START_SIZE),
    probability(START_PROBABILITY),
    degree(START_DEGREE)
{
}

WattsStrogatz::~WattsStrogatz() {
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
        Ui::WattsControl *watsCtrl = new Ui::WattsControl;
        watsCtrl->setupUi(ctlW);

        watsCtrl->nodeSpin->setValue(size);
        watsCtrl->probSpin->setValue(probability);
        watsCtrl->degreeSpin->setValue(degree);

        connect(watsCtrl->nodeSpin, SIGNAL(valueChanged(int)), this, SLOT(onNodesChanged(int)));
        connect(watsCtrl->probSpin, SIGNAL(valueChanged(double)), this, SLOT(onProbabilityChanged(double)));
        connect(watsCtrl->degreeSpin, SIGNAL(valueChanged(int)),this,SLOT(onDegreeChanged(int)));
    }

    return ctlW;
}

void WattsStrogatz::reset(){
    QVector<Node*> nodeVec(size);

    for (int i(0); i < size; ++i){
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
        for (int r(1); r <= degree/2; ++r) {
            int nodeToSelect = (n+r)%size;

            if ((double)qrand() / RAND_MAX < probability) {
                scene->removeEdge(nodeVec[n], nodeVec[nodeToSelect]);
                int newNode = qrand()%size;

                for (int cutOff(0);
                     cutOff < 1000 && !scene->newEdge(nodeVec[n], nodeVec[newNode]);
                     ++cutOff) {
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

    size = newValue;
    scene->repopulate();
}

void WattsStrogatz::onProbabilityChanged(double newValue) {
    probability = newValue;
    scene->repopulate();
}

// The degree has to be smaller than the size. We should communicate the change
// to the user.
void WattsStrogatz::onDegreeChanged(int newValue) {
    if (newValue > size / 2) {
        newValue = size / 2;
    }

    degree = newValue;
    scene->repopulate();
}
