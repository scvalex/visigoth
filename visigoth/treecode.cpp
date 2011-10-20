#include "treecode.h"
#include "treenode.h"

#include <QRectF>
#include <cmath>
#include <limits>

#include <iostream>

int ipow(int base, int exp)
{
    int result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }

    return result;
}

TreeCode::TreeCode(QVector<Node*>& nodeVector) :
    boundaries(calculateBoundaries(nodeVector))
{
    levels = calculateLevels(this->boundaries.width());

    allocateNodes();

    fillNodes(nodeVector);
}

TreeCode::~TreeCode()
{
    for (int i = 0; i < nodes.size(); ++i) {
        for (int j = 0; j < nodes[i].size(); ++j) {
            for (int k = 0; k < nodes[i][j].size(); ++k) {
                delete nodes[i][j][k];
            }
        }
    }
}

TreeNode* TreeCode::getRoot()
{
    return nodes[0][0][0];
}

QRectF TreeCode::getBoundaries()
{
    return boundaries;
}

QRectF TreeCode::calculateBoundaries(QVector<Node*>& nodeVector)
{
    QPointF topLeft(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    QPointF bottomRight(std::numeric_limits<double>::min(), std::numeric_limits<double>::min());

    foreach (Node* node, nodeVector) {
        QPointF pos = node->pos();
        if (pos.x() < topLeft.x()) {
            topLeft.setX(pos.x());
        }
        if (pos.y() < topLeft.y()) {
            topLeft.setY(pos.y());
        }
        if (pos.x() > bottomRight.x()) {
            bottomRight.setX(pos.x());
        }
        if (pos.y() > bottomRight.y()) {
            bottomRight.setY(pos.y());
        }
    }

    // Increase the right and bottom part by one so that the node on the boundary will be in
    bottomRight.setX(bottomRight.x() + 1);
    bottomRight.setY(bottomRight.y() + 1);

    QRectF boundaries(topLeft, bottomRight);

    qreal edge = qMax(boundaries.height(), boundaries.width());
    // Make sure that the boundaries are a square
    if (boundaries.height() > edge) {
        qreal diff = (boundaries.height() - edge) / 2;
        boundaries.setLeft(boundaries.left() - diff);
        boundaries.setRight(boundaries.right() + diff);
    }
    if (boundaries.width() > edge) {
        qreal diff = (boundaries.width() - edge) / 2;
        boundaries.setTop(boundaries.top() - diff);
        boundaries.setBottom(boundaries.bottom() + diff);
    }
    return boundaries;
}

int TreeCode::calculateLevels(qreal edge)
{
    levels = 1;
    while (edge / TREE_WAY > QUADRANT_MIN_SIZE && levels <= MAX_LEVELS) {
        edge /= TREE_WAY;
        ++levels;
    }
    return levels;
}

int TreeCode::flattenIndex(int rowCells, int row, int col) {
    return rowCells * row + col;
}

void TreeCode::allocateNodes()
{
    nodes.resize(levels);

    for (int l = levels-1; l >= 0; --l) {
        int quadrants = getLevelQuadrants(l);

        nodes[l].resize(quadrants);
        qreal levelWidth = boundaries.width() / (qreal) quadrants;

        for (int row = 0; row < quadrants; row++) {
            nodes[l][row].resize(quadrants);
            for (int col = 0; col < quadrants; col++) {
                Branch* branch = new Branch(levelWidth);
                if (l < levels-1) {
                    int rowsup = row * TREE_WAY;
                    int colsup = col * TREE_WAY;
                    for (int r = 0; r < TREE_WAY; r++) {
                        for (int c = 0; c < TREE_WAY; c++) {
                            branch->addChild(nodes[l+1][rowsup+r][colsup+c]);
                        }
                    }
                }
                nodes[l][row][col] = branch;
            }
        }
    }
}

void TreeCode::fillNodes(QVector<Node*>& nodeVector)
{
    qreal leavesQuadrants = getLevelQuadrants(levels-1);
    qreal leafQuadrantWidth = boundaries.width() / (qreal) leavesQuadrants;

    // Fill up the nodes
    foreach (Node* node, nodeVector) {
        QPointF pos = node->pos();

        // row and col are the ones referring to the leaves (the last quadrants)
        int row = floor((pos.x() - boundaries.left()) / leafQuadrantWidth);
        int col = floor((pos.y() - boundaries.top()) / leafQuadrantWidth);

        if (row >= leavesQuadrants - 1) {
            row = leavesQuadrants - 1;
        }
        if (col >= leavesQuadrants - 1) {
            col = leavesQuadrants - 1;
        }
        nodes[levels-1][row][col]->addChild(node);
        nodes[levels-1][row][col]->addNode(node);

        for (int l = 0; l < levels; ++l) {
            int levelQuadrants = getLevelQuadrants(l);

            int rowl = (levelQuadrants * row) / leavesQuadrants;
            int coll = (levelQuadrants * col) / leavesQuadrants;

            nodes[l][rowl][coll]->addNode(node);
        }
    }
}

inline int TreeCode::getLevelQuadrants(int l)
{
    return ipow(TREE_WAY, l);
}

QVector<TreeNode*> TreeCode::Branch::noChildren(0);

TreeCode::Branch::Branch(qreal width, int size) :
    width(width),
    size(size),
	center(QPointF(0, 0))
{
}

int TreeCode::Branch::getSize()
{
    return size;
}

QPointF TreeCode::Branch::getCenter()
{
    return center;
}

QVector<TreeNode*>& TreeCode::Branch::getChildren()
{
    if (size > 1) {
        return children;
    } else {
        return noChildren;
    }
}

qreal TreeCode::Branch::getWidth()
{
    return width;
}

void TreeCode::Branch::addNode(TreeNode* node)
{
    QPointF nodeCenter = node->getCenter();
    int nodeSize = node->getSize();
    center = QPointF((size * center.x() + nodeCenter.x()) / (size + nodeSize),
                     (size * center.y() + nodeCenter.y()) / (size + nodeSize));
    size++;
}

void TreeCode::Branch::addChild(TreeNode* node) {
    children.append(node);
}
