#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QMap>
#include <QSet>
#include <QVector>

#include "abstractgraphwidget.h"

class Edge;
class GraphWidget;
class Node;
class Algorithm;
class Statistics;

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(AbstractGraphWidget *parent = 0);

    QVector<Node*>& nodes();
    QList<Edge*>& edges();
    int maxDegree() const;
    // returns the number of nodes with degree "degree"
    int nodeCount(int degree) const;
    void degreeRemove(Node *n);

    bool doesEdgeExist(Node *source, Node *dest) const;

    Node* newNode();
    bool newEdge(Node *source, Node *dest);

    void repopulate();
    Algorithm* algorithm() const;

    void itemMoved();

    void randomizePlacement();

    void addVertex();

    QList<Node*> getDegreeList(int degree) const;

    void calculateMetrics();

    void calculateForces();
    bool isRunning() const;
    void reset();

    QList<QString> algorithms() const;

public slots:
    void chooseAlgorithm(const QString &name);

signals:
    void algorithmChanged(Algorithm *newAlgo);

protected:
    void updateDegreeCount(Node *node);

private:
    enum ALGOS {
        BIPARTITE_MODEL,
        PREFERENTIAL_ATTACHAMENT,
        ERDOS_RENYI
    };

    Algorithm *algo;
    Statistics *stats;
    int algoId;
    QVector<QSet<int> > hasEdge;
    QVector<Node*> myNodes;
    QList<Edge*> myEdges;
    AbstractGraphWidget *view;
    QVector<QList<Node*> > degreeCount;
    bool running;
    QMap<QString, int> myAlgorithms;

    // Metrics
    QVector<double> metricVector;
};

#endif // GRAPHSCENE_H
