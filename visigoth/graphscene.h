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
    explicit GraphScene(QMainWindow *mainWindow);
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

signals:
    void nodeMoved();

public slots:
    void onNodeMoved();
    void addVertex();
    void randomizePlacement();
    void repopulate();
    void chooseAlgorithm(const QString &name);
    void customizeEdgesColor(QColor newColor, bool isColorChanged);
    void customizeNodesColor(QColor newColor, bool isColorChanged);

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
    QColor myEdgeColor;
    QColor myNodeColor;
};

#endif // GRAPHSCENE_H
