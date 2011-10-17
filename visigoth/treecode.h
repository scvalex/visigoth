#ifndef TREECODE_H
#define TREECODE_H

#include "node.h"
#include "treeobject.h"

#include <QVector>
#include <QGraphicsItem>

class TreeCode
{
public:
	TreeCode(QVector<Node*>& nodeVector, QRectF boundaries);
	~TreeCode();
	TreeObject& getRoot();
private:

    class Branch : public TreeObject
    {
    public:
        Branch(TreeCode* tree, int level, int row, int col, int size=5);
        int getSize();
        QPointF getCenter();
        QVector<TreeObject*>* getChildren();
        QVector<Node*>* getNodes();
        void addNode(Node*);
    private:
        TreeCode* tree;
        int level;
        int row;
        int col;
        int size;
    };

    class Leaf : public TreeObject
    {
    public:
        Leaf(TreeCode* tree, int row, int col);
        int getSize();
        QPointF getCenter();
        QVector<TreeObject*>* getChildren();
        QVector<Node*>* getNodes();
        void addNode(Node* node);
    private:
        QVector<Node*> nodes;
        TreeCode* tree;
        int row;
        int col;
    };

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
	// Containing all the Branchs, and the TreeLeaves at the last level.
	// The first vector holds the different levels.
	// The seconds holds groups TREE_WAY^(l-1) cells, which in turn contain TREE_WAY * TREE_WAY
	// TreeObjects. The TreeObjects are stored like this so that the given one tree object
	// we can easily get the 4 children.
	QVector<QVector<QVector<TreeObject*> > > nodes;
	// Utility node to start from
	TreeCode::Branch root;

	// Utility functions
	QRectF squareBoundaries(QRectF boundaries);
	int calculateLevels(qreal edge);
	void allocateNodes();
	void fillNodes(QVector<Node*>& nodeVector);
	int getLevelQuadrants(int l);

    friend class TreeCode::Branch;
    friend class TreeCode::Leaf;
};

#endif // TREECODE_H
