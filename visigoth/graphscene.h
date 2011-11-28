#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QSet>
#include <QVector>

#include "vtools.h"

class Edge;
class Node;
class Algorithm;
class Statistics;


class GraphScene : public QObject
{
    Q_OBJECT
public:
    explicit GraphScene();
    ~GraphScene();

    QVector<Node*>& nodes();
    QList<Edge*>& edges();
    int maxDegree() const;
    // returns the number of nodes with degree "degree"
    int nodeCount(int degree) const;
    void degreeRemove(Node *n);

    bool doesEdgeExist(Node *source, Node *dest) const;

    Node* newNode();
    bool newEdge(Node *source, Node *dest);

    Algorithm* algorithm() const;

    QList<Node*> getDegreeList(int degree) const;

    bool calculateForces();
    void reset();

    QList<QString> algorithms() const;

    VCubeF graphCube();
    Statistics* getStatistics();

public slots:
    void addVertex();
    void randomizePlacement();
    void repopulate();
    void chooseAlgorithm(const QString &name);

signals:
    void algorithmChanged(Algorithm *newAlgo);
    void repopulated();

protected:
    void updateDegreeCount(Node *node);

private:
    enum ALGOS {
        BIPARTITE_MODEL,
        PREFERENTIAL_ATTACHAMENT,
        ERDOS_RENYI,
        BARABASI_ALBERT,
        TWITTER
    };
    Algorithm *algo;
    Statistics *stats;
    int algoId;
    QVector<QSet<int> > hasEdge;
    QVector<Node*> myNodes;
    QList<Edge*> myEdges;
    QVector<QList<Node*> > degreeCount;
    QMap<QString, int> myAlgorithms;
};

#endif // GRAPHSCENE_H
