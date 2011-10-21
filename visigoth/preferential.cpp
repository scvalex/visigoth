#include "preferential.h"

Preferential::Preferential(GraphWidget *graph) :
    graph(graph)
{
    int numEdges(0);
    foreach (QGraphicsItem *item, graph->scene()->items()) {
        if (qgraphicsitem_cast<Edge*>(item))
            ++numEdges;
    }
    updatePreference(graph->scene()->items(), 2 * numEdges);
}

// Add vertex using preferential attachment with clustering.
void Preferential::addVertex(int edgesToAdd, double p) {
    Node *vPref;
    int numNodes(0);
    int numEdges(0);
    foreach (QGraphicsItem *item, graph->scene()->items()) {
        if (qgraphicsitem_cast<Node*>(item))
            ++numNodes;
        if (qgraphicsitem_cast<Edge*>(item))
            ++numEdges;
    }
    Node *vertex = new Node(graph);
    QVector<Node*> *neighbours;
    QList<Node*> *usedNodes = new QList<Node*>();

    // saftey check to ensure that the method
    // does not get stuck looping

    if (edgesToAdd > numNodes) {
        edgesToAdd = numNodes;
    }

    while (edgesToAdd > 0) {
        vPref = getPreference(graph->scene()->items(), genRandom());
        int cutOff;
        for (cutOff = 0; cutOff < 100 && graph->doesEdgeExist(vertex->tag(), vPref->tag()); ++cutOff) {
            vPref = getPreference(graph->scene()->items(), genRandom());
        }
        if (cutOff == 100)
            break;

        Edge *edge = new Edge(vertex, vPref);
        graph->addNewEdge(edge);
        --edgesToAdd;

        usedNodes->append(vPref);

        if (genRandom() < p){
            neighbours = getNeighbours(vPref);
            addNewEdges(edgesToAdd, vertex, neighbours, usedNodes);
        }

        if (usedNodes->count() == numNodes){
            edgesToAdd = 0;
        }
    }

    graph->addNode(vertex);
    updatePreference(graph->scene()->items(), 2 * numEdges);
}

void Preferential::addNewEdges(int edgesToAdd,
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
        preferences[node->tag()] = 100;
        cumulativePreferences[node->tag()] = 100;
        return;
    }

    foreach (QGraphicsItem *item, items) {
        Node *node = qgraphicsitem_cast<Node*>(item);
        if (!node)
            continue;

        double tempLength = (double)node->edges()->length();
        double tempPref = (tempLength / (double) totalDegree) * 100;
        preferences[node->tag()] = tempPref;
        cumulativePreferences[node->tag()] = prefCumulative;
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
