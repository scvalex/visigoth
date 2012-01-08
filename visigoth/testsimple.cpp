#include <QDebug>
#include <QDoubleSpinBox>
#include <QList>
#include <QObject>
#include <QSpinBox>
#include <QString>
#include <QtTest/QtTest>
#include <QTest>

#include <math.h>

#include "algorithm.h"
#include "barabasialbert.h"
#include "bipartite.h"
#include "erdosrenyi.h"
#include "graphscene.h"
#include "preferential.h"
#include "statistics.h"
#include "wattsstrogatz.h"

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
        QCOMPARE(scene->nodes().size(), count + 1);
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

    void controlWidgetBarabasi() {
        scene->chooseAlgorithm("Barabasi Albert");
        BarabasiAlbert *algo = (BarabasiAlbert*)scene->algorithm();

        QSpinBox *nodesSpin = algo->controlWidget()->findChild<QSpinBox*>("sizeEdit");
        QSpinBox *degreeSpin = algo->controlWidget()->findChild<QSpinBox*>("degreeEdit");

        int numNodes = algo->getNumNodes();
        int nodeDegree = algo->getNodeDegree();

        QCOMPARE(nodesSpin->value(), numNodes);
        QCOMPARE(degreeSpin->value(), nodeDegree);

        scene->addVertex();

        int numNodes1 = algo->getNumNodes();

        QCOMPARE(numNodes1, numNodes + 1);
        QCOMPARE(nodesSpin->value(), numNodes1);

        QTest::keyClick(degreeSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(degreeSpin, "2");
        int nodeDegree1 = algo->getNodeDegree();

        QCOMPARE(nodeDegree1, 2);
        QCOMPARE(algo->getNumNodes(), numNodes1);
        QCOMPARE(nodesSpin->value(), numNodes1);
        QCOMPARE(degreeSpin->value(), nodeDegree1);

        QTest::keyClick(nodesSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(nodesSpin, "20");
        int numNodes2 = algo->getNumNodes();

        QCOMPARE(numNodes2, 20);
        QCOMPARE(algo->getNodeDegree(), nodeDegree1);
        QCOMPARE(degreeSpin->value(), nodeDegree1);
        QCOMPARE(nodesSpin->value(), numNodes2);
    }

    void controlWidgetBipartite() {
        scene->chooseAlgorithm("Bipartite Model");
        Bipartite *algo = (Bipartite*)scene->algorithm();

        QSpinBox *uSizeSpin = algo->controlWidget()->findChild<QSpinBox*>("uSizeEdit");
        QSpinBox *vSizeSpin = algo->controlWidget()->findChild<QSpinBox*>("vSizeEdit");

        int uSize = algo->getUSize();
        int vSize = algo->getVSize();

        QCOMPARE(uSizeSpin->value(), uSize);
        QCOMPARE(vSizeSpin->value(), vSize);

        QTest::keyClick(uSizeSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(uSizeSpin, "27");
        int uSize1 = algo->getUSize();

        QCOMPARE(uSize1, 27);
        QCOMPARE(uSizeSpin->value(), uSize1);
        QCOMPARE(vSizeSpin->value(), vSize);

        QTest::keyClick(vSizeSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(vSizeSpin, "5");
        int vSize1 = algo->getVSize();

        QCOMPARE(vSize1, 5);
        QCOMPARE(uSizeSpin->value(), uSize1);
        QCOMPARE(vSizeSpin->value(), vSize1);
    }

    void controlWidgetErdos() {
        scene->chooseAlgorithm("Erdos Renyi");
        ErdosRenyi *algo = (ErdosRenyi*)scene->algorithm();

        QSpinBox *nodesSpin = algo->controlWidget()->findChild<QSpinBox*>("nodesSpin");
        QDoubleSpinBox *probabilitySpin = algo->controlWidget()->findChild<QDoubleSpinBox*>("probabilitySpin");

        int numNodes = algo->getNumNodes();
        double probability = algo->getProbability();

        QCOMPARE(nodesSpin->value(), numNodes);
        QCOMPARE(probabilitySpin->value(), probability);

        scene->addVertex();

        int numNodes1 = algo->getNumNodes();

        QCOMPARE(numNodes1, numNodes + 1);
        QCOMPARE(nodesSpin->value(), numNodes1);

        QTest::keyClick(probabilitySpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(probabilitySpin, "0.27");
        double probability1 = algo->getProbability();

        QCOMPARE(probability1, 0.27);
        QCOMPARE(algo->getNumNodes(), numNodes1);
        QCOMPARE(nodesSpin->value(), numNodes1);
        QCOMPARE(probabilitySpin->value(), probability1);

        QTest::keyClick(nodesSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(nodesSpin, "42");
        int numNodes2 = algo->getNumNodes();

        QCOMPARE(numNodes2, 42);
        QCOMPARE(algo->getProbability(), probability1);
        QCOMPARE(probabilitySpin->value(), probability1);
        QCOMPARE(nodesSpin->value(), numNodes2);
    }

    void controlWidgetPreferential() {
        scene->chooseAlgorithm("Preferential Attachament");
        Preferential *algo = (Preferential*)scene->algorithm();

        QSpinBox *nodesSpin = algo->controlWidget()->findChild<QSpinBox*>("sizeEdit");
        QSpinBox *degreeSpin = algo->controlWidget()->findChild<QSpinBox*>("degreeEdit");

        int numNodes = algo->getNumNodes();
        int nodeDegree = algo->getNodeDegree();

        QCOMPARE(nodesSpin->value(), numNodes);
        QCOMPARE(degreeSpin->value(), nodeDegree);

        scene->addVertex();

        int numNodes1 = algo->getNumNodes();

        QCOMPARE(numNodes1, numNodes + 1);
        QCOMPARE(nodesSpin->value(), numNodes1);

        QTest::keyClick(degreeSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(degreeSpin, "2");
        int nodeDegree1 = algo->getNodeDegree();

        QCOMPARE(nodeDegree1, 2);
        QCOMPARE(algo->getNumNodes(), numNodes1);
        QCOMPARE(nodesSpin->value(), numNodes1);
        QCOMPARE(degreeSpin->value(), nodeDegree1);

        QTest::keyClick(nodesSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(nodesSpin, "20");
        int numNodes2 = algo->getNumNodes();

        QCOMPARE(numNodes2, 20);
        QCOMPARE(algo->getNodeDegree(), nodeDegree1);
        QCOMPARE(degreeSpin->value(), nodeDegree1);
        QCOMPARE(nodesSpin->value(), numNodes2);
    }

    void controlWidgetWatts() {
        scene->chooseAlgorithm("Watts Strogatz");
        WattsStrogatz *algo = (WattsStrogatz*)scene->algorithm();

        QSpinBox *nodesSpin = algo->controlWidget()->findChild<QSpinBox*>("nodeSpin");
        QSpinBox *degreeSpin = algo->controlWidget()->findChild<QSpinBox*>("degreeSpin");
        QDoubleSpinBox *probabilitySpin = algo->controlWidget()->findChild<QDoubleSpinBox*>("probSpin");

        int numNodes = algo->getNumNodes();
        int nodeDegree = algo->getNodeDegree();
        double probability = algo->getProbability();

        QCOMPARE(nodesSpin->value(), numNodes);
        QCOMPARE(degreeSpin->value(), nodeDegree);
        QCOMPARE(probabilitySpin->value(), probability);

        QTest::keyClick(degreeSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(degreeSpin, "2");
        int nodeDegree1 = algo->getNodeDegree();

        QCOMPARE(nodeDegree1, 2);
        QCOMPARE(algo->getNumNodes(), numNodes);
        QCOMPARE(nodesSpin->value(), numNodes);
        QCOMPARE(degreeSpin->value(), nodeDegree1);
        QCOMPARE(algo->getProbability(), probability);
        QCOMPARE(probabilitySpin->value(), probability);

        QTest::keyClick(nodesSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(nodesSpin, "20");
        int numNodes1 = algo->getNumNodes();

        QCOMPARE(numNodes1, 20);
        QCOMPARE(algo->getNodeDegree(), nodeDegree1);
        QCOMPARE(degreeSpin->value(), nodeDegree1);
        QCOMPARE(nodesSpin->value(), numNodes1);
        QCOMPARE(algo->getProbability(), probability);
        QCOMPARE(probabilitySpin->value(), probability);

        QTest::keyClick(probabilitySpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(probabilitySpin, "0.13");
        double probability1 = algo->getProbability();

        QCOMPARE(probability1, 0.13);
        QCOMPARE(algo->getNodeDegree(), nodeDegree1);
        QCOMPARE(degreeSpin->value(), nodeDegree1);
        QCOMPARE(algo->getNumNodes(), numNodes1);
        QCOMPARE(nodesSpin->value(), numNodes1);
        QCOMPARE(probabilitySpin->value(), probability1);
    }

    void cleanup() {
        delete scene;
    }

private:
    GraphScene *scene;

    void setAlgoNames() {
        QTest::addColumn<QString>("algoName");

        QTest::newRow("Barabasi Albert") << "Barabasi Albert";
        QTest::newRow("Bipartite Model") << "Bipartite Model";
        QTest::newRow("Erdos Renyi") << "Erdos Renyi";
        QTest::newRow("Preferential Attachament") << "Preferential Attachament";
        QTest::newRow("Watts Strogatz") << "Watts Strogatz";
    }
};

QTEST_MAIN(TestSimple)
#include "testsimple.moc"
