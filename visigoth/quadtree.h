#ifndef QUADTREE_H
#define QUADTREE_H

#include <QRectF>
#include <QVector>

class QuadTree
{
public:
    class TreeNode {
    public:
        virtual int weight() const = 0;
        virtual QPointF center() const = 0;
        virtual bool hasChildren() const = 0;
        virtual QVector<TreeNode> children() const = 0;
        virtual qreal width() const = 0;
    };

    static const int BASE_QUADRANT_SIZE = 25;

    QuadTree(QRectF boundaries);
    // void addNode(TreeNode node);
    // TreeNode root() const;

private:
    class Quadrant {
    public:
        Quadrant(int level, int x, int y, int edge);
        int weight() const;
        QPointF center() const;
        bool hasChildren() const;
        QVector<TreeNode*> children() const;
        qreal width() const;

        QPointF addChild(TreeNode& child);

    private:
        QVector<TreeNode*> _children;
        int level;
        int x;
        int y;
        int edge;
        int _weight;
        QPointF _center;
    };

    static const int TREE_WAY = 2;


    // The width of the edge of the square space
    int edge;
    // The root node
    // Quadrant _root;

    int calculateEdge(QRectF boundaries) const;
};

#endif // QUADTREE_H
