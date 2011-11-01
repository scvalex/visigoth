#include "edge.h"
#include "graphscene.h"
#include "abstractgraphwidget.h"
#include "node.h"
#include "preferential.h"
#include "bipartite.h"

GraphScene::GraphScene(AbstractGraphWidget *parent) :
    //QGraphicsScene(parent),
    algo(0),
    algoId(0),
    targetNumNodes(1000),
    view(parent)
{
}

void GraphScene::reset() {
    clear();
    hasEdge.clear();
    foreach (Node* node, myNodes) {
        delete node;
    }
    foreach (Edge* edge, myEdges) {
        delete edge;
    }
    myEdges.clear();
    myNodes.clear();
    Node::reset();
    if (algo) {
        delete algo;
    }
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

void GraphScene::repopulate() {
    reset();
    switch (algoId) {
    case 0:
        algo = new Preferential(this);
        break;
    case 1:
        algo = new Bipartite(this);
        break;
    }
    algo->init(targetNumNodes);
}

void GraphScene::nextAlgorithm() {
    algoId = (algoId + 1) % 2;
    repopulate();
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
    algo->addVertex();
}
