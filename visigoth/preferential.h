#ifndef PREFERENTIAL_H
#define PREFERENTIAL_H

#include "edge.h"
#include "node.h"

#include <QList>
#include <QMap>
#include <QVector>

class GraphScene;
class Preferential;

class Preferential {
public:
    Preferential(GraphScene *graph);

    // public for demo purpose, will be made private later
    // using preferential attachment
    void addVertex(int edgesToAdd, double p);
    QVector<Node*>* cloneVector(QVector<Node*> *nVec);
    bool edgeExists(int sourceTag, int destTag, QList<Edge*> *edges);

protected:
    // genPef is a randomly generated number satisfing 0 <= genPref < 100
    void addNewEdges(int edgesToAdd,
                     Node *vertex, QVector<Node*> neighbours,
                     QList<Node*> &usedNodes);
    double genRandom();
    QVector<Node*> getIntersection(QVector<Node*> vec1, QVector<Node*> vec2);
    Node* getPreference(const QVector<Node*> &nodes, double genPref);
    void updatePreference(const QVector<Node*> &nodes, int numEdges);

private:
    GraphScene *graph;

    // used for selecting a node by preferential seleciton
    QMap<int, double> cumulativePreferences;
    // used for display purposes only
    QMap<int, double> preferences;
};

#endif // PREFERENTIAL_H
