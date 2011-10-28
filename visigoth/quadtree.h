#ifndef QUADTREE_H
#define QUADTREE_H

#include <QRectF>
#include <QVector>
#include <QPointF>

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

    static const int BASE_QUADRANT_SIZE = 30;

    QuadTree(QRectF boundaries);
    ~QuadTree();
    void addNode(TreeNode& node);
    TreeNode& root() const;

    void printTree(TreeNode* node); // Debug

private:
    class Quadrant : public TreeNode {
    public:
        Quadrant(int level, QPointF center, int edge);
        ~Quadrant();
        int size() const;
        QPointF center() const;
        bool hasChildren() const;
        const QVector<TreeNode*>& children() const;
        qreal width() const;

        void addChild(TreeNode& child);

        // Both this functions are needed by printTree() only, which is useful to debug.
        int getLevel() const;
        QPointF getQuadrantCenter() const;

    private:
        QVector<TreeNode*> _children;
        int level;
        QPointF quadrantCenter;
        int _width;
        int _size;
        QPointF _center;

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
