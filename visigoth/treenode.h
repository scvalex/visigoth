#ifndef TREEOBJECT_H
#define TREEOBJECT_H

#include <QPointF>
#include <QVector>

#include "node.h"

class TreeNode
{
public:
	virtual int getSize() = 0;
	virtual QPointF getCenter() = 0;
	virtual void addNode(Node*) = 0;
	// This will return NULL if the TreeObject is not a TreeNode
	virtual QVector<TreeNode*>* getChildren() = 0;
	// This will return NULL if the TreeObject is not a TreeLeaf
	virtual QVector<Node*>* getNodes() = 0;
};

#endif // TREEOBJECT_H
