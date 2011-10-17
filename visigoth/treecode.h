#ifndef TREECODE_H
#define TREECODE_H

#include "node.h"
#include "treeobject.h"

#include <QVector>
#include <QGraphicsItem>

class TreeObject;
class TreeNode;
class TreeLeaf;

class TreeCode
{
public:
	TreeCode(QVector<Node*>& nodeVector, QRectF boundaries);
	~TreeCode();
	int getSize();
private:
	// The size of the edge of each decomposition (for a quad-tree it'll be 2)
	static const int TREE_WAY = 2;
	// The minimun size of a quadrant
	static const int QUADRANT_MIN_SIZE = 25;
	// The maximum number of levels
	static const int MAX_LEVELS = 15;

	// The total number of elements
	int size;
	QRectF boundaries;
	// The total level of decomposition
	int levels;
	// Containing all the TreeNodes, and the TreeLeaves at the last level.
	// The first vector holds the different levels.
	// The seconds holds groups TREE_WAY^(l-1) cells, which in turn contain TREE_WAY * TREE_WAY
	// TreeObjects. The TreeObjects are stored like this so that the given one tree object
	// we can easily get the 4 children.
	QVector<QVector<QVector<TreeObject*> > > nodes;

	int getLevelEdge(int l);

    friend class TreeNode;
    friend class TreeLeaf;
};

class TreeNode : public TreeObject
{
	int getSize();
	QPointF getCenter();
	virtual QVector<TreeObject*>* getChildren();
private:
	TreeNode(TreeCode* tree, int level, int row, int col);
	void addNode();

	TreeCode* tree;
	int level;
	int row;
	int col;
	int size;

	friend class TreeCode;
};

class TreeLeaf : public TreeObject
{
public:
	int getSize();
	QPointF getCenter();
	QVector<TreeObject*>* getChildren();
	void addNode(Node* node);
private:
	TreeLeaf(TreeCode* tree, int row, int col);

    QVector<TreeObject*> nodes;
    TreeCode* tree;
    int row;
    int col;

    friend class TreeCode;
};

#endif // TREECODE_H
