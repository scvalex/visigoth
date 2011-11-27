#include "vtools.h"
#include "edge.h"
#include "graphscene.h"
#include "glgraphwidget.h"
#include "node.h"
#include "preferential.h"
#include "bipartite.h"
#include "erdosrenyi.h"
#include "statistics.h"
#include "barabasialbert.h"

GraphScene::GraphScene(GLGraphWidget *parent) :
    algo(0),
    stats(0),
    view(parent),
    degreeCount(1),
    running(false)
{
    myAlgorithms["Preferential Attachament"] = PREFERENTIAL_ATTACHAMENT;
    myAlgorithms["Bipartite Model"] = BIPARTITE_MODEL;
    myAlgorithms["Erdos Renyi"] = ERDOS_RENYI;
    myAlgorithms["Barabasi Albert"] = BARABASI_ALBERT;
}

QList<QString> GraphScene::algorithms() const {
    return myAlgorithms.keys();
}

void GraphScene::reset() {
    //clear();
    hasEdge.clear();
    myEdges.clear();
    myNodes.clear();
    degreeCount.clear();
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

    myEdges << edge;
    updateDegreeCount(source);
    updateDegreeCount(dest);

    return true;
}

// used only by the algorithms
Node* GraphScene::newNode() {
    Node *node = new Node(this);

    myNodes << node;

    return node;
}

bool GraphScene::doesEdgeExist(Node *source, Node *dest) const {
    int sourceTag = source->tag();
    int destTag = dest->tag();

    if (0 <= sourceTag && sourceTag < hasEdge.size() &&
        0 <= destTag && destTag < hasEdge.size()) {
        // We consider an edge always to be connected to itself
        return hasEdge[sourceTag].contains(destTag) ||
               hasEdge[destTag].contains(sourceTag) ||
               source->tag() == dest->tag();
    }
    return false;
}

void GraphScene::itemMoved() {
    running = true;
    view->itemMoved();
}

void GraphScene::chooseAlgorithm(const QString &name) {
    if (algo) {
        delete algo;
        algo = 0;
    }
    algoId = myAlgorithms[name];
    repopulate();

    emit algorithmChanged(algo);
}

void GraphScene::repopulate() {
    reset();
    if (!algo) {
        switch (algoId) {
        case BIPARTITE_MODEL:
            algo = new Bipartite(this);
            break;
        case PREFERENTIAL_ATTACHAMENT:
            algo = new Preferential(this);
            break;
        case ERDOS_RENYI:
            algo = new ErdosRenyi(this);
            break;
        case BARABASI_ALBERT:
            algo = new BarabasiAlbert(this);
            break;
        }
    }
    algo->reset();
    randomizePlacement();
}

Algorithm* GraphScene::algorithm() const {
    return algo;
}

void GraphScene::randomizePlacement() {
    foreach (Node *node, nodes()) {
        node->setPos(VPointF((qrand() % 1000) - 500,
                              (qrand() % 600) - 300,
                              //(qrand() % 600) - 300));
                              0.0));
    }

    // Why do we recalculate the metrics now? Do they depend on node positions?
    calculateMetrics();
}

void GraphScene::addVertex() {
    algo->addVertex();

    calculateMetrics();
}

// Pre: degree is valid
QList<Node *> GraphScene::getDegreeList(int degree) const {
    return degreeCount[degree - 1];
}

// Pre: Node has just been given a new edge
void GraphScene::updateDegreeCount(Node *node) {
    int degree = node->edges().size();

        if(degree > degreeCount.size())
        degreeCount.resize(degree);

    degreeCount[degree - 1].append(node);

    if(degree > 1)
         degreeRemove(node);
}

void GraphScene::calculateMetrics() {
    if(!stats)
        stats = new Statistics(this);

    // Do something with the metrics
}

void GraphScene::calculateForces() {
    QuadTree quadTree(graphCube().longestEdge());
    foreach (Node* node, nodes()) {
        quadTree.addNode(*node);
    }

    // Don't move the first node
    bool first = true;
    foreach (Node* node, nodes()) {
        if (first) {
            first = false;
            continue;
        }

        node->calculatePosition(quadTree.root());
    }

    running = false;
    foreach (Node *node, nodes()) {
        if (node->advance()) {
            running = true;
        }
    }
}

bool GraphScene::isRunning() const {
    return true;
}

int GraphScene::maxDegree() const {

    return degreeCount.count();
}

int GraphScene::nodeCount(int degree) const {
    /*
    actually it is degree is one less than the degree we are looking for
    But since this is only used by statistics.cpp it does not matter
    */
    return degreeCount[degree].size();
}

void GraphScene::degreeRemove(Node *n) {
    int degree = n->edges().size();

    QList<Node*> list = degreeCount[degree-2];
    int counter = 0;
    foreach(Node *n2, list){

        if(n2->tag() == n->tag()){
            degreeCount[degree-2].removeAt(counter);
            break;
        }

        ++counter;
    }
}

VCubeF GraphScene::graphCube() {
    VPointF p1 = VPointF(0.0);
    VPointF p2 = VPointF(0.0);

    foreach (Node *n, myNodes) {
        if (n->pos().x < p1.x)
            p1.x = n->pos().x;
        if (n->pos().x > p2.x)
            p2.x = n->pos().x;

        if (n->pos().y < p1.y)
            p1.y = n->pos().y;
        if (n->pos().y > p2.y)
            p2.y = n->pos().y;

        if (n->pos().z < p1.z)
            p1.z = n->pos().z;
        if (n->pos().z > p2.z)
            p2.z = n->pos().z;
    }

    return VCubeF(p1, p2);
}
