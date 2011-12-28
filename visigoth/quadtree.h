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
        virtual const QVector<TreeNode*>& children() const = 0;
        virtual vreal width() const = 0;

        // Checks if a node is "far enough", that is if we should calculate the force based
        // on the current node.
        bool isFarEnough(vreal distance);

        // A fixed tolerance. The higher the tolerance, the more unstable the graph.
        static const vreal tolerance = 0.8;
    };


    QuadTree(vreal longestEdge);
    ~QuadTree();
    void addNode(TreeNode *node);
    TreeNode& root() const;

    void printTree(TreeNode *node) const; // Debug

private:
    class Quadrant : public TreeNode {
    public:
        Quadrant(int level, VPointF center, int edge);
        ~Quadrant();
        int size() const;
        VPointF center() const;
        const QVector<TreeNode*>& children() const;
        vreal width() const;

        static const int CHILDREN = 8;

        void addChild(TreeNode *child);

        // Both this functions are needed by printTree() only, which is useful to debug.
        int getLevel() const;
        // While center() returns the center of gravity, based on the contained nodes,
        // this returns the center of the original quadrant.
        VPointF getQuadrantCenter() const;

    private:
        enum CUBE_X {
            LEFT = -1,
            RIGHT = 1
        };

        enum CUBE_Y {
            TOP = -1,
            BOTTOM = 1
        };

        enum CUBE_Z {
            BACK = -1,
            FRONT = 1
        };

        QVector<TreeNode*> _children;
        int level;
        VPointF quadrantCenter;
        int _width;
        int _size;
        VPointF _center;

        int childIndex(TreeNode& node) const;
        void castAndAddChild(TreeNode *node, TreeNode *child) const;
        bool isTerminal() const;
        VPointF weightedMiddle(TreeNode *node1, TreeNode *node2) const;
        int getIndex(int x, int y, int z) const;
        void allocateChildren();
        void addChildToChildren(TreeNode *node);
    };

    Quadrant* _root;
};

#endif // QUADTREE_H
