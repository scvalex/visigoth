#ifndef TREEOBJECT_H
#define TREEOBJECT_H

#include <QPointF>
#include <QVector>

class TreeObject
{
public:
	virtual ~TreeObject() {}
	virtual int getSize() = 0;
	virtual QPointF getCenter() = 0;
	virtual QVector<TreeObject*>* getChildren() = 0;
};

#endif // TREEOBJECT_H
