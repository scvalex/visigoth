#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include "graphgenerator.h"

class Edge;
class GraphWidget;
class Node;

class RandomGenerator : public GraphGenerator
{
public:
    RandomGenerator(GraphWidget *parentGraph);

    int populate();

private:
    GraphWidget *graph;
};

#endif // RANDOMGENERATOR_H
