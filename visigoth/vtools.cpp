#include <cmath>        // sqrt
#include <QtGlobal>     // qMax, qAbs

#include "vtools.h"


VPointF::VPointF(vreal newX, vreal newY, vreal newZ) :
        x(newX), y(newY), z(newZ)
{ }

VPointF::VPointF(vreal newX, vreal newY) :
        x(newX), y(newY), z(0.0)
{ }

VPointF::VPointF(vreal newXYZ) :
        x(newXYZ), y(newXYZ), z(newXYZ)
{ }


vreal VPointF::lengthSquared() {
    return (x*x) + (y*y) + (z*z);
}

vreal VPointF::length() {
    return sqrt(lengthSquared());
}


VPointF operator + (const VPointF &p1, const VPointF &p2) {
    return VPointF(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

VPointF operator - (const VPointF &p1, const VPointF &p2) {
    return VPointF(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

VPointF operator * (const VPointF &p, const vreal scale) {
    return VPointF(p.x*scale, p.y*scale, p.z*scale);
}
VPointF operator * (const vreal scale, const VPointF &p) {
    return (p * scale);
}

VPointF operator / (const VPointF &p, const vreal scale) {
    return VPointF(p.x/scale, p.y/scale, p.z/scale);
}

bool operator == (const VPointF &p1, const VPointF &p2) {
    return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}




VCubeF::VCubeF(VPointF newP1, VPointF newP2) :
        p1(newP1), p2(newP2)
{ }

vreal VCubeF::longestEdge() {
    return qMax(qMax(qAbs(p2.x - p1.x), qAbs(p2.y - p1.y)), qAbs(p2.z - p1.z));
}
