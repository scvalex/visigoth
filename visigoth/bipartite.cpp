#include "graphscene.h"
#include "bipartite.h"

Bipartite::Bipartite(GraphScene *scene) :
    scene(scene)
{
}

void Bipartite::init(int size) {
    init(size, size);
}

void Bipartite::init(int vSize, int uSize) {
   QVector<Node*> vVector(vSize);
   QVector<Node*> uVector(uSize);

    for (int i(0); i < uSize; ++i) {
        Node *u = scene->newNode();
        uVector[i] = u;
    }

    for (int i(0); i < vSize; ++i) {
        Node *v =  scene->newNode();
        vVector[i] = v;
    }

    // Use class edgeList
    updatePreference();

    // have to edit this for case uSize = 1
    for (int i(0); i < vSize; ++i) {
        Node *v = vVector[i];
        QList<Node*> usedNodes;

        int n = 0;
        double degree = degreeDist(i);
        int cutoff = 0;

        while ((n < degree) && usedNodes.count() < uSize && cutoff < 100) {

            // may have to implement check for infinite looping
            double rand = fmod(qrand(),cumulativePreferences[uSize-1]);
            Node *u = uVector[getPreference(rand)];

            if (!scene->doesEdgeExist(u,v)) {
                usedNodes << u;
                scene->newEdge(u,v);
                n += 1;
            }
            ++cutoff;
        }
    }

    for (int i(0); i < uSize; ++i) {
        Node *u = uVector[i];
        // if not connected
        if (u->edges().count() == 0) {
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

    vVector.clear();
    uVector.clear();
}

void Bipartite::addVertex() {
    qDebug("Bipartite does not support adding new vertices");
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

    return qCeil(qPow((double)x, (qLn(3)/qLn(2)) - 1));
}

// Return the preferred node, using binary search.
int Bipartite::getPreference(double genPref) {
    const float E = 0.01;
    int l;
    for (l = 1; l < uVector.count(); l <<= 1)
        ;
    int i(0);
    for (; l > 0; l >>= 1) {
        if (l + i < uVector.count()) {
            if (cumulativePreferences[l + i] <= genPref + E)
                i += l;
        }
    }
    return i;
}

void Bipartite::updatePreference() {
    double prefCumulative = 0;

    if (uVector.count() == 1) {
        cumulativePreferences[0] = 1;
        return;
    }

    for (int i(0); i < uVector.count(); ++i) {
        Node *node = uVector[i];
        cumulativePreferences[node->tag()] = prefCumulative;
        prefCumulative += fitnessDist(node->tag() + 1);
    }
}
