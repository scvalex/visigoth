#include "graphscene.h"
#include "erdosrenyi.h"

#include <QtCore/qmath.h>

ErdosRenyi::ErdosRenyi(GraphScene *scene) :
    Algorithm(scene),
    scene(scene),
    size(60),
    probability(0.4)
{
}

ErdosRenyi::~ErdosRenyi() {
}

void ErdosRenyi::addVertex() {
    qDebug("Cannot add vertex to Erdos-Renyi graph");
}

void ErdosRenyi::reset() {
    QVector<Node*> nodesVector(size);

    for (int i(0); i < size; ++i) {
        Node *node = scene->newNode();
        nodesVector[i] = node;
    }

    for (int i(0); i < size; ++i) {
        for (int j(i+1); j < size; ++j) {
            int rand = qrand() % 2;
            if ( rand < probability ) {
                scene->newEdge(nodesVector[i],nodesVector[j]);
            } else {
                continue;
            }
        }
    }
}

QWidget* ErdosRenyi::controlWidget(QWidget *parent) {
    return 0;
}
