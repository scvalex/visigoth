#include "graphscene.h"
#include "erdosRenyi.h"

ErdosRenyi::ErdosRenyi(GraphScene *scene) :
    scene(scene)
{
}

void ErdosRenyi::init(int size) {
    init(size, 0.6);
}

void ErdosRenyi::addVertex() {

}

void ErdosRenyi::init(int size, double probability) {
    QVector<Node*> nodesVector(size);

    for (int i(0); i < size; ++i) {
        Node *node = scene->newNode();
        nodesVector[i] = node;
    }

    for (int i(0); i < size; ++i)
        for (int j(i+1); j < size; ++j) {
            int rand = qrand() % 2;
            if ( rand < probability ) {
                scene->newEdge(nodesVector[i],nodesVector[j]);
            } else {
                continue;
            }
        }
}


