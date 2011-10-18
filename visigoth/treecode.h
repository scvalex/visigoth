#ifndef TREECODE_H
#define TREECODE_H

#include "node.h"
#include "treenode.h"

#include <QVector>
#include <QGraphicsItem>

class TreeCode
{
public:
	TreeCode(QVector<Node*>& nodeVector);
	~TreeCode();
	TreeNode& getRoot();
	QRectF getBoundaries();
private:
    class Branch : public TreeNode
    {
    public:
        Branch(qreal width, int size=0);
        int getSize();
        QPointF getCenter();
        QVector<TreeNode*>& getChildren();
        void addNode(Node*);
        void addChild(TreeNode*);
        qreal getWidth();
    private:
        QVector<TreeNode*> children;
        qreal width;
        int size;
        QPointF center;
    };

    // The size of the edge of each decomposition (for a quad-tree it'll be 2)
    static const int TREE_WAY = 2;
    // The minimun size of a quadrant
    static const int QUADRANT_MIN_SIZE = 25;
    // The maximum number of levels
    static const int MAX_LEVELS = 15;

	QRectF boundaries;
	// The total level of decomposition
	int levels;
	// The branches in the tree, at the last level containing the proper nodes
	QVector<QVector<QVector<Branch*> > > nodes;

	Branch root;

	// Utility functions
	QRectF calculateBoundaries(QVector<Node*>& nodeVector);
	int calculateLevels(qreal edge);
	void allocateNodes();
	void fillNodes(QVector<Node*>& nodeVector);
	int getLevelQuadrants(int l);
	int flattenIndex(int rowWidth, int row, int col);
};

#endif // TREECODE_H
