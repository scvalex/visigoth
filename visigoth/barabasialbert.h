#ifndef BARABASIALBERT_H
#define BARABASIALBERT_H

#include "algorithm.h"
#include "ui_barabasialbert.h"
#include "edge.h"
#include "node.h"

#include <QList>
#include <QMap>
#include <QVector>

class GraphScene;
class QWidget;

class BarabasiAlbert : public Algorithm {

    Q_OBJECT

public:
    BarabasiAlbert(GraphScene *graph);
    ~BarabasiAlbert();

    void reset();
    bool canAddVertex();
    void addVertex();
    QWidget* controlWidget(QWidget *parent = 0);
    Ui::BarabasiControl* getUi();

protected slots:
    void onSizeChanged(int newSize);
    void onDegreeChanged(int newDegree);

protected:
    void addVertex(bool saveSize = false);
    void addVertex(int edgesToAdd);

    double genRandom();
    Node* getPreference(const QVector<Node*> &nodes, double genPref);
    void updatePreference(const QVector<Node*> &nodes, int numEdges);

private:
    static const int START_NODES = 300;
    static const int START_DEGREE = 3;
    GraphScene *graph;
    QWidget *ctlW;

    // used for selecting a node by preferential seleciton
    QMap<int, double> cumulativePreferences;
    // used for display purposes only
    QMap<int, double> preferences;

    int size;
    int nodeDegree;
    Ui::BarabasiControl* baCtrl;
};

#endif // BARABASIALBERT_H
