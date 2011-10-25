#include "edge.h"
#include "graphscene.h"
#include "graphwidget.h"
#include "node.h"
#include "preferential.h"

GraphScene::GraphScene(GraphWidget *parent) :
    QGraphicsScene(parent),
    view(parent),
    algo(0)
{
}

void GraphScene::reset() {
    clear();
    hasEdge.clear();
    myEdges.clear();
    myNodes.clear();
    Node::reset();
    //FIXME also free nodes and edges
}

QVector<Node*>& GraphScene::nodes() {
    return myNodes;
}

QList<Edge*>& GraphScene::edges() {
    return myEdges;
}

bool GraphScene::newEdge(Node *source, Node *dest) {
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

Node* GraphScene::newNode() {
    Node *node = new Node(this);
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

void GraphScene::populate() {
    algo = new Preferential(this);
    for (int i(0); i < 100; ++i) {
        addVertex();
    }
}

void GraphScene::randomizePlacement() {
    foreach (Node *node, nodes()) {
        node->setPos(10 + qrand() % 1000, 10 + qrand() % 600);
    }
    foreach (Edge *edge, edges()) {
        edge->adjust();
    }
}

void GraphScene::addVertex() {
    algo->addVertex((qrand() % 3 ) + 1, qrand() % 100);
}

