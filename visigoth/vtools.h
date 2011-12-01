#ifndef VTOOLS_H
#define VTOOLS_H

#include <QtGlobal>     // For qreal

typedef qreal vreal;


class VPointF {
public:
    VPointF(vreal newX, vreal newY, vreal newZ);
    VPointF(vreal newX, vreal newY);
    VPointF(vreal newXYZ);

    vreal x, y, z;

    vreal lengthSquared();
    vreal length();

private:
    friend VPointF operator + (const VPointF &p1, const VPointF &p2);
    friend VPointF operator - (const VPointF &p1, const VPointF &p2);
    friend VPointF operator * (const VPointF &p, const vreal scale);
    friend VPointF operator * (const vreal scale, const VPointF &p);
    friend VPointF operator / (const VPointF &p, const vreal scale);
    friend bool operator == (const VPointF &p1, const VPointF &p2);
};



class VCubeF {
public:
    VCubeF(VPointF newP1, VPointF newP2);

    VPointF p1, p2;

    vreal longestEdge();
};

#endif
