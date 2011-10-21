#include "algorithms.h"

// Add vertex using preferential attachment with clustering.
void Algorithms::addVertex(GraphWidget *graph, int edgesToAdd, double p) {
    Node *vPref;
    QVector<Node*> *nVec = graph->getNodeVector();
    int nVecLength = nVec->count();
    Node* vertex = new Node(nVecLength, graph);
    QList<Edge*> *edges = graph->getEdgeList();
    QVector<Node*> *neighbours;
    QList<Node*> *usedNodes = new QList<Node*>();

    // saftey check to ensure that the method
    // does not get stuck looping

    if (edgesToAdd > nVecLength) {
        edgesToAdd = nVecLength;
    }

    while (edgesToAdd != 0) {
        do {
            vPref = getPref(nVec, genRandom());
        } while (edgeExists(vertex->tag(), vPref->tag(), edges));

        Edge *edge = new Edge(vertex, vPref);
        graph->addEdgeToScene(edge);
        *edges << edge;
        --edgesToAdd;

        usedNodes->append(vPref);

        if (genRandom() < p){
            neighbours = getNeighbours(vPref);
            addNewEdges(graph, edgesToAdd, vertex, neighbours, usedNodes);
        }

        if (usedNodes->count() == nVecLength){
            edgesToAdd = 0;
        }
    }

    *nVec << vertex;
    graph->addNodeToScene(vertex);
    updatePreference(nVec, 2*edges->count());
}

void Algorithms::addNewEdges(GraphWidget *graph, int edgesToAdd,
                             Node *vertex, QVector<Node *> *neighbours,
                             QList<Node*> *usedNodes) {
    QList<Edge*> *edges = graph->getEdgeList();
    QVector<Node*> *setUsed = neighbours;
    int length = setUsed->count();

    while (edgesToAdd != 0 && !(setUsed->empty())) {
        int rand = qrand() % length;
        Node *vi = setUsed->at(rand);

        if (!edgeExists(vertex->tag(), vi->tag(), graph->getEdgeList())) {
            Edge *e = new Edge(vertex, vi);
            *edges << e;
            --edgesToAdd;
            graph->addEdgeToScene(e);
            usedNodes->append(vi);
            setUsed = getIntersection(neighbours, getNeighbours(vi));
        } else {
            setUsed->remove(rand);
        }

        length = setUsed->count();
    }
}

QVector<Node*>* Algorithms::getNeighbours(Node *n) {
    QList<Edge*> tempList = *(n->getList());
    QVector<Node*> *neighbours = new QVector<Node*>();
    int nTag = n->tag();

    for (QList<Edge*>::const_iterator ii = tempList.constBegin();
         ii != tempList.constEnd();
         ++ii)
    {
        int sT = (*ii)->sourceNode()->tag();

        if(nTag == sT) {
            neighbours->append((*ii)->destNode());
        } else{
            neighbours->append((*ii)->sourceNode());
        }
    }

    return neighbours;
}

void Algorithms::updatePreference(QVector<Node*> *nVec, int totalDegree) {
    int prefCumulative = 0;

    if(nVec->count() == 1) {
        nVec->first()->setPref(100);
        nVec->first()->setCumPref(100);
        return;
    }

    foreach (Node *n, *nVec) {
        double tempLength = (double)n->getList()->length();
        double tempPref = (tempLength / (double) totalDegree) * 100;
        n->setPref(tempPref);
        n->setCumPref(prefCumulative);
        prefCumulative += tempPref;
    }
}

// Return the preferred node, using binary search.
Node* Algorithms::getPref(QVector<Node*> *nVec, double genPref) {
    int start = 0;
    int end = nVec->count() - 1;
    bool found = false;
    Node* retNode;

    while (!found) {
        int avg = (start + end) / 2;
        Node *temp1 = nVec->at(avg);

        if ((start == end ) || (avg == end)) {
            retNode = temp1;
            found = true;
        } else {
            Node *temp2 = nVec->at(avg + 1);
            double pref1 = temp1->getCumPref();
            double pref2 = temp2->getCumPref();
            if (genPref >= pref1 && genPref < pref2) {
                found = true;
                retNode = temp1;
            } else if (genPref > pref1) {
                start = avg + 1;
            } else {
                end = avg - 1;
            }
        }
    }

    return retNode;
}

bool Algorithms::edgeExists(int sourceTag, int destTag, QList<Edge*> *edges) {
    bool exist = false;

    for(QList<Edge*>::const_iterator ii = edges->constBegin();
        ii != edges->constEnd();
        ++ii)
    {
        int sTag = (*ii)->sourceNode()->tag();
        int dTag = (*ii)->destNode()->tag();


        if ((sTag == sourceTag && dTag == destTag) || (sourceTag == destTag)) {
            exist = true;
            break;
        }
    }

    return exist;
}

QVector<Node*>* Algorithms::getIntersection(QVector<Node*> *vec1, QVector<Node*> *vec2) {
    QVector<Node*> *retVec = new QVector<Node *>();
    QVector<Node*> *shorterVec;
    QVector<Node*> *longerVec;
    int length;
    if (vec1->count() > vec2->count()) {
        length = vec1->count();
        shorterVec = vec2;
        longerVec = vec1;
    } else {
       length = vec2->count();
       shorterVec = vec1;
       longerVec = vec2;
    }

    for (int i(0); i < length; ++i) {
        Node *tempPointer = longerVec->at(i);
        if (shorterVec->contains(tempPointer)) {
            retVec->append(tempPointer);
        }

    }

    return retVec;
}

// Generate random double with 2 precision.
double Algorithms::genRandom(){
    double main = qrand() % 100;
    return main + (( qrand() % 100 ) / 100 );
}
