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

    void controlWidgetBarabasi() {
        scene->chooseAlgorithm("Barabasi Albert");
        BarabasiAlbert *algo = (BarabasiAlbert*)scene->algorithm();

        QSpinBox *nodesSpin = algo->controlWidget()->findChild<QSpinBox*>("sizeEdit");
        QSpinBox *degreeSpin = algo->controlWidget()->findChild<QSpinBox*>("degreeEdit");

        int numNodes = algo->getNumNodes();
        int nodeDegree = algo->getNodeDegree();

        QCOMPARE(numNodes, nodesSpin->value());
        QCOMPARE(nodeDegree, degreeSpin->value());

        scene->addVertex();

        int numNodes1 = algo->getNumNodes();

        QCOMPARE(numNodes + 1, numNodes1);
        QCOMPARE(numNodes1, nodesSpin->value());

        QTest::keyClick(degreeSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(degreeSpin, "2");
        int nodeDegree1 = algo->getNodeDegree();

        QCOMPARE(2, nodeDegree1);
        QCOMPARE(numNodes1, algo->getNumNodes());
        QCOMPARE(numNodes1, nodesSpin->value());
        QCOMPARE(nodeDegree1, degreeSpin->value());

        QTest::keyClick(nodesSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(nodesSpin, "20");
        int numNodes2 = algo->getNumNodes();

        QCOMPARE(20, numNodes2);
        QCOMPARE(nodeDegree1, algo->getNodeDegree());
        QCOMPARE(nodeDegree1, degreeSpin->value());
        QCOMPARE(numNodes2, nodesSpin->value());
    }

    void controlWidgetBipartite() {
        scene->chooseAlgorithm("Bipartite Model");
        Bipartite *algo = (Bipartite*)scene->algorithm();

        QSpinBox *uSizeSpin = algo->controlWidget()->findChild<QSpinBox*>("uSizeEdit");
        QSpinBox *vSizeSpin = algo->controlWidget()->findChild<QSpinBox*>("vSizeEdit");

        int uSize = algo->getUSize();
        int vSize = algo->getVSize();

        QCOMPARE(uSize, uSizeSpin->value());
        QCOMPARE(vSize, vSizeSpin->value());

        QTest::keyClick(uSizeSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(uSizeSpin, "27");
        int uSize1 = algo->getUSize();

        QCOMPARE(27, uSize1);
        QCOMPARE(uSize1, uSizeSpin->value());
        QCOMPARE(vSize, vSizeSpin->value());

        QTest::keyClick(vSizeSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(vSizeSpin, "5");
        int vSize1 = algo->getVSize();

        QCOMPARE(5, vSize1);
        QCOMPARE(uSize1, uSizeSpin->value());
        QCOMPARE(vSize1, vSizeSpin->value());
    }

    void controlWidgetErdos() {
        scene->chooseAlgorithm("Erdos Renyi");
        ErdosRenyi *algo = (ErdosRenyi*)scene->algorithm();

        QSpinBox *nodesSpin = algo->controlWidget()->findChild<QSpinBox*>("nodesSpin");
        QDoubleSpinBox *probabilitySpin = algo->controlWidget()->findChild<QDoubleSpinBox*>("probabilitySpin");

        int numNodes = algo->getNumNodes();
        double probability = algo->getProbability();

        QCOMPARE(numNodes, nodesSpin->value());
        QCOMPARE(probability, probabilitySpin->value());

        scene->addVertex();

        int numNodes1 = algo->getNumNodes();

        QCOMPARE(numNodes + 1, numNodes1);
        QCOMPARE(numNodes1, nodesSpin->value());

        QTest::keyClick(probabilitySpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(probabilitySpin, "0.27");
        double probability1 = algo->getProbability();

        QCOMPARE(probability1, 0.27);
        QCOMPARE(numNodes1, algo->getNumNodes());
        QCOMPARE(numNodes1, nodesSpin->value());
        QCOMPARE(probability1, probabilitySpin->value());

        QTest::keyClick(nodesSpin, Qt::Key_A, Qt::ControlModifier);
        QTest::keyClicks(nodesSpin, "42");
        int numNodes2 = algo->getNumNodes();

        QCOMPARE(42, numNodes2);
        QCOMPARE(probability1, algo->getProbability());
        QCOMPARE(probability1, probabilitySpin->value());
        QCOMPARE(numNodes2, nodesSpin->value());
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
