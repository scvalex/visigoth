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

void FrancescoGenerator::populate(QVector<Node*> &nodes, QList<Edge*> &edges) {
    const int NUM_NODES = 50;

    nodes.clear();
    edges.clear();
    for (int i(0); i < NUM_NODES; ++i) {
        Node *node = new Node(i, graph);
        nodes << node;
    }

    for (int node(0); node < NUM_NODES; ++node)	{
        if (node > 0) {
            int other;
            do {
                other = qrand() % node;

                Edge *edge = new Edge(nodes[node], nodes[other]);
                edges << edge;
            } while (qrand() % 100 > density);
        }
    }
}
