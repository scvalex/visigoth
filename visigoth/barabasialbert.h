#ifndef BARABASIALBERT_H
#define BARABASIALBERT_H

#include "algorithm.h"
#include "edge.h"
#include "node.h"

#include <QList>
#include <QMap>
#include <QVector>

class GraphScene;
class Barabasialbert;

class Barabasialbert : public Algorithm {
public:
    Barabasialbert(GraphScene *graph);

    void init(int size);
    void addVertex();

protected:
    void addVertex(int edgesToAdd, double p);

    double genRandom();
    Node* getPreference(const QVector<Node*> &nodes, double genPref);
    void updatePreference(const QVector<Node*> &nodes, int numEdges);

private:
    GraphScene *graph;

    // used for selecting a node by preferential seleciton
    QMap<int, double> cumulativePreferences;
    // used for display purposes only
    QMap<int, double> preferences;
};

#endif // BARABASIALBERT_H
