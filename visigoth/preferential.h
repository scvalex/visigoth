#ifndef PREFERENTIAL_H
#define PREFERENTIAL_H

#include "algorithm.h"
#include "edge.h"
#include "node.h"

#include <QList>
#include <QMap>
#include <QObject>
#include <QVector>

class GraphScene;
class Preferential;
class QWidget;

class Preferential : public Algorithm {
    Q_OBJECT

public:
    Preferential(GraphScene *graph);

    void init(int size);
    void addVertex();
    QWidget *newControlWidget(QWidget *parent = 0);

protected:
    void addVertex(int edgesToAdd, double p);

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
