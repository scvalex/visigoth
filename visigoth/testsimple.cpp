#include <QDebug>
#include <QList>
#include <QObject>
#include <QString>
#include <QtTest/QtTest>
#include <QTest>

#include <math.h>

#include "algorithm.h"
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

    void setAlgo_data() {
        setAlgoNames();
    }

    void setAlgo() {
        QFETCH(QString, algoName);
        scene->chooseAlgorithm(algoName);
    }

    void addNode_data() {
        setAlgoNames();
    }

    void addNode() {
        QFETCH(QString, algoName);

        scene->chooseAlgorithm(algoName);
        if (!scene->algorithm()->canAddVertex())
            return;

        int count = scene->nodes().size();
        scene->addVertex();
        QVERIFY(count + 1 == scene->nodes().size());
    }

    void statsSimple_data() {
        setAlgoNames();
    }

    void statsSimple() {
        QFETCH(QString, algoName);

        scene->chooseAlgorithm(algoName);
        if (!scene->algorithm()->canAddVertex())
            return;

        scene->addVertex();
        double val = scene->getStatistics()->degreeAvg();
        QVERIFY(fpclassify(val) == FP_NORMAL && val > 0);
        val = scene->getStatistics()->lengthAvg();
        QVERIFY(fpclassify(val) == FP_NORMAL && val > 0);
        val = scene->getStatistics()->clusteringAvg();
        QVERIFY(fpclassify(val) == FP_NORMAL && val > 0);
    }

    void hasControlWidget_data() {
        setAlgoNames();
    }

    void hasControlWidget() {
        QFETCH(QString, algoName);

        scene->chooseAlgorithm(algoName);
        QVERIFY(0 != scene->algorithm()->controlWidget(0));
    }

    void cleanup() {
        delete scene;
    }

private:
    GraphScene *scene;

    void setAlgoNames() {
        QTest::addColumn<QString>("algoName");

        QTest::newRow("Barabasi Albert") << "Barabasi Albert";
        QTest::newRow("Erdos Renyi") << "Erdos Renyi";
        QTest::newRow("Preferential Attachament") << "Preferential Attachament";
        QTest::newRow("Watts Strogatz") << "Watts Strogatz";
    }
};

QTEST_MAIN(TestSimple)
#include "testsimple.moc"
