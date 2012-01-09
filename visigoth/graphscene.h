#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QSet>
#include <QVector>
#include <QColor>
#include <QMainWindow>

#include "vtools.h"

class Edge;
class Node;
class Algorithm;
class Statistics;

class GraphScene : public QObject
{
    Q_OBJECT
public:
    explicit GraphScene(QObject *parent = 0);
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
    void removeNode(Node *n);
    void setAllNodes(int i);
    void removeEdges(int cutoffTag);
    void removeEdge(Node * source, Node* dst);

    Statistics* getStatistics();

    void set3DMode(bool enabled);

    QColor edgeColour();
    QColor nodeColour();
    QColor backgroundColour();

signals:
    void nodeMoved();

public slots:
    void onNodeMoved();
    void addVertex();
    void randomizePlacement();
    void repopulate();
    void chooseAlgorithm(const QString &name);
    void customizeEdgesColour(const QColor &newColour);
    void customizeNodesColour(const QColor &newColour);
    void customizeBackgroundColour(const QColor &newColour);

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
        WATTS_STROGATZ,
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

    QColor myBackgroundColour;
    bool mode3d;

    QColor myEdgeColour;
    QColor myNodeColour;
};

#endif // GRAPHSCENE_H
