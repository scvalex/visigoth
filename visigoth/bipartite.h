#ifndef BIPARTITE_H
#define BIPARTITE_H

#include "algorithm.h"
#include "edge.h"
#include "node.h"

#include <QList>
#include <QMap>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtCore/qmath.h>

class GraphScene;
class Bipartite;
class QWidget;

class Bipartite : public Algorithm {
    Q_OBJECT

public:
    Bipartite(GraphScene *scene);
    virtual ~Bipartite();

    void init(int size);
    void addVertex();
    QWidget* newControlWidget(QWidget *parent = 0);

    void init(int vSize, int uSize);
    void showBipartite();

private slots:
    void onUSizeChanged(const QString &newValue);

private:
    // Both preference funcs will only be used on vector set U
    int getPreference(double genPref);
    void updatePreference();

    double degreeDist(int x);
    double fitnessDist(int x);

    /* vVector and uVector exist since we have two disjoint sets
       During the graph generation */
    QVector<Node*> vVector;
    QVector<Node*> uVector;

    // used for selecting a node by preferential seleciton
    QMap<int, double> cumulativePreferences;
    GraphScene *scene;
};

#endif // BIPARTITE_H
