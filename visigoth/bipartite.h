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

    void reset();
    void addVertex();
    QWidget* newControlWidget(QWidget *parent = 0);

    void init(int vSize, int uSize);
    void showBipartite();

private slots:
    void onUSizeChanged(int newSize);
    void onVSizeChanged(int newSize);

private:
    // Both preference funcs will only be used on vector set U
    int getPreference(double genPref);
    void updatePreference();

    double degreeDist(int x);
    double fitnessDist(int x);

    QVector<Node*> vVector;
    QVector<Node*> uVector;
    QMap<int, double> cumulativePreferences;

    int uSize;
    int vSize;

    GraphScene *scene;
};

#endif // BIPARTITE_H
