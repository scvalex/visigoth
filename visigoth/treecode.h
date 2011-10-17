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
	QVector<QVector<TreeLeaf*> > leaves;
	// Containing all the TreeNodes, and the TreeLeaves at the last level.
	QVector<QVector<QVector<TreeNode* > > > nodes;

	int getLevelEdge(int l);

	friend class TreeNode;
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
	TreeLeaf();
	int getSize();
	QPointF getCenter();
	QVector<TreeObject*>* getChildren();
	void addNode(Node* node);
private:
	QVector<Node*> nodes;
};

#endif // TREECODE_H
