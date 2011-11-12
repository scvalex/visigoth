#include "graphscene.h"
#include "bipartite.h"
#include "ui_bipartitecontrol.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

#include <QDebug>
#include <QWidget>
#include <QMessageBox>

Bipartite::Bipartite(GraphScene *scene) :
    Algorithm(scene),
    ctlW(0),
    uSize(START_USIZE),
    vSize(START_VSIZE),
    scene(scene)
{
}

Bipartite::~Bipartite() {
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

    // have to edit this for case uSize = 1
    for (int i(0); i < vSize; ++i) {
        Node *v = vVector[i];
        QList<bool> usedNodes;

        int n = 0;
        double degree = degreeDist(i);
        int cutoff = 0;

        while ((n < degree) && usedNodes.size() < uSize && cutoff < 1000) {

            // may have to implement check for infinite looping
            double rand = fmod(qrand(), cumulativePreferences.value(uSize-1));
            Node *u = uVector[getPreference(rand)];

            if (!scene->doesEdgeExist(u,v)) {
                if(!u->getVisited()){
                    usedNodes << u;
                    u->setVisited(true);
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

    // reset visited flag for stats
    for(int j(0); j < uSize; ++j){
        uVector[j]->setVisited(false);
    }

    vVector.clear();
    uVector.clear();
}

void Bipartite::addVertex() {
    qDebug("Bipartite does not support adding new vertices");
}

QWidget* Bipartite::controlWidget(QWidget *parent) {
    if (!ctlW) {
        ctlW = new QWidget(parent);
        Ui::BipartiteControl *bipCtl = new Ui::BipartiteControl();
        bipCtl->setupUi(ctlW);
        connect(bipCtl->uSizeEdit, SIGNAL(valueChanged(int)), this, SLOT(onUSizeChanged(int)));
        connect(bipCtl->uSizeEdit, SIGNAL(editingFinished()), this, SLOT(repopulate()));
        connect(bipCtl->vSizeEdit, SIGNAL(valueChanged(int)), this, SLOT(onVSizeChanged(int)));
        connect(bipCtl->vSizeEdit, SIGNAL(editingFinished()), this, SLOT(repopulate()));
    }
    return ctlW;
}

void Bipartite::repopulate() {
    scene->repopulate();
    scene->randomizePlacement();
}

double Bipartite::fitnessDist(int x) {
    if (x == 0) {
        return 0;
    }

    return 1/qPow(x,3);
}

// Ceil because we can not have fractional degrees
double Bipartite::degreeDist(int x) {
    if (x == 0) {
        return 1;
    }

    return qCeil(qPow(static_cast<double>(x), (qLn(3)/qLn(2)) - 1));
}

// Return the preferred node, using binary search.
int Bipartite::getPreference(double genPref) {
    const float E = 0.01;
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
    uSize = newSize;
}

void Bipartite::onVSizeChanged(int newSize) {
    vSize = newSize;
}
