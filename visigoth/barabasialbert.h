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
class QWidget;

class Barabasialbert : public Algorithm {

    Q_OBJECT

public:
    Barabasialbert(GraphScene *graph);

    void reset();
    void addVertex();
    QWidget* controlWidget(QWidget *parent = 0);

protected slots:
    void onSizeChanged(int newSize);

protected:
    void addVertex(bool saveSize = false);
    void addVertex(int edgesToAdd, double p);

    double genRandom();
    Node* getPreference(const QVector<Node*> &nodes, double genPref);
    void updatePreference(const QVector<Node*> &nodes, int numEdges);

private:
    static const int START_NODES = 2000;
    GraphScene *graph;

    // used for selecting a node by preferential seleciton
    QMap<int, double> cumulativePreferences;
    // used for display purposes only
    QMap<int, double> preferences;

    int size;
};

#endif // BARABASIALBERT_H
