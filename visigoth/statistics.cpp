#include "statistics.h"
#include "graphscene.h"



Statistics::Statistics(GraphScene *scene): graph(scene){

}

double Statistics::averageLength(){





    // placeholder
    return 0.0;

}

double Statistics::averageDegree(){

    return (2 * graph->edges().count() ) / graph->nodes().count();
}

double Statistics::clusteringCoeff(Node *node){

    QList<Edge *> edges = node->edges();
    int k = edges.count();
    int intersection = 0;

    while(!edges.empty()){

        Edge * e = edges.takeFirst();
        Node * src = e->sourceNode();
        Node * dest = e->destNode();

        QVector<Node *> nNeigh = buildNeighbourVector(node);

        if(src->tag() == node->tag()){

            QVector<Node *> dNeigh = buildNeighbourVector(dest);
            intersection+=intersectionCount(nNeigh,dNeigh);
        }

        else{

            QVector<Node *> dNeigh = buildNeighbourVector(dest);
            intersection+=intersectionCount(nNeigh,dNeigh);

        }

    }

    return 2*intersection/(k*(k-1));

}

double Statistics::custeringAvg(){

    QVector<Node *> nodes = graph->nodes();
    double clusterCumulative = 0;
    int size = nodes.count();

    for(int i(0); i < size; ++i){
        clusterCumulative+= clusteringCoeff(nodes[i]);
    }

    return clusterCumulative/size;


}

double Statistics::clusteringDegree(int degree){


    QList<Node *> nodeList = graph->getDegreeList(degree);
    int degreeCount = nodeList.count();
    int clusterCumulative= 0;

    foreach (Node * n, nodeList){

        clusterCumulative+=clusteringCoeff(n);

    }


    return clusterCumulative/degreeCount;


}

QVector<Node *> Statistics::buildNeighbourVector(Node *n){

    QList<Edge *> eList = n->edges();
    QVector<Node *> retVec(eList.count());

    while(!eList.empty()){

        Edge * e = eList.takeFirst();

        if(e->sourceNode()->tag() == n->tag()){

            retVec << e->destNode();

        }

        else{
            retVec << e->sourceNode();
        }

    }

    return retVec;

}

int Statistics::intersectionCount(QVector<Node *> vec1, QVector<Node *> vec2){

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

    return retVec.count();
}





