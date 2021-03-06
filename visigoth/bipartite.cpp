#include "graphscene.h"
#include "bipartite.h"
#include "mainwindow.h"
#include "notify.h"
#include "ui_bipartitecontrol.h"
#include "ui_mainwindow.h"

#include <QSet>
#include <QWidget>

Bipartite::Bipartite(GraphScene *scene) :
    // true will be set when it actually works
    Algorithm(scene, true),
    ctlW(0),
    uSize(START_USIZE),
    vSize(START_VSIZE),
    scene(scene),
    bipartiteCtl(0)
{
}

Bipartite::~Bipartite() {
    if (ctlW != 0) {
        delete ctlW;
        delete bipartiteCtl;
    }
}

int Bipartite::getUSize() const {
    return uSize;
}

int Bipartite::getVSize() const {
    return vSize;
}

void Bipartite::reset() {
    uVector.clear();
    vVector.clear();
    cumulativePreferences.clear();

    for (int i(0); i < uSize; ++i) {
        uVector << scene->newNode();
    }

    for (int i(0); i < vSize; ++i) {
        vVector << scene->newNode();
    }

    // Use class edgeList
    updatePreference();

    QSet<Node*> visited;
    // have to edit this for case uSize = 1
    for (int i(0); i < vSize; ++i) {
        Node *v = vVector[i];
        QList<bool> usedNodes;

        int n = 0;
        double degree = degreeDist(i);
        int cutoff = 0;

        while ((n < degree) && usedNodes.size() < uSize && cutoff < 1000) {

            // may have to implement check for infinite looping
            double rand = fmod(qrand(), cumulativePreferences.value(uSize-1)) + (fmod(qrand(),1000)/1000);
            Node *u = uVector[getPreference(rand)];

            if (!scene->doesEdgeExist(u,v)) {
                if (!visited.contains(u)) {
                    usedNodes << u;
                    visited.insert(u);
                }
                scene->newEdge(u,v);
                n += 1;
            }
            ++cutoff;
        }
    }

    for (int i(0); i < uSize; ++i) {
        Node *u = uVector[i];
        // if not connected
        if (u->edges().size() == 0) {
            Node *v = vVector[qrand() % vSize];
            scene->newEdge(u,v);
        }
    }

    for (int i(0); i < vSize; ++i) {
        Node *v = vVector[i];

        QList<Edge*> A;

        // Find all edges where v is the dest Node
        foreach (Edge *e, scene->edges()) {
            if (e->destNode()->tag() == v->tag()) {
                A << e;
            }
        }

        foreach (Edge *e1, A) {
            foreach (Edge *e2, A) {
                Node *u1 = e1->sourceNode();
                Node *u2 = e2->sourceNode();

                if (u1->tag() != u2->tag()){
                        scene->newEdge(u1,u2);
                }
            }
        }
    }

    scene->removeEdges(uSize);

    for(int i(vSize-1); i >= 0 ; --i){
        scene->removeNode(vVector[i]);
    }

    vVector.clear();
    uVector.clear();
}

bool Bipartite::canAddVertex() {
    return false;
}

void Bipartite::addVertex() {
    Notify::important("Bipartite does not support adding new vertices");
}

QWidget* Bipartite::controlWidget(QWidget *parent) {
    if (ctlW == 0) {
        ctlW = new QWidget(parent);
        bipartiteCtl = new Ui::BipartiteControl();
        bipartiteCtl->setupUi(ctlW);

        updateUI();

        connect(bipartiteCtl->uSizeEdit, SIGNAL(valueChanged(int)), this, SLOT(onUSizeChanged(int)));
        connect(bipartiteCtl->vSizeEdit, SIGNAL(valueChanged(int)), this, SLOT(onVSizeChanged(int)));
    }
    return ctlW;
}

double Bipartite::fitnessDist(int x) {
    if (x == 0) {
        return 0;
    }

    return 1/qPow(x,3);
}

// Ceil because we can not have fractional degrees
// 0.125 is an expoent we found to give an good scale free exponent
double Bipartite::degreeDist(int x) {
    if (x == 0) {
        return 1;
    }

    return qPow(static_cast<double>(x), .125);
}


// Return the preferred node, using binary search.
int Bipartite::getPreference(double genPref) {
    const float E = 0.0001;
    int l;
    for (l = 1; l < uVector.size(); l <<= 1)
        ;
    int i(0);
    for (; l > 0; l >>= 1) {
        if (l + i < uVector.size()) {
            if (cumulativePreferences[l + i] <= genPref + E)
                i += l;
        }
    }
    return i;
}

void Bipartite::updatePreference() {
    double prefCumulative = 0;

    if (uVector.size() == 1) {
        cumulativePreferences.insert(0,1);
        return;
    }

    for (int i(0); i < uVector.size(); ++i) {
        Node *node = uVector[i];
        cumulativePreferences.insert(node->tag(),prefCumulative);
        prefCumulative += fitnessDist(node->tag() + 1);
    }
}

void Bipartite::onUSizeChanged(int newSize) {
    if (newSize == uSize)
        return;

    uSize = newSize;
    updateUI();
    scene->repopulate();
}

void Bipartite::onVSizeChanged(int newSize) {
    if (newSize == vSize)
        return;

    vSize = newSize;
    updateUI();
    scene->repopulate();
}

void Bipartite::updateUI() {
    if (!bipartiteCtl)
        return;

    if (bipartiteCtl->uSizeEdit->value() != uSize) {
        bipartiteCtl->uSizeEdit->setValue(uSize);
    }
    if (bipartiteCtl->vSizeEdit->value() != vSize) {
        bipartiteCtl->vSizeEdit->setValue(vSize);
    }
}
