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
    static void updatePreference(QVector<Node*> *nVec, int totalEdges);
    static bool edgeExsist(int sourceTag, int destTag, QList<Edge*> *edges);
    static QVector<Node*>* cloneVector(QVector<Node*> *nVec);

private:
    // genPef is a randomly generated number satisfing 0 <= genPref < 100
    static Node* getPref(QVector<Node*> *nVec, double genPref);
    static QVector<Node *> *getNeighbours(Node *n);
    static double genRandom();
    static void addNewEdges(GraphWidget *graph, int edgesToAdd,
                            Node *vertex, QVector<Node*> *neighbours,
                            QList<Node*> *usedNodes);
    static QVector<Node*>* getIntersection(QVector<Node*> *vec1, QVector<Node*> *vec2);
    static void restoreTags(QVector<Node*> *nVec);
};

#endif // ALGORITHMS_H
