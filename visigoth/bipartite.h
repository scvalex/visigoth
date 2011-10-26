#ifndef BIPARTITE_H
#define BIPARTITE_H

#include "edge.h"
#include "node.h"

#include <QList>
#include <QMap>
#include <QVector>
#include <QtCore/qmath.h>

class GraphScene;
class Bipartite;

class Bipartite{

public:
    Bipartite(GraphScene *scene);
    void genBipartite(int vSize, int uSize);
    void showBipartite();
private:

    // Both preference funcs will only be used on vector set U
    int getPreference(double genPref);
    void updatePreference();

    double degreeDist(int x);
    double fitnessDist(int x);
    /*vVector and uVector exist since we have two disjoint sets
      During the graph generation
     */
    QVector<Node *> vVector;
    QVector<Node *> uVector;
    // used for selecting a node by preferential seleciton
    QMap<int, double> cumulativePreferences;
    GraphScene *scene;
};

#endif // BIPARTITE_H
