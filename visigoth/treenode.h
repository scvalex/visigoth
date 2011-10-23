#ifndef TREENODE_H
#define TREENODE_H

#include <QPointF>
#include <QVector>

class TreeNode
{
public:
    virtual int getSize() const = 0;
    virtual QPointF getCenter() const = 0;
    virtual QVector<TreeNode*>& getChildren() = 0;
    virtual qreal getWidth() const = 0;

    bool isFarEnough(qreal distance);
    static const qreal tolerance = 0;
};

#endif // TREENODE_H
