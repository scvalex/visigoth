#include "graphscene.h"
#include "preferential.h"

Preferential::Preferential(GraphScene *graph) :
    graph(graph)
{
    updatePreference(graph->nodes(), 2 * graph->edges().size());
}

// Add vertex using preferential attachment with clustering.
void Preferential::addVertex(int edgesToAdd, double p) {
    Node *vPref;
    int numNodes = graph->nodes().size();
    int numEdges = graph->edges().size();
    Node *vertex = new Node(graph);
    QList<Node*> usedNodes;

    // saftey check to ensure that the method
    // does not get stuck looping

    if (edgesToAdd > numNodes) {
        edgesToAdd = numNodes;
    }

    while (edgesToAdd > 0) {
        vPref = getPreference(graph->nodes(), genRandom());
        int cutOff;
        for (cutOff = 0; cutOff < 100 && !graph->addNewEdge(vertex, vPref); ++cutOff) {
            vPref = getPreference(graph->nodes(), genRandom());
        }
        if (cutOff == 100)
            break;

        --edgesToAdd;

        usedNodes << vPref;

        if (genRandom() < p) {
            addNewEdges(edgesToAdd, vertex, vPref->neighbours(), usedNodes);
        }

        if (usedNodes.count() >= numNodes)
            break;
    }

    graph->addNode(vertex);
    updatePreference(graph->nodes(), 2 * numEdges);
}

void Preferential::addNewEdges(int edgesToAdd,
                               Node *vertex, QVector<Node*> neighbours,
                               QList<Node*> &usedNodes) {
    int length = neighbours.count();

    while (edgesToAdd > 0 && !(neighbours.empty())) {
        int rand = qrand() % length;
        Node *vi = neighbours[rand];

        if (graph->addNewEdge(vertex, vi)) {
            --edgesToAdd;
            usedNodes << vi;
            neighbours = getIntersection(neighbours, vi->neighbours());
        } else {
            neighbours.remove(rand);
        }

        length = neighbours.count();
    }
}

void Preferential::updatePreference(const QVector<Node*> &nodes, int totalDegree) {
    int prefCumulative = 0;

    if (nodes.count() == 1) {
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
    const float E = 0.01;
    int l;
    for (l = 1; l < nodes.count(); l <<= 1)
        ;
    int i(0);
    for (; l > 0; l >>= 1) {
        if (l + i < nodes.count()) {
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
    if (vec1.count() > vec2.count()) {
        length = vec1.count();
        shorterVec = &vec2;
        longerVec = &vec1;
    } else {
       length = vec2.count();
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

// Generate random double with 2 precision.
double Preferential::genRandom(){
    double main = qrand() % 100;
    return main + (( qrand() % 100 ) / 100 );
}
