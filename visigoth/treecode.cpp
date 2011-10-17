#include "treecode.h"

#include <QRectF>
#include <cmath>

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

	// std::cout << "Levels: " << levels << ", width: " << boundaries.x() << ", height: " << boundaries.y() << "\n";

	// Allocate the array of leaves
	int leavesEdge = getLevelEdge(levels);
	leaves.resize(leavesEdge);
	for (int i = 0; i < leavesEdge; ++i) {
		leaves[i].resize(leavesEdge);
		for (int j = 0; j < leavesEdge; ++j) {
			leaves[i][j] = new TreeLeaf();
		}
	}


	// Allocate the array of nodes
	nodes.resize(levels);
	for (int l = 0; l < levels; ++l) {
		int edge = getLevelEdge(l);
		nodes[l].resize(edge);
		for (int i = 0; i < edge; ++i) {
			nodes[l][i].resize(edge);
			for (int j = 0; j < edge; ++j) {
				nodes[l][i][j] = new TreeNode(this, l, i, j);
			}
		}
	}

	// The width of quadrans in the last level
	qreal leafWidth = boundaries.width() / (qreal) leavesEdge;
	// Position the nodes in the cells and nodes
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
		leaves[row][col]->addNode(node);

		for (int l = 0; l < levels; ++l) {
			int levelEdge = getLevelEdge(l);
			int rowl = (int) ceil((levelEdge * row) / leavesEdge);
			int coll = (int) ceil((levelEdge * col) / leavesEdge);

			nodes[l][rowl][coll]->addNode();
		}
	}
}

int TreeCode::getLevelEdge(int l)
{
	return (int) pow(TREE_WAY, l);
}

TreeCode::~TreeCode()
{
	for (int i = 0; i < getLevelEdge(levels); ++i) {
		for (int j = 0; j < getLevelEdge(levels); ++j) {
			delete leaves[i][j];
		}
	}

	for (int l = 0; l < levels; ++l) {
		for (int i = 0; i < getLevelEdge(l); ++i) {
			for (int j = 0; j < getLevelEdge(l); ++j) {
				delete nodes[l][i][j];
			}
		}
	}
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
	return QPointF();
}

QVector<TreeObject*>* TreeNode::getChildren()
{
	return NULL;
}

void TreeNode::addNode()
{
	this->size++;
}

TreeLeaf::TreeLeaf() :
	nodes(QVector<Node*>())
{
}

int TreeLeaf::getSize()
{
return nodes.size();
}

QPointF TreeLeaf::getCenter()
{
	return QPointF();
}

void TreeLeaf::addNode(Node* node)
{
	nodes.append(node);
}

QVector<TreeObject*>* TreeLeaf::getChildren()
{
	return (QVector<TreeObject*>*) &nodes;
}
