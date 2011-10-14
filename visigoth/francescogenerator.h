#ifndef FRANCESCOGENERATOR_H
#define FRANCESCOGENERATOR_H

#include "graphgenerator.h"

class FrancescoGenerator : public GraphGenerator
{
public:
    FrancescoGenerator(GraphWidget *parentGraph, int density);

    void populate(QVector<Node*> &nodes, QList<Edge*> &edges);

private:
    GraphWidget *graph;
    int density;
};

#endif // FRANCESCOGENERATOR_H
