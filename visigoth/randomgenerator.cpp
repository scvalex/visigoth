#include "edge.h"
#include "graphwidget.h"
#include "node.h"
#include "randomgenerator.h"

#include <cmath>

RandomGenerator::RandomGenerator(GraphWidget *parentGraph) :
    graph(parentGraph)
{
}

void RandomGenerator::populate(QVector<Node*> &nodes, QList<Edge*> &edges) {
    const int NUM_NODES = 50;
    const int NUM_EDGES = 150;
    nodes.clear();
    for (int i(0); i < NUM_NODES; ++i) {
        Node *node = new Node(graph);
        nodes << node;
    }
    edges.clear();
    int a = 0;
    for (int i(0); i < NUM_EDGES; ++i) {
        int b = a;
        while (a == b)
            b = qrand() % NUM_NODES;

        qDebug("creating edge betwee %d and %d", a ,b);
        Edge *edge = new Edge(nodes[a], nodes[b]);
        edges << edge;

        //if (qrand() % int(pow(1.2, a)) == 0)
        a = (a+1) % NUM_NODES;
    }
}
