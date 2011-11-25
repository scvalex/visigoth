#include <QList>
#include <QObject>
#include <QString>
#include <QtTest/QtTest>

#include <math.h>

#include "graphscene.h"
#include "statistics.h"

class TestSimple : public QObject {
Q_OBJECT
public:
    TestSimple(QObject *parent = 0) :
        QObject(parent)
    {
    }

    virtual ~TestSimple() {
    }

private slots:
    void init() {
        scene = new GraphScene();
    }

    void nopTest() {
        QVERIFY(1 == 1);
    }

    void setAlgoBipartiteModel() {
        scene->chooseAlgorithm("Bipartite Model");
    }

    void setAlgoPreferentialModel() {
        scene->chooseAlgorithm("Preferential Attachament");
    }

    void setAlgoErdosRenyi() {
        scene->chooseAlgorithm("Erdos Renyi");
    }

    void setAlgoBarabasiAlbert() {
        scene->chooseAlgorithm("Barabasi Albert");
    }

    void addNodeBarabasiAlbert() {
        addNodeToAlgo("Barabasi Albert");
    }

    void addNodeErdosRenyi() {
        addNodeToAlgo("Erdos Renyi");
    }

    void addNodePreferential() {
        addNodeToAlgo("Preferential Attachament");
    }

    void statsSimpleBarabasiAlbert() {
        statsSimple("Barabasi Albert");
    }

    void statsSimpleErdosRenyi() {
        statsSimple("Erdos Renyi");
    }

    void statsSimplePreferential() {
        statsSimple("Preferential Attachament");
    }

    void cleanup() {
        delete scene;
    }

private:
    GraphScene *scene;

    void addNodeToAlgo(QString name) {
        scene->chooseAlgorithm(name);
        int count = scene->nodes().size();
        scene->addVertex();
        QVERIFY(count + 1 == scene->nodes().size());
    }

    void statsSimple(QString name) {
        scene->chooseAlgorithm(name);
        scene->addVertex();
        double val = scene->getStatistics()->degreeAvg();
        QVERIFY(fpclassify(val) == FP_NORMAL && val > 0);
        val = scene->getStatistics()->lengthAvg();
        QVERIFY(fpclassify(val) == FP_NORMAL && val > 0);
        val = scene->getStatistics()->clusteringAvg();
        QVERIFY(fpclassify(val) == FP_NORMAL && val > 0);
    }

};

QTEST_MAIN(TestSimple)
#include "testsimple.moc"
