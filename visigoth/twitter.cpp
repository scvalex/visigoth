#include "graphscene.h"
#include "twitter.h"

#include <QWidget>

Twitter::Twitter(GraphScene *scene) :
    Algorithm(scene)
{
}

Twitter::~Twitter() {
}

void Twitter::reset() {
}

void Twitter::addVertex() {
}

QWidget* Twitter::controlWidget(QWidget *parent) {
    return 0;
}
