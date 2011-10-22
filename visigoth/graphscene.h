#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(QObject *parent = 0);
};

#endif // GRAPHSCENE_H
