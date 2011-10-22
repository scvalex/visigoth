#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QSet>
#include <QVector>

class Edge;
class GraphWidget;
class Node;

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(GraphWidget *parent = 0);

    QVector<Node*>& nodes();
    QList<Edge*>& edges();

    bool doesEdgeExist(Node *source, Node *dest);

    Node* newNode();
    bool newEdge(Node *source, Node *dest);

    void reset();

    void itemMoved();

private:
    QVector<QSet<int> > hasEdge;
    QVector<Node*> myNodes;
    QList<Edge*> myEdges;
    GraphWidget *view;
};

#endif // GRAPHSCENE_H
