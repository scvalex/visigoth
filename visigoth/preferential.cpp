#include "graphscene.h"
#include "preferential.h"
#include "ui_preferentialcontrol.h"

#include <QWidget>

Preferential::Preferential(GraphScene *graph) :
    Algorithm(graph),
    graph(graph),
    ctlW(0),
    size(START_NODES)
{
    updatePreference(graph->nodes(), 2 * graph->edges().size());
}

void Preferential::reset() {
    preferences.clear();
    cumulativePreferences.clear();
    for (int i(0); i < size; ++i) {
        addVertex(false);
    }
}

void Preferential::addVertex() {
    addVertex(true);
}

void Preferential::addVertex(bool saveSize) {
    addVertex((qrand() % 3 ) + 1, qrand() % 100);
    if (saveSize) {
        ++size;
    }
}

QWidget* Preferential::controlWidget(QWidget *parent) {
    if (!ctlW) {
        ctlW = new QWidget(parent);
        Ui::PreferentialControl *prefCtl = new Ui::PreferentialControl();
        prefCtl->setupUi(ctlW);
        connect(prefCtl->sizeEdit, SIGNAL(valueChanged(int)), this, SLOT(onSizeChanged(int)));
    }
    return ctlW;
}

// Add vertex using preferential attachment with clustering.
void Preferential::addVertex(int edgesToAdd, double p) {
    Node *vPref;
    int numNodes = graph->nodes().size();
    int numEdges = graph->edges().size();
    QList<Node*> usedNodes;
    QVector<Node*> nodes = graph->nodes(); /* important: doesn't contain the new node */
    Node *vertex = graph->newNode();

    // saftey check to ensure that the method
    // does not get stuck looping

    if (edgesToAdd > numNodes) {
        edgesToAdd = numNodes;
    }

    while (edgesToAdd > 0) {
        vPref = getPreference(nodes, genRandom());
        int cutOff;
        for (cutOff = 0; cutOff < 1000 && !graph->newEdge(vertex, vPref); ++cutOff) {
            vPref = getPreference(nodes, genRandom());
        }
        if (cutOff == 1000)
            break;

        --edgesToAdd;

        usedNodes << vPref;

        if (genRandom() < p) {
            addNewEdges(edgesToAdd, vertex, vPref->neighbours(), usedNodes);
        }

        if (usedNodes.size() >= numNodes)
            break;
    }

    updatePreference(graph->nodes(), 2 * numEdges);
}

void Preferential::addNewEdges(int edgesToAdd,
                               Node *vertex, QVector<Node*> neighbours,
                               QList<Node*> &usedNodes) {
    int length = neighbours.size();

    while (edgesToAdd > 0 && !(neighbours.empty())) {
        int rand = qrand() % length;
        Node *vi = neighbours[rand];

        if (graph->newEdge(vertex, vi)) {
            --edgesToAdd;
            usedNodes << vi;
            neighbours = getIntersection(neighbours, vi->neighbours());
        } else {
            neighbours.remove(rand);
        }

        length = neighbours.size();
    }
}

void Preferential::updatePreference(const QVector<Node*> &nodes, int totalDegree) {
    int prefCumulative = 0;

    if (nodes.size() == 1) {
        preferences[nodes.first()->tag()] = 100;
        cumulativePreferences[nodes.first()->tag()] = 100;
        return;
    }

    foreach (Node *node, nodes) {
        double tempLength = (double)node->edges().length();
        double tempPref = (tempLength / (double) totalDegree) * 100;
        preferences[node->tag()] = tempPref;
        cumulativePreferences[node->tag()] = prefCumulative;
        prefCumulative += tempPref;
    }
}

// Return the preferred node, using binary search.
Node* Preferential::getPreference(const QVector<Node*> &nodes, double genPref) {
    const float E = 0.0001;
    int l;
    for (l = 1; l < nodes.size(); l <<= 1)
        ;
    int i(0);
    for (; l > 0; l >>= 1) {
        if (l + i < nodes.size()) {
            if (cumulativePreferences[l + i] <= genPref + E)
                i += l;
        }
    }
    return nodes[i];
}

QVector<Node*> Preferential::getIntersection(QVector<Node*> vec1, QVector<Node*> vec2) {
    QVector<Node*> retVec;
    QVector<Node*> *shorterVec;
    QVector<Node*> *longerVec;
    int length;
    if (vec1.size() > vec2.size()) {
        length = vec1.size();
        shorterVec = &vec2;
        longerVec = &vec1;
    } else {
       length = vec2.size();
       shorterVec = &vec1;
       longerVec = &vec2;
    }

    for (int i(0); i < length; ++i) {
        Node* tempPointer = longerVec->at(i);
        if (shorterVec->contains(tempPointer)) {
            retVec << tempPointer;
        }

    }

    return retVec;
}

// Generate random double with 4 precision.
double Preferential::genRandom(){
    double main = qrand() % 100;
    return main + (( qrand() % 10000 ) / 10000 );
}

void Preferential::onSizeChanged(int newSize) {
    size = newSize;
}
