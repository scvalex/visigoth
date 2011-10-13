#ifndef GRAPHGENERATOR_H
#define GRAPHGENERATOR_H

#include <QVector>
#include <QList>

class Edge;
class Node;

class GraphGenerator
{
public:
    virtual void populate(QVector<Node*> &nodes, QList<Edge*> &edges) = 0;
};

#endif // GRAPHGENERATOR_H
