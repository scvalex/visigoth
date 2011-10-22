#include "edge.h"
#include "graphscene.h"
#include "graphwidget.h"
#include "node.h"

GraphScene::GraphScene(GraphWidget *parent) :
    QGraphicsScene(parent),
    view(parent)
{
}

void GraphScene::reset() {
    clear();
    hasEdge.clear();
    myEdges.clear();
    myNodes.clear();
}

QVector<Node*>& GraphScene::nodes() {
    return myNodes;
}

QList<Edge*>& GraphScene::edges() {
    return myEdges;
}

bool GraphScene::addNewEdge(Node *source, Node *dest) {
    Q_ASSERT(source != 0);
    Q_ASSERT(dest != 0);
    if (doesEdgeExist(source, dest)) {
        return false;
    }
    Edge *edge = new Edge(source, dest);
    if (source->tag() >= hasEdge.size()) {
        hasEdge.resize(source->tag() + 1);
    }
    if (dest->tag() >= hasEdge.size()) {
        hasEdge.resize(dest->tag() + 1);
    }
    hasEdge[source->tag()].insert(dest->tag());
    hasEdge[dest->tag()].insert(source->tag());
    addItem(edge);
    myEdges << edge;
    return true;
}

Node* GraphScene::addNode(Node *node) {
    addItem(node);
    myNodes << node;
    return node;
}

bool GraphScene::doesEdgeExist(Node *source, Node *dest) {
    int sourceTag = source->tag();
    int destTag = dest->tag();

    if (0 <= sourceTag && sourceTag < hasEdge.size() &&
        0 <= destTag && destTag < hasEdge.size())
    {
        return hasEdge[sourceTag].contains(destTag) ||
               hasEdge[destTag].contains(sourceTag);
    }
    return false;
}

void GraphScene::itemMoved() {
    view->itemMoved();
}
