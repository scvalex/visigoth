#ifndef QUADTREE_H
#define QUADTREE_H

#include <QVector>

#include "vtools.h"


class QuadTree
{
public:
    class TreeNode {
    public:
        virtual int size() const = 0;
        virtual VPointF center() const = 0;
        virtual bool hasChildren() const = 0;
        virtual const QVector<TreeNode*>& children() const = 0;
        virtual vreal width() const = 0;

        // Checks if a node is "far enough", that is if we should calculate the force based
        // on the current node.
        bool isFarEnough(vreal distance);

        // A fixed tolerance. The higher the tolerance, the more unstable the graph.
        static const vreal tolerance = 0.2;
    };

    QuadTree(vreal longestEdge);
    ~QuadTree();
    void addNode(TreeNode& node);
    TreeNode& root() const;

    void printTree(TreeNode* node) const; // Debug

private:
    class Quadrant : public TreeNode {
    public:
        Quadrant(int level, VPointF center, int edge);
        ~Quadrant();
        int size() const;
        VPointF center() const;
        bool hasChildren() const;
        const QVector<TreeNode*>& children() const;
        vreal width() const;

        void addChild(TreeNode& child);

        // Both this functions are needed by printTree() only, which is useful to debug.
        int getLevel() const;
        // While center() returns the center of gravity, based on the contained nodes,
        // this returns the center of the original quadrant.
        VPointF getQuadrantCenter() const;

    private:
        QVector<TreeNode*> _children;
        int level;
        VPointF quadrantCenter;
        int _width;
        int _size;
        VPointF _center;

        int childIndex(TreeNode& node) const;
        void castAndAddChild(TreeNode* node, TreeNode& child) const;
        bool isTerminal();
        VPointF weightedMiddle(TreeNode& node1, TreeNode& node2) const;
        void allocateChildren();
        void addChildToChildren(TreeNode& node);
    };

    Quadrant* _root;
};

#endif // QUADTREE_H
