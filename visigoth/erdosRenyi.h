#ifndef ERDOSRENYI_H
#define ERDOSRENYI_H

#include "algorithm.h"
#include "edge.h"
#include "node.h"

#include <QList>
#include <QMap>
#include <QVector>
#include <QtCore/qmath.h>

class GraphScene;
class ErdosRenyi;

class ErdosRenyi : public Algorithm {
public:
    ErdosRenyi(GraphScene *scene);

    void init(int size);
    void addVertex();

    void init(int size, double probability);

private:
    GraphScene *scene;
};

#endif // ERDOSRENYI_H
