#include "edge.h"
#include "graphscene.h"
#include "node.h"
#include "quadtree.h"

#include <cmath>
#include <stdexcept>

int Node::ALL_NODES(0);

Node::Node(GraphScene *graph) :
    QObject(graph),
    graph(graph),
    myColour(QColor::fromRgbF(0.0, 1.0, 0.3, 0.7)),
    curPos(0.0),
    newPos(0.0),
    allowAdvance(true),
    visited(false),
    distance(0)
{
    myTag = ALL_NODES++;
}

Node::~Node() {
}

int Node::tag() const {
    return myTag;
}

void Node::addEdge(Edge *edge) {
    edgeList << edge;
}

VPointF Node::pos() const {
    return curPos;
}

void Node::setPos(VPointF pos, bool silent) {
    curPos = pos;
    if (!silent)
        emit nodeMoved();
}

VPointF Node::calculatePosition(TreeNode &treeNode) {
    VPointF vel = calculateNonEdgeForces(&treeNode);

    // Now all the forces that pulling items together
    double weight = (edgeList.size() + 1) * 10;

    foreach (Edge *edge, edgeList) {
        VPointF vec = VPointF(0.0);
        if (edge->sourceNode() == this) {
            vec = pos() - edge->destNode()->pos();
        } else {
            vec = pos() - edge->sourceNode()->pos();
        }
        vel = vel - (vec / weight);
    }

    if (qAbs(vel.lengthSquared()) < 0.1) {
        vel = VPointF(0.0);
    }

    newPos = pos() + vel;

    return newPos;
}

VPointF Node::calculateNonEdgeForces(QuadTree::TreeNode* treeNode) {
    if (treeNode->size() < 1) {
        return VPointF(0.0);
    }

    VPointF vec = this->pos() - treeNode->center();
    VPointF vel = VPointF(0.0);

    if (treeNode->isFarEnough(vec.length()) || treeNode->size() == 1) {
        double l = vec.lengthSquared();

        if (l > 0) {
            vel = vec * (75.0 / l);
        } else {
            vel = VPointF(0.0);
        }
    } else {
        vel = VPointF(0.0);
        foreach (TreeNode* child, treeNode->children()) {
            vel = vel + calculateNonEdgeForces(child);
        }
    }
    return vel;
}

VPointF Node::calculatePosition3D(QVector<Node*> &nodes) {

    // Sum up all forces pushing this item away
    VPointF vel = VPointF(0.0);

    foreach (Node *node, nodes) {
        VPointF vec = pos() - node->pos();
        double l = vec.lengthSquared();
        if (l > 0) {
            vel = vel + (vec * 75 / l);
        }
    }

    // Now subtract all forces pulling items together
    double weight = (edgeList.size() + 1) * 10;

    foreach (Edge *edge, edgeList) {
        VPointF vec = VPointF(0.0);
        if (edge->sourceNode() == this) {
            vec = pos() - edge->destNode()->pos();
        } else {
            vec = pos() - edge->sourceNode()->pos();
        }
        vel = vel - (vec / weight);
    }

    if (vel.lengthSquared() < 0.1) {
        vel = VPointF(0);
    }

    newPos = pos() + vel;

    // newPos = pos();

    return newPos;
}


bool Node::advance() {
    if (!allowAdvance)
        return false;

    if (newPos == pos())
        return false;

    setPos(newPos, true);

    return true;
}

void Node::setAllowAdvance(bool allow) {
    this->allowAdvance = allow;
}

QList<Edge*>& Node::edges() {
    return edgeList;
}

QVector<Node*> Node::neighbours() const {
    QVector<Node*> ns;
    foreach (Edge *e, edgeList) {
        if (e->sourceNode()->tag() == this->tag())
            ns << e->destNode();
        if (e->destNode()->tag() == this->tag())
            ns << e->sourceNode();
    }
    return ns;
}

void Node::reset() {
    ALL_NODES = 0;
}

int Node::size() const {
    return 1;
}

VPointF Node::center() const {
    return pos();
}

bool Node::hasChildren() const {
    return false;
}

const QVector<QuadTree::TreeNode*>& Node::children() const {
    throw std::runtime_error("Node: calling children() on a terminal node");
}

vreal Node::width() const {
    return 0;
}

QColor& Node::colour() {
    return myColour;
}

void Node::setColour(const QColor &c) {
    myColour = c;
}
