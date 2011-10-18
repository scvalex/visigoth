#ifndef TREEOBJECT_H
#define TREEOBJECT_H

#include <QPointF>
#include <QVector>

class TreeNode
{
public:
	virtual int getSize() = 0;
	virtual QPointF getCenter() = 0;
	virtual QVector<TreeNode*>& getChildren() = 0;
	virtual qreal getWidth() = 0;

    bool isFarEnough(qreal distance);
    static const qreal tolerance = 0;
};

#endif // TREEOBJECT_H
