#ifndef TREECODE_H
#define TREECODE_H

#include "node.h"

#include <QVector>

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
	static const int TREE_WAY 2
	// The minimun size of a quadrant
	static const int QUADRANT_MIN_SIZE = 25;
	// The maximum number of levels
	static const int MAX_LEVELS = 15;

	// The total number of elements
	int size;
	QRectF boundaries;
	// The total level of decomposition
	int levels;
	// This contains all the "leaves" (the nodes in the last level of decomposition).
	QVector<QVector<TreeLeaf*> > leaves;
	// Containing all the TreeNodes, for each level.
	QVector<QVector<QVector<TreeNode*> > > nodes;

	int getLevelEdge(int l);

	friend class TreeNode;
};

class TreeObject
{
public:
	virtual ~TreeObject() {}
	virtual int getSize() = 0;
};

class TreeNode : public TreeObject
{
	int getSize();
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
	TreeLeaf();
	int getSize();
	QVector<Node*>& getNodes();
	void addNode(Node* node);
private:
	QVector<Node*> nodes;
};

#endif //TREECODE_H
