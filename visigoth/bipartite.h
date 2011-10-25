#ifndef BIPARTITE_H
#define BIPARTITE_H

#include "edge.h"
#include "graphwidget.h"
#include "node.h"

#include <QList>
#include <QMap>
#include <QVector>
#include <QtCore/qmath.h>

class Bipartite{

public:
    Bipartite(GraphWidget *graph);
    void genBipartite(int vSize, int uSize);
    void showBipartite();
    bool returnGenerated();
    void setGenerated(bool b);

private:

    // Both preference funcs will only be used on vector set U
    Node* getPreference(double genPref);
    void updatePreference();

    double degreeDist(int x);
    double fitnessDist(int x);
    /*vVector and uVector exist since we have two disjoint sets
      During the graph generation
     */
    QVector<Node *> vVector;
    QVector<Node *> uVector;
    QList<Edge *> * edgeList;
    // used for selecting a node by preferential seleciton
    QMap<int, double> cumulativePreferences;
    GraphWidget *graph;
    bool generated;








};

#endif // BIPARTITE_H
