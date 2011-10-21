#include "algorithms.h"
#include "edge.h"
#include "graphwidget.h"
#include "node.h"
#include "francescogenerator.h"

#include <stdexcept>
#include <cmath>

FrancescoGenerator::FrancescoGenerator(GraphWidget *parentGraph, int density) :
    graph(parentGraph)
{
    if (density < 0 || density > 100) {
        throw std::invalid_argument("FrancescoGenerator: density must be < 100 && > 0");
    }
    this->density = density;
}

int FrancescoGenerator::populate() {
    const int NUM_NODES = 200;

    QVector<Node*> nodes(NUM_NODES);
    for (int i(0); i < NUM_NODES; ++i) {
        nodes[i] = new Node(i, graph);
        graph->addNode(nodes[i]);
    }

    int numEdges(0);
    for (int node(0); node < NUM_NODES; ++node)	{
        if (node > 0) {
            do {
                if (graph->addNewEdge(new Edge(nodes[node], nodes[qrand() % node])))
                    ++numEdges;
            } while (qrand() % 100 > density);
        }
    }
    return numEdges;
}
