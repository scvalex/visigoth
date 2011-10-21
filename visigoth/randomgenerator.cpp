#include "edge.h"
#include "graphwidget.h"
#include "node.h"
#include "randomgenerator.h"

#include <cmath>

RandomGenerator::RandomGenerator(GraphWidget *parentGraph) :
    graph(parentGraph)
{
}

int RandomGenerator::populate() {
    const int NUM_NODES = 50;
    const int NUM_EDGES = 150;

    QVector<Node*> nodes(NUM_NODES);
    for (int i(0); i < NUM_NODES; ++i) {
        nodes[i] = new Node(graph);
        graph->addNode(nodes[i]);
    }

    int a(0);
    int numEdges(0);
    for (int i(0); i < NUM_EDGES; ++i) {
        int b = a;
        while (a == b)
            b = qrand() % NUM_NODES;

        if (graph->addNewEdge(new Edge(nodes[a], nodes[b])))
            ++numEdges;

        //if (qrand() % int(pow(1.2, a)) == 0)
        a = (a+1) % NUM_NODES;
    }

    return numEdges;
}
