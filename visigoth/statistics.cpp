#include "statistics.h"
#include "graphscene.h"

#include <QPointF>

Statistics::Statistics(GraphScene *scene):
    graph(scene)
{
}

double Statistics::degreeAvg() {
    return (2 * graph->edges().count()) / graph->nodes().count();
}

double Statistics::lengthAvg() {
    double allLengths = 0;

    foreach (Node *n, graph->nodes()) {
        allLengths += shortestPath(n,NULL);
        foreach (Node *m, graph->nodes()) {
            m->setDistance(0);
            m->setVisited(false);
        }
    }

    double res =  allLengths / (double) (graph->nodes().size()*(graph->nodes().size() - 1));
    return res;
}

double Statistics::clusteringAvg() {
    double clusterCumulative = 0.0;

    foreach (Node *n, graph->nodes()) {
        clusterCumulative += clusteringCoeff(n);
    }

    double res = clusterCumulative / (double)graph->nodes().size();


    return res;
}

double Statistics::clusteringCoeff(Node *node) {
    QList<Edge*> edges = node->edges();
    int k = edges.count();
    int intersection = 0;

    while(!edges.empty()) {
        Edge *e = edges.takeFirst();
        Node *src = e->sourceNode();
        Node *dest = e->destNode();

        QVector<Node*> nNeigh = buildNeighbourVector(node);

        if(src->tag() == node->tag()) {
            QVector<Node*> dNeigh = buildNeighbourVector(dest);
            intersection += intersectionCount(nNeigh, dNeigh);
        } else {
            QVector<Node*> dNeigh = buildNeighbourVector(dest);
            intersection += intersectionCount(nNeigh, dNeigh);
        }
    }

    double debug = k > 1 ? (2*intersection)/(double) (k*(k-1)) : 0 ;

    return k > 1 ? (2*intersection)/(double) (k*(k-1)) : 0 ;
}

double Statistics::clusteringDegree(int degree) {
    QList<Node*> nodeList = graph->getDegreeList(degree);
    int degreeCount = nodeList.count();
    int clusterCumulative = 0;

    foreach (Node *n, nodeList) {
        clusterCumulative += clusteringCoeff(n);
    }

    return clusterCumulative / degreeCount;
}

// private:

QVector<Node*> Statistics::buildNeighbourVector(Node *n) {
    QList<Edge*> eList = n->edges();
    QVector<Node*> retVec(eList.size());

    while(!eList.empty()) {
        Edge *e = eList.takeFirst();

        if(e->sourceNode()->tag() == n->tag()) {
            retVec << e->destNode();
        } else {
            retVec << e->sourceNode();
        }
    }

    return retVec;
}

// d = -1 for shortestPath between all nodes in the network
double Statistics::shortestPath(Node *s, Node *d) {
    QList<Node*> queue;
    queue.append(s);
    double retLength = 0;
    s->setVisited(true);

    // Find the distances to all other nodes using breadth first search
    while(!queue.empty()) {
        Node *parent = queue.first();
        QList<Edge*> edges = parent->edges();

        foreach(Edge *e, edges) {
            Node *n;

            if(e->sourceNode()->tag() == parent->tag()) {
                n = e->destNode();
            } else {
                n = e->sourceNode();
            }

            if(!n->getVisited()) {
                n->setVisited(true);
                n->setDistance(parent->getDistance() + 1);
                queue.append(n);
            }

            if( d != NULL && n->tag() == d->tag()){
                return d->getDistance();
            }
        }

        queue.removeFirst();
        retLength += parent->getDistance();
    }

    return retLength;
}

int Statistics::intersectionCount(QVector<Node*> vec1, QVector<Node*> vec2) {
    QVector<Node*> retVec;
    QVector<Node*> *shorterVec;
    QVector<Node*> *longerVec;

    // tags are unqiue, so no counter is needed
    QMap<int,bool> mapShort;
    QMap<int,bool> mapLong;


    int length;
    int shortLength;
    if (vec1.size() > vec2.size()) {
        length = vec1.size();
        shortLength = vec2.size();
        shorterVec = &vec2;
        longerVec = &vec1;
    } else {
       length = vec2.size();
       shortLength = vec1.size();
       shorterVec = &vec1;
       longerVec = &vec2;
    }

    for (int i(0); i < length; ++i) {

        Node* longPointer;
        Node* shortPointer;

        if( i < shortLength ){

            shortPointer = shorterVec->at(i);
            mapShort.insert(shortPointer->tag(),true);

        }

        longPointer= longerVec->at(i);

        mapLong.insert(longPointer->tag(),true);

        // if it contains its true
        if (mapShort.contains(longPointer->tag())) {
            retVec << longPointer;
            //don'thave to clear maps because tags are unique
        }

    }

    return retVec.size();
}


double Statistics::powerLawExponent() {
    //Made a list here incase we want to plot the data in a widget.
    QList<QPointF> plot;


    int maxDegree = graph->maxDegree();
    for(double i(0); i < maxDegree ; ++i){


        double count = graph->nodeCount(i);
        double y =  count/(double) graph->nodes().size();


        if( y != 0 )
        {
            // incase we want to plot
            QPointF p(qLn(i+1), qLn(y));
            plot << p;
        }

    }

    double deltaY = 0.0;
    double deltaX = 0.0;
    double yPref;
    int c = 0;


    foreach(QPointF p, plot) {
        // init calculation
        if(c == 0){
           //yPrev = p.getY();
            yPref = p.ry();
           deltaX = p.rx();
           ++c;

        }

        else{
            deltaY += p.ry() - yPref;
            yPref = p.ry();
            deltaX = p.rx() - deltaX;
            ++c;
        }


    }

    return -1*(deltaY/deltaX);
}
