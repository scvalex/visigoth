#include "treecode.h"

#include <QRectF>
#include <cmath>

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

TreeCode::TreeCode(QVector<Node*>& nodeVector, QRectF boundaries) :
    root(Branch(this, 0, 0, 0, nodeVector.size()))
{
    this->boundaries = squareBoundaries(boundaries);

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

TreeObject& TreeCode::getRoot()
{
    return root;
}

QRectF TreeCode::squareBoundaries(QRectF boundaries)
{
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
	levels = 0;
	while (edge / TREE_WAY > QUADRANT_MIN_SIZE && levels <= MAX_LEVELS) {
		edge /= TREE_WAY;
		++levels;
	}
	return levels;
}

void TreeCode::allocateNodes()
{
	nodes.resize(levels);
	for (int l = 0; l < levels; ++l) {
		int nextl = l + 1;

		// The number of outer quadrants
		int quadrants = ipow(TREE_WAY, l);
		nodes[l].resize(quadrants * quadrants);

		// Traverse the outer quadrants
		for (int rowout = 0; rowout < quadrants; ++rowout) {
			for (int colout = 0; colout < quadrants; ++colout) {

				int ix = rowout * quadrants + colout;

				// Allocate the inner 4 (or 9, or whatever) nodes and traverse
				nodes[l][ix].resize(TREE_WAY * TREE_WAY);
				for (int rowin = 0; rowin < TREE_WAY; ++rowin) {
					for (int colin = 0; colin < TREE_WAY; ++colin) {
						int ixin = rowin * TREE_WAY + colin;

                        int rown = rowout * TREE_WAY + rowin;
                        int coln = colout * TREE_WAY + colin;

                        // If it's the last level, create a leaf
                        if (nextl < levels) {
                            nodes[l][ix][ixin] = new Branch(this, nextl, rown, coln);
                        } else {
                            nodes[l][ix][ixin] = new Leaf(this, rown, coln);
                        }
                    }
                }
            }
        }
    }
}

void TreeCode::fillNodes(QVector<Node*>& nodeVector)
{
    qreal leavesQuadrants = getLevelQuadrants(levels);
    qreal leafQuadrantWidth = boundaries.width() / (qreal) leavesQuadrants;

    // Fill up the nodes
    foreach (Node* node, nodeVector) {
        QPointF pos = node->pos();

        // row and col are the ones referring to the leaves (the last quadrants)
        int row = floor((pos.x() - boundaries.left()) / leafQuadrantWidth);
        int col = floor((pos.y() - boundaries.top()) / leafQuadrantWidth);

		// This shouldn't happen, but it seems to happen
		if (row >= leavesQuadrants) {
			row = leavesQuadrants - 1;
		}
		if (col >= leavesQuadrants) {
			col = leavesQuadrants - 1;
		}

        for (int l = 0; l < levels; ++l) {
            int nextl = l + 1;
            int levelQuadrants = getLevelQuadrants(nextl);

            int rowin = (int) ceil((levelQuadrants * row) / leavesQuadrants);
            int colin = (int) ceil((levelQuadrants * col) / leavesQuadrants);

            int rowout = rowin / (TREE_WAY * TREE_WAY);
            int colout = colin / (TREE_WAY * TREE_WAY);

            int ixout = rowout * ipow(TREE_WAY, l) + colout;
            int ixin = (rowout % TREE_WAY) * TREE_WAY + (colout % TREE_WAY);

            nodes[l][ixout][ixin]->addNode(node);
        }
    }
}

int TreeCode::getLevelQuadrants(int l)
{
    return ipow(TREE_WAY, l);
}

QPointF getQuadrantCenter(QRectF boundaries, int treeWay, int level, int row, int col)
{
    qreal edgeWidth = boundaries.x() / ipow(treeWay, level);
    qreal x = (col * edgeWidth) + (edgeWidth / 2);
    qreal y = (row * edgeWidth) + (edgeWidth / 2);
    return QPointF(x, y);
}

TreeCode::Branch::Branch(TreeCode* tree, int level, int row, int col, int size) :
	tree(tree),
	level(level),
	row(row),
	col(col),
	size(size)
{
}

int TreeCode::Branch::getSize()
{
    return 0;
}

QPointF TreeCode::Branch::getCenter()
{
    return getQuadrantCenter(tree->boundaries, tree->TREE_WAY, level, row, col);
}

void TreeCode::Branch::addNode(Node* node)
{
    this->size++;
}

QVector<TreeObject*>* TreeCode::Branch::getChildren()
{
    int edgeWidth = ipow(this->tree->TREE_WAY, this->level);
    return &this->tree->nodes[this->level + 1][this->row * edgeWidth + this->col];
}

QVector<Node*>* TreeCode::Branch::getNodes()
{
    return NULL;
}

TreeCode::Leaf::Leaf(TreeCode* tree, int row, int col) :
    nodes(QVector<Node*>()),
    tree(tree),
    row(row),
    col(col)
{
}

int TreeCode::Leaf::getSize()
{
	return nodes.size();
}

QPointF TreeCode::Leaf::getCenter()
{
	return getQuadrantCenter(tree->boundaries, tree->TREE_WAY, tree->levels, row, col);
}

void TreeCode::Leaf::addNode(Node* node)
{
	nodes.append(node);
}

QVector<TreeObject*>* TreeCode::Leaf::getChildren()
{
	return NULL;
}

QVector<Node*>* TreeCode::Leaf::getNodes()
{
	return &nodes;
}
