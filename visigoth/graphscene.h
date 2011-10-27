#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QSet>
#include <QVector>

#include "abstractgraphwidget.h"

class Edge;
class GraphWidget;
class Node;
class Algorithm;

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(AbstractGraphWidget *parent = 0);

    QVector<Node*>& nodes();
    QList<Edge*>& edges();

    bool doesEdgeExist(Node *source, Node *dest);

    Node* newNode();
    bool newEdge(Node *source, Node *dest);

    void repopulate();
    void nextAlgorithm();
    void modifyNodesNumber(int nodesNB);

    void itemMoved();

    void randomizePlacement();

    void addVertex();

protected:
    void reset();

private:
    Algorithm *algo;
    int algoId;
    QVector<QSet<int> > hasEdge;
    QVector<Node*> myNodes;
    QList<Edge*> myEdges;
    int targetNumNodes;
    AbstractGraphWidget *view;
};

#endif // GRAPHSCENE_H
