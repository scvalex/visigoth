#include "statistics.h"
#include "graphscene.h"

#include <QQueue>
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
        allLengths += lengthSum(n);
        foreach (Node *m, graph->nodes()) {
            m->setDistance(0);
            m->setVisited(false);
        }
    }

    return allLengths / (double) (graph->nodes().size()*(graph->nodes().size() - 1));
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

double Statistics::lengthSum(Node *s) {
    QQueue<Node*> queue;
    queue.append(s);
    double retLength = 0;
    s->setVisited(true);

    // Find the distances to all other nodes using breadth first search
    while(!queue.empty()) {
        Node *parent = queue.dequeue();
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
                queue.enqueue(n);
            }
        }

        retLength += parent->getDistance();
    }

    return retLength;
}

int Statistics::intersectionCount(QVector<Node*> vec1, QVector<Node*> vec2) {
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

    return retVec.count();
}


double Statistics::powerLawExponent() {
    //Made a list here incase we want to plot the data in a widget.
    QList<QPointF> plot;

    int logCounter = 0;
    double x(0);

    int maxDegree = graph->maxDegree();
    for(double i(0); x < maxDegree ; ++i){
        x = (i+1)*qPow(10,logCounter);
        if(x >=maxDegree){
            break;
        }

        double count = graph->nodeCount(x);
        double y =  count/(double) graph->nodes().size();


        if( y != 0 && x!= 1 ) {
            // in case we want to plot
            QPointF p(qLn(x), qLn(y));
            plot << p;
        }

        if(i == 9){
            ++logCounter;
            i=0;
        }

    }

    double deltaY = 0.0;
    double deltaX = 0.0;
    int c = 0;

    foreach(QPointF p, plot) {
        // init calculation
        if(c == 0){
            //yPrev = p.getY();
            deltaY = p.y();
            deltaX = p.x();
        } else if (c == plot.size() -1) {
            deltaY = p.y() - deltaY;
            deltaX = p.x() - deltaX;
        }
        ++c;
    }

    return -1*(deltaY/deltaX);
}
