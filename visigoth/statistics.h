#ifndef STATISTICS_H
#define STATISTICS_H

#include "edge.h"
#include "node.h"


#include <QList>
#include <QMap>
#include <QVector>
#include <QtCore/qmath.h>


class GraphScene;

class Statistics {

public:

    Statistics(GraphScene* scene);

    double averageDegree();
    double averageLength();
    double clusteringAvg();
    double clusteringCoeff(Node *node);
    double clusteringDegree(int degree);
    double powerLawExponent();
    //double smallWorldIndex();

private:
    GraphScene* graph;

    //QVector<int> shortestDistance();
    double lengthSum(Node *s);
    QVector<Node*> buildNeighbourVector(Node *n);
    int intersectionCount(QVector<Node*> vec1, QVector<Node*> vec2);

};

#endif // STATISTICS_H
