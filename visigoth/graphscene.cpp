#include "edge.h"
#include "graphscene.h"
#include "abstractgraphwidget.h"
#include "node.h"
#include "preferential.h"
#include "bipartite.h"
#include "erdosrenyi.h"
#include "statistics.h"
#include "barabasialbert.h"

#ifdef HAS_OAUTH
#include "twitter.h"
#endif

GraphScene::GraphScene() :
    algo(0),
    degreeCount(1),
    running(false)
{
    myAlgorithms["Preferential Attachament"] = PREFERENTIAL_ATTACHAMENT;
    myAlgorithms["Bipartite Model"] = BIPARTITE_MODEL;
    myAlgorithms["Erdos Renyi"] = ERDOS_RENYI;
    myAlgorithms["Barabasi Albert"] = BARABASI_ALBERT;
#ifdef HAS_OAUTH
    myAlgorithms["Twitter"] = TWITTER;
#endif
    stats = new Statistics(this);
}

GraphScene::~GraphScene() {
    delete stats;
}

QList<QString> GraphScene::algorithms() const {
    return myAlgorithms.keys();
}

Statistics* GraphScene::getStatistics() {
    return stats;
}

void GraphScene::reset() {
    clear();
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
    addItem(edge);
    myEdges << edge;
    updateDegreeCount(source);
    updateDegreeCount(dest);

    return true;
}

// used only by the algorithms
Node* GraphScene::newNode() {
    Node *node = new Node(this);
    addItem(node);
    myNodes << node;
    node->setPos(10 + qrand() % 1000, 10 + qrand() % 600);

    return node;
}

// Pre: Will remove nodes staring from last node in list
void GraphScene::removeNode(Node *n){

    removeItem(n);
    myNodes.remove(n->tag());
    Node::setAllNodes(Node::getAllNodes() - 1);
}

// CutoffTag is for destNodes only!
void GraphScene::removeEdges(int cutoffTag){


    for(int i(0); i < myEdges.size(); ++i){
        if(myEdges[i]->destNode()->tag() >= cutoffTag){

            removeItem(myEdges[i]);
            myEdges.removeAt(i);
            // just to make sure nothing is skipped
            --i;
        }



    }
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
    emit itemMovedSignal();
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
#ifdef HAS_OAUTH
        case TWITTER:
            algo = new Twitter(this);
            break;
#endif
        }
    }
    algo->reset();
    int counter = 0;
    if(algo->getSWNFlag()){
        if(!stats)
            stats = new Statistics(this);
        // Cutoff value exsists because for low max degree
        // generating a reasonable exponent is not possible
        double exponent = stats->powerLawExponent();
        while((exponent > 4.0 ||
              exponent < 2.1 ) &&
              (counter < 200)){
            reset();
            algo->reset();
            exponent = stats->powerLawExponent();
            ++counter;

        }

        // if counter = 500 issue message
        // "failed to generate small world network exponent"
        // or smthing like that

    }
    randomizePlacement();

    emit repopulated();

}

Algorithm* GraphScene::algorithm() const {
    return algo;
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

    //calculateMetrics();

    emit repopulated();

}

// Pre: degree is valid
QList<Node *> GraphScene::getDegreeList(int degree) const {
    return degreeCount[degree - 1];
}

// Pre: Node has just been given a new edge
void GraphScene::updateDegreeCount(Node *node) {
    int degree = node->edges().size();

    if (degree > degreeCount.size())
        degreeCount.resize(degree);

    degreeCount[degree - 1].append(node);

    if (degree > 1)
         degreeRemove(node);
}

/*
void GraphScene::calculateMetrics() {
    if(!stats)
        stats = new Statistics(this);

    stats->clusteringAvg();
}
*/


void GraphScene::calculateForces() {
    QPointF topLeft;
    QPointF bottomRight;

    QuadTree quadTree(sceneRect());
    foreach (Node *node, nodes()) {
        quadTree.addNode(node);
    }

    // Don't move the first node
    bool first = true;
    foreach (Node *node, nodes()) {
        if (first) {
            first = false;
            continue;
        }

        QPointF pos = node->calculatePosition(quadTree.root());

        if (pos.x() < topLeft.x())
            topLeft.setX(pos.x());
        if (pos.y() < topLeft.y())
            topLeft.setY(pos.y());
        if (pos.x() > bottomRight.x())
            bottomRight.setX(pos.x());
        if (pos.y() > bottomRight.y())
            bottomRight.setY(pos.y());
    }

    // Resize the scene to fit all the nodes
    sceneRect().setLeft(topLeft.x() - 10);
    sceneRect().setTop(topLeft.y() - 10);
    sceneRect().setRight(bottomRight.x() + 10);
    sceneRect().setBottom(bottomRight.y() + 10);

    running = false;
    foreach (Node *node, nodes()) {
        if (node->advance()) {
            running = true;
        }
    }
}

int GraphScene::maxDegree() const {
    return degreeCount.count();
}

int GraphScene::nodeCount(int degree) const {
    /*
    actually degree is one less than the degree we are looking for
    But since this is only used by statistics.cpp it does not matter
    */
    return degreeCount[degree].size();
}

void GraphScene::degreeRemove(Node *node) {
    int degree = node->edges().size();

    QList<Node *> nodes = degreeCount[degree - 2];
    for (int i(0); i < nodes.size(); ++i) {
        if (nodes[i]->tag() == node->tag()) {
            degreeCount[degree - 2].removeAt(i);
            break;
        }
    }
}

void GraphScene::setAllNodes(int i){
    Node::setAllNodes(i);
}

bool GraphScene::isRunning() const{
    return running;
}
