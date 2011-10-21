#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "edge.h"
#include "graphwidget.h"
#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <QList>
#include <QVector>

class Algorithms {
public:
    // public for demo purpose, will be made private later
    // using preferential attachment
    static void addVertex(GraphWidget *graph, int edgesToAdd, double p);
    static QVector<Node*>* cloneVector(QVector<Node*> *nVec);
    static bool edgeExists(int sourceTag, int destTag, QList<Edge*> *edges);
    static void updatePreference(QVector<Node*> *nVec, int totalEdges);

private:
    // genPef is a randomly generated number satisfing 0 <= genPref < 100
    static void addNewEdges(GraphWidget *graph, int edgesToAdd,
                            Node *vertex, QVector<Node*> *neighbours,
                            QList<Node*> *usedNodes);
    static double genRandom();
    static QVector<Node*>* getIntersection(QVector<Node*> *vec1, QVector<Node*> *vec2);
    static QVector<Node*>* getNeighbours(Node *n);
    static Node* getPref(QVector<Node*> *nVec, double genPref);
    static void restoreTags(QVector<Node*> *nVec);
};

#endif // ALGORITHMS_H
