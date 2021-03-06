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

namespace Ui {
    class PreferentialControl;
};

class Preferential : public Algorithm {
    Q_OBJECT

public:
    Preferential(GraphScene *graph);
    virtual ~Preferential();

    int getNumNodes() const;
    int getNodeDegree() const;

    void reset();
    bool canAddVertex();
    void addVertex();
    QWidget* controlWidget(QWidget *parent = 0);

protected slots:
    void onSizeChanged(int newSize);
    void onDegreeChanged(int newDegree);

protected:
    void addVertex(bool saveSize = false);
    void addVertex(int edgesToAdd, double p);

    // genPef is a randomly generated number satisfing 0 <= genPref < 100
    void addNewEdges(int edgesToAdd,
                     Node *vertex, QVector<Node*> neighbours,
                     QList<Node*> &usedNodes);
    double genRandom();
    QVector<Node*> getIntersection(QVector<Node*> vec1, QVector<Node*> vec2);
    Node* getPreference(const QVector<Node*> &nodes, double genPref);
    void updatePreference(const QVector<Node*> &nodes, int numEdges);

    void updateUI();

private:
    static const int START_NODES = 300;
    static const int START_DEGREE = 3;

    GraphScene *graph;
    QWidget *ctlW;
    Ui::PreferentialControl *preferentialCtl;

    // used for selecting a node by preferential seleciton
    QMap<int, double> cumulativePreferences;
    // used for display purposes only
    QMap<int, double> preferences;

    int size;
    int nodeDegree;
};

#endif // PREFERENTIAL_H
