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
#include "wattsstrogatz.h"
#ifdef HAS_OAUTH
#include "twitter.h"
#endif

GraphScene::GraphScene(QObject *parent) :
    QObject(parent),
    algo(0),
    degreeCount(1),
    myBackgroundColour(Qt::black),
    mode3d(false),
    myEdgeColour(QColor::fromRgbF(0.0, 0.0, 1.0, 0.5)),
    myNodeColour(QColor::fromRgbF(0.0, 1.0, 0.3, 0.7))
{
    myAlgorithms["Preferential Attachament"] = PREFERENTIAL_ATTACHAMENT;
    myAlgorithms["Bipartite Model"] = BIPARTITE_MODEL;
    myAlgorithms["Erdos Renyi"] = ERDOS_RENYI;
    myAlgorithms["Barabasi Albert"] = BARABASI_ALBERT;
#ifdef HAS_OAUTH
    myAlgorithms["Twitter"] = TWITTER;
#endif
    myAlgorithms["Watts Strogatz"] = WATTS_STROGATZ;
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

void GraphScene::onNodeMoved() {
    emit nodeMoved();
}

void GraphScene::reset() {
    //clear();
    hasEdge.clear();
    myEdges.clear();
    foreach (Node *node, myNodes) {
        disconnect(node, 0, 0, 0);
    }
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

void GraphScene::set3DMode(bool enabled) {
    mode3d = enabled;

    randomizePlacement();
}

QColor GraphScene::nodeColour() {
    return myNodeColour;
}

QColor GraphScene::edgeColour() {
    return myEdgeColour;
}

QColor GraphScene::backgroundColour() {
    return myBackgroundColour;
}

bool GraphScene::newEdge(Node *source, Node *dest) {
    Q_ASSERT(source != 0);
    Q_ASSERT(dest != 0);
    if (doesEdgeExist(source, dest)) {
        return false;
    }
    Edge *edge = new Edge(source, dest);
    edge->setColour(myEdgeColour);
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
    node->setColour(myNodeColour);

    myNodes << node;

    float z = 0;
    if (mode3d) {
        z = (qrand() % 600) - 300;
    }

    node->setPos(VPointF((qrand() % 1000) - 500,
                         (qrand() % 600) - 300,
                         z));
    connect(node, SIGNAL(nodeMoved()), this, SLOT(onNodeMoved()));
    onNodeMoved();

    return node;
}

// Pre: Will remove nodes staring from last node in list
void GraphScene::removeNode(Node *n) {
    myNodes.remove(n->tag());
}

// CutoffTag is for destNodes only!
void GraphScene::removeEdges(int cutoffTag) {
    for (int i(0); i < myEdges.size(); ++i) {
        if (myEdges[i]->destNode()->tag() >= cutoffTag) {
            myEdges.removeAt(i);
            // just to make sure nothing is skipped
            --i;
        }
    }
}

// used in Watts Strogatz
void GraphScene::removeEdge(Node * source, Node* dst){
    for(int i(0); i < myEdges.size(); ++i){
        if(myEdges[i]->sourceNode()->tag() == source->tag() &&
            myEdges[i]->destNode()->tag() == dst->tag()){
            myEdges.removeAt(i);
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

void GraphScene::chooseAlgorithm(const QString &name) {
    if (algo) {
        delete algo;
        algo = 0;
    }
    algoId = myAlgorithms[name];
    repopulate();

    emit algorithmChanged(algo);
}

void GraphScene::customizeEdgesColour(const QColor &newColour) {
    myEdgeColour = newColour;

    foreach(Edge* edge, myEdges) {
        if (!edge->isHighlighted)
            edge->setColour(myEdgeColour);
    }
}

void GraphScene::customizeNodesColour(const QColor &newColour) {
    myNodeColour = newColour;

    foreach(Node* node, myNodes) {
        if(!node->isHighlighted)
            node->setColour(myNodeColour);
    }
}

void GraphScene::customizeBackgroundColour(const QColor &newColour) {
    myBackgroundColour = newColour;
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
        case WATTS_STROGATZ:
            algo = new WattsStrogatz(this);
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
        if(!stats) {
            stats = new Statistics(this);
        }
        // Cutoff value exsists because for low max degree
        // generating a reasonable exponent is not possible
        double exponent = stats->powerLawExponent();
        while((exponent > 4.0 ||
              exponent < 2.1 ) &&
              (counter < 200))
        {
            reset();
            algo->reset();
            exponent = stats->powerLawExponent();
            ++counter;
        }
    }
    randomizePlacement();

    emit repopulated();
}

Algorithm* GraphScene::algorithm() const {
    return algo;
}

void GraphScene::randomizePlacement() {
    float z = 0;
    if (mode3d) {
        z = (qrand() % 600) - 300;
    }

    foreach (Node *node, nodes()) {
        node->setPos(VPointF((qrand() % 1000) - 500,
                              (qrand() % 600) - 300,
                              z));
    }
}

void GraphScene::addVertex() {
    algo->addVertex();
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

bool GraphScene::calculateForces() {
    QuadTree quadTree(graphCube().longestEdge());
    foreach (Node* node, nodes()) {
        quadTree.addNode(node);
    }

    // Don't move the first node
    bool first = true;
    foreach (Node *node, nodes()) {
        if (first) {
            first = false;
            continue;
        }

        node->calculatePosition(quadTree.root());
    }

    bool somethingMoved = false;
    foreach (Node *node, nodes()) {
        if (node->advance()) {
            somethingMoved = true;
        }
    }

    return somethingMoved;
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
