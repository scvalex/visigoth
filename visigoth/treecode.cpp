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
    root(Branch(this, 0, 0, 0, nodeVector.size()))
{
    this->boundaries = calculateBoundaries(nodeVector);

    levels = calculateLevels(this->boundaries.width());

    allocateNodes();

    fillNodes(nodeVector);

    /*
    foreach (Node* node, nodeVector) {
        std::cout << node->getCenter().x() << " " << node->getCenter().y() << "\n";
    }

    std::cout << "----------------------\n";

    foreach (QVector<TreeNode*> vec, leaves) {
        foreach (TreeNode* node, vec) {
            std::cout << node->getCenter().x() << " " << node->getCenter().y() << "\n";
        }
    }

    throw 1;
    */
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

TreeNode& TreeCode::getRoot()
{
    return root;
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
		int quadrants = getLevelQuadrants(l);
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

                        nodes[l][ix][ixin] = new Branch(this, nextl, rown, coln);
                    }
                }
            }
        }
    }

    int leavesQuadrants = getLevelQuadrants(levels);
    leaves.resize(leavesQuadrants * leavesQuadrants);
    for (int row = 0; row < leavesQuadrants; ++row) {
        for (int col = 0; col < leavesQuadrants; ++col) {
            leaves[row * leavesQuadrants + col] = QVector<TreeNode*>();
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

            int ixout = rowout * getLevelQuadrants(l) + colout;
            int ixin = (rowout % TREE_WAY) * TREE_WAY + (colout % TREE_WAY);

            dynamic_cast<Branch*>(nodes[l][ixout][ixin])->addNode(node);

            if (l >= levels - 1) {
                leaves[rowin * levelQuadrants + colin].append(node);
            }
        }
    }
}

int TreeCode::getLevelQuadrants(int l)
{
    return ipow(TREE_WAY, l);
}

TreeCode::Branch::Branch(TreeCode* tree, int level, int row, int col, int size) :
	tree(tree),
	level(level),
	row(row),
	col(col),
	size(size),
	center(QPointF(0, 0))
{
}

int TreeCode::Branch::getSize()
{
    return 0;
}

QPointF TreeCode::Branch::getCenter()
{
    if (this->size == 1) {
        return this->center;
    } else {
        qreal edgeWidth = tree->boundaries.x() / tree->getLevelQuadrants(level);
        qreal x = (col * edgeWidth) + (edgeWidth / 2);
        qreal y = (row * edgeWidth) + (edgeWidth / 2);
        return QPointF(x, y);
    }
}

void TreeCode::Branch::addNode(Node* node)
{
    this->size++;
    this->center = node->pos();
}

QVector<TreeNode*>& TreeCode::Branch::getChildren()
{
    int edgeWidth = tree->getLevelQuadrants(level);
    if (level >= this->tree->levels) {
        return this->tree->leaves[this->row * edgeWidth + this->col];
    } else {
        return this->tree->nodes[this->level + 1][this->row * edgeWidth + this->col];
    }
}
