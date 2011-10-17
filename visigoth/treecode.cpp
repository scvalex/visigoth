#include "treecode.h"

#include <QRectF>
#include <cmath>

int ipow(int base, int exp) {
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

TreeCode::TreeCode(QVector<Node*>& nodeVector, QRectF boundaries) :
    size(nodeVector.size())
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
    this->boundaries = boundaries;

	// Calculate level
	levels = 0;
	while (edge / TREE_WAY > QUADRANT_MIN_SIZE && levels <= MAX_LEVELS) {
		edge /= TREE_WAY;
		++levels;
	}

	// Allocate all the nodes
	nodes.resize(levels);
	for (int l = 0; l < levels; ++l) {
		int l1 = l + 1;

		int edge = ipow(TREE_WAY, l);
		nodes[l].resize(edge * edge);

		for (int row = 0; row < edge; ++row) {
			for (int col = 0; col < edge; ++col) {

				int ix = row * edge + col;
				nodes[l][ix].resize(TREE_WAY * TREE_WAY);

				for (int rowin = 0; rowin < TREE_WAY; ++rowin) {
					for (int colin = 0; colin < TREE_WAY; ++colin) {
						int ixin = rowin * TREE_WAY + colin;
						if (l1 < levels) {
							nodes[l][ix][ixin] = new TreeNode(this, l1, row * TREE_WAY + rowin, col * TREE_WAY + colin);
						} else {
							nodes[l][ix][ixin] = new TreeLeaf(this, row * TREE_WAY + rowin, col * TREE_WAY + colin);
						}
					}
				}
			}
		}
	}

    // The width of quadrans in the last level
    qreal leavesEdge = getLevelEdge(levels);
    qreal leafWidth = boundaries.width() / (qreal) leavesEdge;
    // Fill up the nodes
    foreach (Node* node, nodeVector) {
        QPointF pos = node->pos();
        int row = floor((pos.x() - boundaries.left()) / leafWidth);
        int col = floor((pos.y() - boundaries.top()) / leafWidth);

		if (row >= leavesEdge) {
			row = leavesEdge - 1;
		}
		if (col >= leavesEdge) {
			col = leavesEdge - 1;
		}

        for (int l = 0; l < levels; ++l) {
            int l1 = l + 1;
            int levelEdge = getLevelEdge(l1);
            int rowin = (int) ceil((levelEdge * row) / leavesEdge);
            int colin = (int) ceil((levelEdge * col) / leavesEdge);
            int rowout = rowin / (TREE_WAY * TREE_WAY);
            int colout = colin / (TREE_WAY * TREE_WAY);

            int ixout = rowout * ipow(TREE_WAY, l) + colout;
            int ixin = (rowout % TREE_WAY) * TREE_WAY + (colout % TREE_WAY);

            if (l < levels - 1) {
                dynamic_cast<TreeNode*>(nodes[l][ixout][ixin])->addNode();
            } else {
                dynamic_cast<TreeLeaf*>(nodes[l][ixout][ixin])->addNode(node);
            }
        }
    }
}

int TreeCode::getLevelEdge(int l)
{
    return ipow(TREE_WAY, l);
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

QPointF getQuadrantCenter(QRectF boundaries, int treeWay, int level, int row, int col)
{
    qreal edgeWidth = boundaries.x() / ipow(treeWay, level);
    qreal x = (col * edgeWidth) + (edgeWidth / 2);
    qreal y = (row * edgeWidth) + (edgeWidth / 2);
    return QPointF(x, y);
}

TreeNode::TreeNode(TreeCode* tree, int level, int row, int col) :
	tree(tree),
	level(level),
	row(row),
	col(col),
	size(0)
{
}

int TreeNode::getSize()
{
    return 0;
}

QPointF TreeNode::getCenter()
{
    return getQuadrantCenter(tree->boundaries, tree->TREE_WAY, level, row, col);
}

QVector<TreeObject*>* TreeNode::getChildren()
{
    int edgeWidth = ipow(this->tree->TREE_WAY, this->level);
    return &this->tree->nodes[this->level + 1][this->row * edgeWidth + this->col];
}

void TreeNode::addNode()
{
    this->size++;
}

TreeLeaf::TreeLeaf(TreeCode* tree, int row, int col) :
    nodes(QVector<TreeObject*>()),
    tree(tree),
    row(row),
    col(col)
{
}

int TreeLeaf::getSize()
{
	return nodes.size();
}

QPointF TreeLeaf::getCenter()
{
	return getQuadrantCenter(tree->boundaries, tree->TREE_WAY, tree->levels, row, col);
}

void TreeLeaf::addNode(Node* node)
{
	nodes.append(node);
}

QVector<TreeObject*>* TreeLeaf::getChildren()
{
	return &nodes;
}
