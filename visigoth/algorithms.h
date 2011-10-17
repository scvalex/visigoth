#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QVector>
#include <QList>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "edge.h"
#include "graphwidget.h"
#include "node.h"

class Algorithms{

public:

    // public for demo purpose, will be made private later
    // using preferential attachment
   static void addVertex(GraphWidget* graph,int maxNeighbourhoodSize,double p);
   static void updatePreference(QVector<Node*>* nVec, int totalEdges);
   static bool edgeExsist(int sourceTag, int destTag, QList<Edge*>* edges);

/* all public for debug purposes
private:
*/
    // genPef is a randomly generated number satisfing 0 <= genPref < 100
   static Node* getPref(QVector<Node*>* nVec, double genPref);
   static QVector<Node *> getNeighbours(Node* n);
   static double genRandom();


};

#endif // ALGORITHMS_H
