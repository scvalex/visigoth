#include "algorithms.h"

// Adding vertex using preferential attachment with clustering
void Algorithms::addVertex(GraphWidget *graph, int edgesToAdd, double p){

    double random;
    Node * vPref;
    QVector<Node*>* nVec = graph->getNodeVector();
    Node* vertex = new Node(nVec->count(), graph);
    QList<Edge *>* edges = graph->getEdgeList();
    QVector<Node*>* neighbours;
    QList<Node *>* usedNodes = new QList<Node*>();

    while (edgesToAdd != 0){


        do{
            random = genRandom();
            vPref = getPref(nVec,random);
        }
        while(edgeExsist(vertex->tag(),vPref->tag(),edges));


        Edge* e = new Edge(vertex,vPref);
        graph->addEdgeToScene(e);
        *edges << e;
        --edgesToAdd;

        usedNodes->append(vPref);

        if(genRandom() < p){

            neighbours = getNeighbours(vPref);
            addNewEdges(graph,edgesToAdd,vertex,neighbours,usedNodes);

        }


        if(usedNodes->count() == nVec->count()){
            edgesToAdd = 0;
        }

    }


    *nVec << vertex;
    graph->addNodeToScene(vertex);
    // debug
    int debug1 = edges->count();
    int debug2 = nVec->count();
    updatePreference(nVec,2*edges->count());

}

// subprocedure of addVertex
void Algorithms::addNewEdges(GraphWidget *graph,int edgesToAdd,
                                         Node *vertex, QVector<Node *>* neighbours,
                                         QList<Node*>* usedNodes){

    QList<Edge *>* edges = graph->getEdgeList();
    QVector<Node *>* setUsed = neighbours;
    int length = setUsed->count();

    while(edgesToAdd!= 0 && !(setUsed->empty())){

        int rand = qrand() % length;
        Node * vi = setUsed->at(rand);

        if(!edgeExsist(vertex->tag(),vi->tag(),graph->getEdgeList())){

            Edge * e = new Edge(vertex,vi);
            *edges << e;
            --edgesToAdd;
            graph->addEdgeToScene(e);
            usedNodes->append(vi);
            setUsed = getIntersection(neighbours, getNeighbours(vi));

        }

        else{
            setUsed->remove(rand);
        }

        length = setUsed->count();

    }
}

// gen random double with 2 precision
double Algorithms::genRandom(){
    double main = qrand() % 100;
    return main + (( qrand() % 100 ) / 100 );
}

QVector<Node*>* Algorithms::getNeighbours(Node *n){

    QList<Edge*> tempList = *(n->getList());
    QVector<Node *>* neighbours = new QVector<Node*>();
    QList<Edge*>::const_iterator i;
    int nTag = n->tag();

    for(i = tempList.constBegin(); i != tempList.constEnd(); ++i){
        int sT = (*i)->sourceNode()->tag();

        if(nTag == sT){
            neighbours->append((*i)->destNode());
        }

        else{
            neighbours->append((*i)->sourceNode());
        }
    }

    return neighbours;

}

void Algorithms::updatePreference(QVector<Node*> * nVec, int totalDegree){
    int prefCumulative = 0;

    if(nVec->count() == 1){
        nVec->first()->setPref(100);
        nVec->first()->setCumPref(100);
        return;
    }

    foreach (Node * n, *nVec){

        double tempLength = (double) n->getList()->length();
        double tempPref = (tempLength / (double) totalDegree) * 100;
        double tempCumPref = tempPref + prefCumulative;
        n->setPref( tempPref );
        n->setCumPref(tempCumPref);
        prefCumulative += tempPref;

    }

}

// Returns the preferred node, using binary search.
Node* Algorithms::getPref(QVector<Node*>* nVec, double genPref){
    int start = 0;
    int end = nVec->count()-1;
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

        if( (avg == end ) || (avg == start)){
            retNode = temp1;
            found = true;
        }
        else{ // (1)
            Node* temp2 = nVec->at(avg+1);
            double pref1 = temp1->getCumPref();
            double pref2 = temp2->getCumPref();
            if(pref1 <= genPref && genPref < pref2){
                found = true;
                retNode = temp1;
            }
            else if (genPref > pref1){
                start = avg+1;
            }
            else{
                end = avg -1;
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


        if((sTag == sourceTag && dTag == destTag) || (sourceTag == destTag)){
            exsist = true;
            break;
        }
    }
    return exsist;

}

QVector<Node *>* Algorithms::getIntersection(QVector<Node *>* vec1, QVector<Node *>* vec2){

    QVector<Node *>* retVec = new QVector<Node *>();
    QVector<Node *>* shorterVec;
    QVector<Node *>* longerVec;
    int length;
    if(vec1->count() > vec2->count()){

        length = vec1->count();
        shorterVec = vec2;
        longerVec = vec1;
    }
    else{

       length = vec2->count();
       shorterVec = vec1;
       longerVec = vec2;
    }

    for(int i = 0; i < length; ++i){

        Node * tempPointer = longerVec->at(i);
        if(shorterVec->contains(tempPointer)){
            retVec->append(tempPointer);

        }

    }

    return retVec;
}
