#include "edge.h"
#include "graphscene.h"
#include "abstractgraphwidget.h"
#include "node.h"
#include "preferential.h"
#include "bipartite.h"
#include "erdosrenyi.h"
#include "statistics.h"
#include "barabasialbert.h"


GraphScene::GraphScene(AbstractGraphWidget *parent) :
    //QGraphicsScene(parent),
    algo(0),
    stats(0),
    view(parent),
    degreeCount(1),
    metricVector(6, 0.0),
    running(false)
{

    myAlgorithms["Preferential Attachament"] = PREFERENTIAL_ATTACHAMENT;
    myAlgorithms["Bipartite Model"] = BIPARTITE_MODEL;
    myAlgorithms["Erdos Renyi"] = ERDOS_RENYI;
}

QList<QString> GraphScene::algorithms() const {
    return myAlgorithms.keys();
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

    return node;
}

bool GraphScene::doesEdgeExist(Node *source, Node *dest) {
    int sourceTag = source->tag();
    int destTag = dest->tag();

    if (0 <= sourceTag && sourceTag < hasEdge.size() &&
        0 <= destTag && destTag < hasEdge.size())
    {
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
        }
    }
    algo->reset();
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
}

// Pre: degree is valid
QList<Node *> GraphScene::getDegreeList(int degree) {
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

    metricVector[0] = stats->averageDegree();
    //metricVector[1] = stats->averageLength();
    metricVector[2] = stats->clusteringAvg();
    metricVector[3] = stats->clusteringCoeff(myNodes[qrand() % myNodes.count()]);
    //metricVector[4] = stats->clusteringDegree(6);
    metricVector[5] = stats->powerLawExponent();

}

void GraphScene::calculateForces() {
    QPointF topLeft;
    QPointF bottomRight;

    QuadTree quadTree(sceneRect());
    foreach (Node* node, nodes()) {
        quadTree.addNode(*node);
    }

    foreach (Node* node, nodes()) {
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

bool GraphScene::isRunning() {
    return true;
}

int GraphScene::maxDegree(){

    return degreeCount.count();
}

int GraphScene::nodeCount(int degree){

    /*
    actually it is degree is one less than the degree we are looking for
    But since this is only used by statistics.cpp it does not matter
    */
    return degreeCount[degree].size();
}

void GraphScene::degreeRemove(Node *n){

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

