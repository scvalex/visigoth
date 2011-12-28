#include <cmath>
#include <stdexcept>
#include <iostream>

#include "vtools.h"
#include "quadtree.h"

// The size of the smallest quadrants.
static const int BASE_QUADRANT_SIZE = 30;

inline static int pow2(int exp) {
    return 1 << exp;
}

inline static int calculateWidth(vreal longestEdge) {
    // Make the width so that it will contain a number of squares of base size which is a
    // power of 2 (so that the square will contain a number of terminal quadrants which is a
    // power of 4).
    vreal baseQuadrants = longestEdge / (vreal) BASE_QUADRANT_SIZE;
    return pow2((int) ceil(log(baseQuadrants) / log(2.0))) * BASE_QUADRANT_SIZE;
}

inline static int getIndex(int x, int y, int z) {
    // Front:     Back:
    // .-------.  .-------.
    // | 0 | 1 |  | 5 | 4 |
    // |-------|  |-------|
    // | 2 | 3 |  | 7 | 6 |
    // `-------`  `-------`

    if        ( x == LEFT   &&  y == TOP     &&  z == FRONT ) {
        return 0;
    } else if ( x == RIGHT  &&  y == TOP     &&  z == FRONT ) {
        return 1;
    } else if ( x == LEFT   &&  y == BOTTOM  &&  z == FRONT ) {
        return 2;
    } else if ( x == RIGHT  &&  y == BOTTOM  &&  z == FRONT ) {
        return 3;
    } else if ( x == LEFT   &&  y == TOP     &&  z == BACK  ) {
        return 4;
    } else if ( x == RIGHT  &&  y == TOP     &&  z == BACK  ) {
        return 5;
    } else if ( x == LEFT   &&  y == BOTTOM  &&  z == BACK  ) {
        return 6;
    } else if ( x == RIGHT  &&  y == BOTTOM  &&  z == BACK  ) {
        return 7;
    } else {
        throw std::runtime_error("quadtree.cpp castAndAddChild: got an out of bounds direction.");
    }
}

// ---------------------------------------------------------------------------
// QuadTree

QuadTree::QuadTree(vreal longestEdge)
{
    int width = calculateWidth(longestEdge);

    // First node, level 0, coords 0,0, width equals to the width of the entire space
    _root = new QuadTree::Quadrant(0, VPointF(0.0), width);
}

QuadTree::~QuadTree() {
    delete _root;
}

QuadTree::TreeNode& QuadTree::root() const {
    return *_root;
}

void QuadTree::addNode(QuadTree::TreeNode *node) {
    _root->addChild(node);
}


// ---------------------------------------------------------------------------
// QuadTree::TreeNode

bool QuadTree::TreeNode::isFarEnough(vreal distance) {
    return (width() / distance) <= tolerance;
}

// ---------------------------------------------------------------------------
// QuadTree::Quadrant

QuadTree::Quadrant::Quadrant(int level, VPointF center, int width) :
    level(level),
    quadrantCenter(center),
    _width(width),
    _size(0),
    _center(center)
{
}

QuadTree::Quadrant::~Quadrant() {
    foreach (TreeNode *node, _children) {
        QuadTree::Quadrant *quadrant = dynamic_cast<QuadTree::Quadrant*>(node);

        if (quadrant) {
            delete quadrant;
        }
    }
}

int QuadTree::Quadrant::size() const {
    return _size;
}

VPointF QuadTree::Quadrant::center() const {
    return _center;
}

bool QuadTree::Quadrant::hasChildren() const {
    return _children.size() > 0;
}

const QVector<QuadTree::TreeNode*>& QuadTree::Quadrant::children() const {
    return _children;
}

vreal QuadTree::Quadrant::width() const {
    return (vreal) _width;
}

void QuadTree::Quadrant::castAndAddChild(QuadTree::TreeNode *node, QuadTree::TreeNode *child) const {
    QuadTree::Quadrant *q = dynamic_cast<QuadTree::Quadrant*>(node);

    if (q == NULL) {
        throw std::runtime_error("QuadTree::Quadrant::castAndAddChild: trying to cast a non-Quadrant.");
    }

    q->addChild(child);
}

bool QuadTree::Quadrant::isTerminal() {
    return width() <= BASE_QUADRANT_SIZE;
}

inline VPointF QuadTree::Quadrant::weightedMiddle(QuadTree::TreeNode *node1, QuadTree::TreeNode *node2) const {
    // If node1 and node2 are two vectors w and v with respective weights a and b, we want
    // (a*w + b*v) / (a + b)

    return ((node1->center() * (vreal)node1->size()) +
            (node2->center() * (vreal)node2->size())) /
            (vreal)(node1->size() + node2->size());
}

void QuadTree::Quadrant::allocateChildren() {
    _children.resize(CHILDREN);

    int childEdge = _width / 2;

    /* The various children will have quadrant centers translated by
     * childWidth / 2 from the parent node's quadrant center, so we'll
     * need to add/subtract childWidth / 2 to the current quadrant
     * center, depending on the position.
     */

    // For both left/right
    for (int xsign = LEFT; xsign <= RIGHT; xsign += (RIGHT - LEFT)) {

        // For both top/bottom
        for (int ysign = TOP; ysign <= BOTTOM; ysign += (BOTTOM - TOP)) {

            // For both back/front
            for (int zsign = BACK; zsign <= FRONT; zsign += (FRONT - BACK)) {

                int childChildEdge = childEdge / 2;
                VPointF childCenter(
                    quadrantCenter.x + (childChildEdge * xsign),
                    quadrantCenter.y + (childChildEdge * ysign),
                    quadrantCenter.z + (childChildEdge * zsign));
                _children[getIndex(xsign, ysign, zsign)] =
                    new QuadTree::Quadrant(level + 1, childCenter, childEdge);
            }
        }
    }
}

void QuadTree::Quadrant::addChildToChildren(QuadTree::TreeNode *node) {
    // Add the node recursively, inspecting which child it belongs to.
    int x;
    int y;
    int z;

    if (node->center().x < quadrantCenter.x) {
        x = LEFT;
    } else {
        x = RIGHT;
    }

    if (node->center().y < quadrantCenter.y) {
        y = TOP;
    } else {
        y = BOTTOM;
    }

    if (node->center().z < quadrantCenter.z) {
        z = BACK;
    } else {
        z = FRONT;
    }

    castAndAddChild(_children[getIndex(x, y, z)], node);

}

void QuadTree::Quadrant::addChild(QuadTree::TreeNode *node) {
    // Weigh the center with the new node
    _center = weightedMiddle(this, node);

    // If it's not a terminal node, recurse down
    if (!isTerminal()) {
        // If the node has no children, create them
        if (_children.size() < CHILDREN) {
            allocateChildren();
        }

        addChildToChildren(node);
    } else {
        // If it's a terminal node, just add the node to the list of children
        _children.append(node);
    }

    // Increase the size
    ++_size;
}

// ---------------------------------------------------------------------------
// Debug functions

// Prints the tree in a nice way.
void QuadTree::printTree(QuadTree::TreeNode *node) const {
    QuadTree::Quadrant *q = dynamic_cast<QuadTree::Quadrant*>(node);

    if (q != NULL) {
        foreach (QuadTree::TreeNode *child, node->children()) {
            for (int i = 0; i < q->getLevel(); i++) {
                std::cout << "\t";
            }

            std::cout << "Level: " << (q->getLevel() + 1) << ", width: " << child->width()
                      << ", size: " << child->size() << ", center: " << child->center().x
                      << "," << child->center().y;

            QuadTree::Quadrant *qchild = dynamic_cast<QuadTree::Quadrant*>(child);
            if (qchild) {
                std::cout << ", quadcenter: " << qchild->getQuadrantCenter().x
                          << "," << qchild->getQuadrantCenter().y;
            }

            std::cout << "\n";

            printTree(child);
        }
    }
}

int QuadTree::Quadrant::getLevel() const {
    return level;
}

VPointF QuadTree::Quadrant::getQuadrantCenter() const {
    return quadrantCenter;
}
