#include "algorithm.h"

Algorithm::Algorithm(QObject *parent,bool flag) :
    QObject(parent),
    swnFlag(flag)
{
}

Algorithm::~Algorithm() {
}

bool Algorithm::getSWNFlag(){
    return swnFlag;
}
