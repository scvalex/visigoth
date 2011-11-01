#ifndef STATISTICS_H
#define STATISTICS_H

#include "edge.h"
#include "node.h"

#include <QList>
#include <QMap>
#include <QVector>

class GraphScene;

class Statistics {

public:

    Statistics(GraphScene * scene);

    double averageDegree();
    double averageLength();
    double clusteringCoeff(Node * node);
    double custeringAvg();
    double clusteringDegree(int degree);
    double smallWorldIndex();

private:

    GraphScene * graph;

    QVector<int> shoretstDistance();
    int intersectionCount(QVector<Node *> vec1, QVector<Node *> vec2);
    double lengthSum(Node * s);
    QVector<Node *> buildNeighbourVector(Node * n);

};

#endif // STATISTICS_H
