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

namespace Ui {
    class BipartiteControl;
};

class Bipartite : public Algorithm {
    Q_OBJECT

public:
    Bipartite(GraphScene *scene);
    virtual ~Bipartite();

    int getUSize() const;
    int getVSize() const;

    void reset();
    bool canAddVertex();
    void addVertex();
    QWidget* controlWidget(QWidget *parent = 0);

private slots:
    void onUSizeChanged(int newSize);
    void onVSizeChanged(int newSize);

private:
    static const int START_USIZE = 40;
    static const int START_VSIZE = 1;
    QWidget *ctlW;

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
    Ui::BipartiteControl *bipartiteCtl;

    void updateUI();
};

#endif // BIPARTITE_H
