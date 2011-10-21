#include "preferential.h"

// Add vertex using preferential attachment with clustering.
void Preferential::addVertex(GraphWidget *graph, int edgesToAdd, double p) {
    Node *vPref;
    int numNodes(0);
    int numEdges(0);
    foreach (QGraphicsItem *item, graph->scene()->items()) {
        if (qgraphicsitem_cast<Node*>(item))
            ++numNodes;
        if (qgraphicsitem_cast<Edge*>(item))
            ++numEdges;
    }
    Node* vertex = new Node(numNodes, graph);
    QVector<Node*> *neighbours;
    QList<Node*> *usedNodes = new QList<Node*>();

    // saftey check to ensure that the method
    // does not get stuck looping

    if (edgesToAdd > numNodes) {
        edgesToAdd = numNodes;
    }

    while (edgesToAdd != 0) {
        do {
            vPref = getPreference(graph->scene()->items(), genRandom());
        } while (graph->doesEdgeExist(vertex->tag(), vPref->tag()));

        Edge *edge = new Edge(vertex, vPref);
        graph->addNewEdge(edge);
        --edgesToAdd;

        usedNodes->append(vPref);

        if (genRandom() < p){
            neighbours = getNeighbours(vPref);
            addNewEdges(graph, edgesToAdd, vertex, neighbours, usedNodes);
        }

        if (usedNodes->count() == numNodes){
            edgesToAdd = 0;
        }
    }

    graph->addNode(vertex);
    updatePreference(graph->scene()->items(), 2*numEdges);
}

void Preferential::addNewEdges(GraphWidget *graph, int edgesToAdd,
                             Node *vertex, QVector<Node *> *neighbours,
                             QList<Node*> *usedNodes) {
    QVector<Node*> *setUsed = neighbours;
    int length = setUsed->count();

    while (edgesToAdd != 0 && !(setUsed->empty())) {
        int rand = qrand() % length;
        Node *vi = setUsed->at(rand);

        if (graph->addNewEdge(new Edge(vertex, vi))) {
            --edgesToAdd;
            usedNodes->append(vi);
            setUsed = getIntersection(neighbours, getNeighbours(vi));
        } else {
            setUsed->remove(rand);
        }

        length = setUsed->count();
    }
}

QVector<Node*>* Preferential::getNeighbours(Node *n) {
    QList<Edge*> tempList = *(n->edges());
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

void Preferential::updatePreference(QList<QGraphicsItem*> items, int totalDegree) {
    int prefCumulative = 0;

    if (items.count() == 1) {
        Node *node = qgraphicsitem_cast<Node*>(items.first());
        if (!node)
            return;
        node->setPref(100);
        node->setCumPref(100);
        return;
    }

    foreach (QGraphicsItem *item, items) {
        Node *node = qgraphicsitem_cast<Node*>(item);
        if (!node)
            continue;

        double tempLength = (double)node->edges()->length();
        double tempPref = (tempLength / (double) totalDegree) * 100;
        node->setPref(tempPref);
        node->setCumPref(prefCumulative);
        prefCumulative += tempPref;
    }
}

// Return the preferred node, using binary search.
Node* Preferential::getPreference(QList<QGraphicsItem*> items, double genPref) {
    QVector<Node*> nodes;
    foreach (QGraphicsItem *item, items) {
        if (Node *node = qgraphicsitem_cast<Node*>(item))
            nodes << node;
    }
    int start = 0;
    int end = nodes.count() - 1;
    bool found = false;
    Node* retNode;

    while (!found) {
        int avg = (start + end) / 2;
        Node *temp1 = nodes.at(avg);

        if ((start == end ) || (avg == end)) {
            retNode = temp1;
            found = true;
        } else {
            Node *temp2 = nodes.at(avg + 1);
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

QVector<Node*>* Preferential::getIntersection(QVector<Node*> *vec1, QVector<Node*> *vec2) {
    QVector<Node*> *retVec = new QVector<Node*>();
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
double Preferential::genRandom(){
    double main = qrand() % 100;
    return main + (( qrand() % 100 ) / 100 );
}
