#include "algorithms.h"

void Algorithms::addVertex(GraphWidget *graph, int maxNeighbourhoodSize, double p){

    double random = genRandom();
    QVector<Node*>* nVec = graph->getNodeVector();
    Node* vertex = new Node(nVec->count(), graph);
    QList<Edge *>* edges = graph->getEdgeList();
    QVector<Node*> neighbours;
    int n = 0;

    while (n < maxNeighbourhoodSize){
        Node * vPref = getPref(nVec,random);
        Edge* e = new Edge(vertex,vPref);
        *edges << e;
        neighbours = getNeighbours(vertex);
        foreach(Node* n, neighbours){
            if(genRandom() < p){
                *edges << new Edge(vPref, n);
            }
        }

        neighbours.append(vPref);
        ++n;
    }


    *nVec << vertex;
    updatePreference(nVec,edges->count());

}

// gen ranodm double with 2 precision
double Algorithms::genRandom(){
    double main = qrand() % 100;
    return main + (( qrand() % 100 ) / 100 );
}

QVector<Node*> Algorithms::getNeighbours(Node *n){

    QList<Edge*>::QList<Edge*> tempList = *(n->getList());
    QVector<Node*>::QVector<Node *> neighbours;
    neighbours = QVector<Node*>::QVector<Node*>(tempList.length());
    QList<Edge*>::const_iterator i;

    for(i = tempList.constBegin(); i != tempList.constEnd(); ++i){
        int index = 0;
        neighbours[index] = (*i)->destNode();
        index++;
    }

    return neighbours;

}

void Algorithms::updatePreference(QVector<Node*> * nVec, int totalDegree){
    int prefCumulative = 0;

    foreach (Node * n, *nVec){

        double tempLength = (double) n->getList()->length();
        double tempPref = tempLength / (double) totalDegree;
        double tempCumPref = tempPref + prefCumulative;
        n->setPref( tempPref );
        n->setCumPref(tempCumPref);
        prefCumulative= tempCumPref;

    }

}

Node* Algorithms::getPref(QVector<Node*>* nVec, double genPref){
    int start = 0;
    int end = nVec->count();
    bool found = false;
    Node* retNode;

    // only one vertex exists
    if(end == 1){
        retNode = *(nVec->begin());
        found = true;
    }
    while(!found){
        int avg = (start+end)/2;
        Node* temp1 = nVec->at(avg);

        if(avg == end){
            retNode = temp1;
            found = true;
        }
        else{ // (1)
            Node* temp2 = nVec->at(avg+1);
            double pref1 = temp1->getPref();
            double pref2 = temp2->getPref();
            if(pref1 <= genPref && genPref < pref2){
                found = true;
                retNode = temp1;
            }
            else if (genPref > pref1){
                start = avg;
            }
            else{
                end = avg;
            }
        } // end of (1)
    } // end of while

    return retNode;
}

bool Algorithms::edgeExsist(int sourceTag, int destTag, QList<Edge *>* edges){

    QList<Edge*>::const_iterator i;
    bool exsist = false;

    for(i = edges->constBegin(); i != edges->constEnd(); ++i){

        Edge* e = *i;
        int sTag = e->sourceNode()->tag();
        int dTag = e->destNode()->tag();

        if((sTag == sourceTag && dTag == destTag  )
        ||(dTag == sourceTag && sTag == destTag )){
            exsist = true;
            break;
        }

    }

    return exsist;

}
