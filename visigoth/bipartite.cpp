#include "bipartite.h"

Bipartite::Bipartite(GraphWidget *graph) : graph(graph){

    generated = false;
}

void Bipartite::genBipartite(int vSize, int uSize){

    // initialisation

    vVector = *(new QVector<Node *>(vSize));
    uVector = *(new QVector<Node *>(uSize));

    for(int i(0); i < uSize ; ++i){
        Node * u = new Node(graph);
        uVector[i] = u;
    }

    for(int i(0); i < vSize ; ++i){
        Node * v = new Node(graph);
        vVector[i] = v;
    }

    // Use class edgeList

    updatePreference();


    // have to edit this for case uSize = 1
    for(int i(0); i < vSize ; ++i ){
        Node * v = vVector[i];
        QList<Node *>* usedNodes = new QList<Node *>();

        int n = 0;
        double degree = degreeDist(i);
        int cutoff = 0;

        while( (n < degree ) && usedNodes->count() < uSize  && cutoff < 100){

            // may have to implement check for infinite looping
            double rand = fmod(qrand(),cumulativePreferences[uSize-1]);
            Node * u = getPreference(rand);

            if(!graph->doesEdgeExist(u->tag(),v->tag())){
                *usedNodes << u;
                Edge * e = new Edge(u,v);
                graph->addNewEdge(e);
                n+=1;
            }
            ++cutoff;
        }


        usedNodes->clear();

    }

    for(int i(0); i < uSize; ++i){
        Node * u = uVector[i];
        // if not connected
        if(u->edges().count() == 0){

            Node * v = vVector[qrand()%vSize];
            Edge * uv = new Edge(u,v);
            graph->addNewEdge(uv);

        }
    }

    for(int i(0); i < vSize ; ++i){
        Node * v = vVector[i];

        QList<Edge *> * A = new QList<Edge *>();

        // Find all edges where v is the dest Node
        foreach(Edge * e, *edgeList){
            if(e->destNode()->tag() == v->tag() ){
                *A << e;
            }
        }

        foreach (Edge * e1, *A){
            foreach(Edge * e2, *A){

                Node * u1 = e1->sourceNode();
                Node * u2 = e2->sourceNode();

                if(u1->tag() != u2->tag()){

                    Edge * uu = new Edge(u1,u2);
                        graph->addNewEdge(uu);
                }
            }
        }
    }

    generated = true;
}

void Bipartite::showBipartite(){

    if(!generated){
        return;
    }

    int vSize = vVector.count();
    int uSize = uVector.count();

    for(int i(0); i < vSize ; ++i){
        graph->addNode(vVector.at(i));
    }

    for(int i(0); i < uSize ; ++i){
        graph->addNode(uVector.at(i));
    }

    vVector.clear();
    uVector.clear();

}

bool Bipartite::returnGenerated(){
    return generated;
}

void Bipartite::setGenerated(bool b){
    generated = b;
}

double Bipartite::fitnessDist(int x){

    if( x == 0){
        return 0;
    }

    return 1/qPow(x,3);

}

// Ceil because we can not have fractional degrees
double Bipartite::degreeDist(int x){

    if( x == 0){
        return 1;
    }

    return qCeil(qPow((double)x,(qLn(3)/qLn(2)) - 1));

}

// Return the preferred node, using binary search.
Node* Bipartite::getPreference(double genPref) {

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
    return uVector[i];
}

void Bipartite::updatePreference() {
    double prefCumulative = 0;

    if (uVector.count() == 1) {
        cumulativePreferences[0] = 1;
        return;
    }

    for(int i(0); i < uVector.count(); ++i) {

        Node * node = uVector[i];
        cumulativePreferences[node->tag()] = prefCumulative;
        prefCumulative += fitnessDist(node->tag() + 1);
    }
}



