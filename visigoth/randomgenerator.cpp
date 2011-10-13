#include "edge.h"
#include "graphwidget.h"
#include "node.h"
#include "randomgenerator.h"

RandomGenerator::RandomGenerator(GraphWidget *parentGraph) :
    graph(parentGraph)
{
}

void RandomGenerator::populate(QVector<Node*> &nodes, QList<Edge*> &edges) {
    const int NUM_NODES = 60;
    const int NUM_EDGES = 200;
    nodes.clear();
    for (int i(0); i < NUM_NODES; ++i) {
        Node *node = new Node(graph);
        nodes << node;
    }
    edges.clear();
    for (int i(0); i < NUM_EDGES; ++i) {
        int a = qrand() % NUM_NODES;
        int b = a;
        while (a == b)
            b = qrand() % NUM_NODES;
        Edge *edge = new Edge(nodes[a], nodes[b]);
        edges << edge;
    }
}
