#ifndef QUADTREE_H
#define QUADTREE_H

#include <QRectF>
#include <QVector>

class QuadTree
{
public:
    class TreeNode {
    public:
        virtual int size() const = 0;
        virtual QPointF center() const = 0;
        virtual bool hasChildren() const = 0;
        virtual const QVector<TreeNode*>& children() const = 0;
        virtual qreal width() const = 0;

        bool isFarEnough(qreal distance);
        static const qreal tolerance = 0.2;
    };

    static const int BASE_QUADRANT_SIZE = 25;

    QuadTree(QRectF boundaries);
    ~QuadTree();
    void addNode(TreeNode& node);
    TreeNode& root() const;

private:
    class Quadrant : public TreeNode {
    public:
        Quadrant(int level, int x, int y, int edge);
        ~Quadrant();
        int size() const;
        QPointF center() const;
        bool hasChildren() const;
        const QVector<TreeNode*>& children() const;
        qreal width() const;

        void addChild(TreeNode& child);

    private:
        QVector<TreeNode*> _children;
        int level;
        int x;
        int y;
        int edge;
        int _size;
        QPointF _center;

        QPointF quadrantCenter() const;
        int childIndex(TreeNode& node) const;
        void castAndAddChild(TreeNode* node, TreeNode& child) const;
        bool isTerminal();
    };


    // The width of the edge of the square space
    int edge;
    // The root node
    Quadrant* _root;

    int calculateEdge(QRectF boundaries) const;
};

#endif // QUADTREE_H
