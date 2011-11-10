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

        // Checks if a node is "far enough", that is if we should calculate the force based
        // on the current node.
        bool isFarEnough(qreal distance);

        // A fixed tolerance. The higher the tolerance, the more unstable the graph.
        static const qreal tolerance = 0;
    };

    // The size of the smallest quadrants.
    static const int BASE_QUADRANT_SIZE = 30;

    QuadTree(QRectF boundaries);
    ~QuadTree();
    void addNode(TreeNode& node);
    TreeNode& root() const;

    void printTree(TreeNode* node) const; // Debug

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
        // While center() returns the center of gravity, based on the contained nodes,
        // this returns the center of the original quadrant.
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
        QPointF weightedMiddle(TreeNode& node1, TreeNode& node2) const;
        void allocateChildren();
        void addChildToChildren(TreeNode& node);
    };

    Quadrant* _root;

    int calculateWidth(QRectF boundaries) const;
};

#endif // QUADTREE_H
