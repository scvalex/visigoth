#ifndef TWITTER_H
#define TWITTER_H

#include "algorithm.h"

#include <QObject>

class GraphScene;
class QWidget;

class Twitter : public Algorithm
{
    Q_OBJECT
public:
    explicit Twitter(GraphScene *scene = 0);
    virtual ~Twitter();

    void reset();
    void addVertex();
    QWidget* controlWidget(QWidget *parent);
};

#endif // TWITTER_H
