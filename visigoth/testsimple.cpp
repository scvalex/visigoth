#include <QDebug>
#include <QList>
#include <QObject>
#include <QString>
#include <QtTest/QtTest>

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
        normalAlgorithms << "Barabasi Albert" << "Erdos Renyi" << "Preferential Attachament" << "Watts Strogatz";
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

    void setAlgo() {
        foreach (QString algoName, normalAlgorithms) {
            qDebug() << "Testing" << algoName;
            scene->chooseAlgorithm(algoName);
        }
    }

    void addNode() {
        foreach (QString algoName, normalAlgorithms) {
            qDebug() << "Testing" << algoName;
            addNodeToAlgo(algoName);
        }
    }

    void statsSimple() {
        foreach (QString algoName, normalAlgorithms) {
            statsSimple(algoName);
        }
    }

    void cleanup() {
        delete scene;
    }

private:
    GraphScene *scene;
    QList<QString> normalAlgorithms;

    void addNodeToAlgo(QString name) {
        scene->chooseAlgorithm(name);
        if (!scene->algorithm()->canAddVertex())
            return;

        int count = scene->nodes().size();
        scene->addVertex();
        QVERIFY(count + 1 == scene->nodes().size());
    }

    void statsSimple(QString name) {
        scene->chooseAlgorithm(name);
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
};

QTEST_MAIN(TestSimple)
#include "testsimple.moc"
