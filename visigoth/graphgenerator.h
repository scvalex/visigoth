#ifndef GRAPHGENERATOR_H
#define GRAPHGENERATOR_H

#include <QVector>
#include <QList>

class Edge;
class Node;

class GraphGenerator
{
public:
    // Returns the number of edges generated.
    virtual int populate() = 0;
};

#endif // GRAPHGENERATOR_H
