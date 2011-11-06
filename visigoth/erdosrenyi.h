#ifndef ERDOSRENYI_H
#define ERDOSRENYI_H

#include "algorithm.h"
#include "edge.h"
#include "node.h"

#include <QList>
#include <QMap>
#include <QVector>

class GraphScene;
class QWidget;

class ErdosRenyi : public Algorithm {
    Q_OBJECT
public:
    ErdosRenyi(GraphScene *scene = 0);
    virtual ~ErdosRenyi();

    void reset();
    void addVertex();
    QWidget* controlWidget(QWidget *parent = 0);

private:
    GraphScene *scene;

    int size;
    double probability;
};

#endif // ERDOSRENYI_H
