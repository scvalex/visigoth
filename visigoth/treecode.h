#ifndef TREECODE_H
#define TREECODE_H

#include "treenode.h"

#include <QVector>
#include <QGraphicsItem>

class TreeCode
{
public:
	TreeCode(QRectF boundaries);
	~TreeCode();
	TreeNode* getRoot();
	QRectF getBoundaries();
	void addNode(TreeNode* node);
private:
    class Quadrant : public TreeNode
    {
    public:
        Quadrant(qreal width, int size=0);
        int getSize() const;
        QPointF getCenter() const;
        QVector<TreeNode*>& getChildren();
        void addNode(TreeNode*);
        void addChild(TreeNode*);
        qreal getWidth() const;
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
    // The quadrants in the tree, at the last level containing the proper quadrant
    QVector<QVector<QVector<Quadrant*> > > quadrants;

    // Utility functions
    QRectF squareBoundaries(QRectF boundaries);
    int calculateLevels(qreal edge);
    void allocateNodes();
    void allocateNode(int level, int row, int col);
    int getLevelQuadrants(int l);
};

#endif // TREECODE_H
