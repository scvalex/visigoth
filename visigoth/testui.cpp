#include <QList>
#include <QObject>
#include <QString>
#include <QtTest/QtTest>
#include <QAction>
#include <QStringList>

#include <math.h>

#include "preferential.h"
#include "bipartite.h"
#include "erdosrenyi.h"
#include "statistics.h"
#include "barabasialbert.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_helpWidget.h"
#include "ui_statistics.h"

class TestUi : public QObject {
Q_OBJECT

public:

    TestUi(QObject* parent = 0) :
        QObject(parent)
    {
    }

    virtual ~TestUi(){
    }

private slots:

    void init(){
        main = new MainWindow();
        glWidget = main->getGlWidget();
        scene = main->getScene();
    }

    // Test if key A adds a node
    void testKeyA(){

        int countBefore = scene->nodes().size();
        QTest::keyEvent(QTest::Click,glWidget,Qt::Key_A);
        QVERIFY2(countBefore + 1 == scene->nodes().size(), "Error in addNode process");
    }

    // test if Key Space pauses the animation
    void testKeySpace(){

        QTest::keyEvent(QTest::Click,glWidget,Qt::Key_Space);
        QVERIFY2(!glWidget->getIsPlaying(), "Error in is pausing with space key");
    }

    // tests if the newNodeAct emmits the signal triggered when triggered
    void testAddNodeAct(){
        QSignalSpy spy(main->getUi()->newNodeAct, SIGNAL(triggered()));

        int count = scene->nodes().size();

        main->getUi()->newNodeAct->activate(QAction::Trigger);

        QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
        QVERIFY2(count + 1 == scene->nodes.size(), "AddNode() was not triggered");

    }

    // tests if the generateAct emmits the signal triggered when triggered
    void testGenerateAct(){
        QSignalSpy spy(main->getUi()->generateAct, SIGNAL(triggered()));
        main->getUi()->generateAct->activate(QAction::Trigger);

        QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

    }


    // tests if the randomizeAct emmits the signal triggered when triggered
    void testRandomizeAct(){
        QSignalSpy spy(main->getUi()->randomizeAct, SIGNAL(triggered()));
        main->getUi()->randomizeAct>activate(QAction::Trigger);

        QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

    }

    void testLengthLabel(){

        scene->repopulate();
        scene->addVertex();
        Q_ASSERT(QString::number(scene->getStatistics()->lengthAvg()) == main->getStatsUi()->lengthLabel);

    }

    void testExponentLabel(){

        scene->repopulate();
        scene->addVertex();
        Q_ASSERT(QString::number(scene->getStatistics()->powerLawExponent()) == main->getStatsUi()->exponentLabel);

    }

    void testClusterLabel(){

        scene->repopulate();
        scene->addVertex();
        Q_ASSERT(QString::number(scene->getStatistics()->clusteringDegree()) == main->getStatsUi()->clusteringLabel);


    }

    void testDegreeLabel(){

        scene->repopulate();
        scene->addVertex();
        Q_ASSERT(QString::number(scene->getStatistics()->degreeAvg()) == main->getStatsUi()->degreeLabel);

    }

    void testBarabasiSize(){

        int size = 80;

        scene->chooseAlgorithm("Barabasi Albert");
        Algorithm* algo = scene->algorithm();
        BarabasiAlbert* ba = dynamic_cast<BarabasiAlbert*>(algo);
        Ui::BarabasiControl* baControl = ba->getUi();
        // fake user input here
        baControl->sizeEdit(size);
        Q_ASSERT(scene->nodes().size() == size);

    }

    void testBarabasiDegree(){

    }

    void testBipartiteSize(){

        int size = 80;

        scene->chooseAlgorithm("Bipartite Model");
        Algorithm* algo = scene->algorithm();
        Bipartite* ba = dynamic_cast<Bipartite*>(algo);
        Ui::BipartiteControl* bipControl = ba->getUi();
        // fake user input here
        bipControl->sizeEdit(size);
        Q_ASSERT(scene->nodes().size() == size);

    }

    void testPreferentialSize(){

        int size = 80;

        scene->chooseAlgorithm("Preferential Attachment");
        Algorithm* algo = scene->algorithm();
        Preferential* pref = dynamic_cast<Preferential*>(algo);
        Ui::PreferentialControl* prefControl = pref->getUi();
        // fake user input here
        prefControl->sizeEdit(size);
        Q_ASSERT(scene->nodes().size() == size);

    }

    void testErdosSize(){

        int size = 80;

        scene->chooseAlgorithm("Erdos Renyi");
        Algorithm* algo = scene->algorithm();
        ErdosRenyi* erdos = dynamic_cast<RedosRenyi*>(algo);
        Ui::ErdosControl* erdosControl = erdos->getUi();
        // fake user input here
        erdosControl->sizeEdit(size);
        Q_ASSERT(scene->nodes().size() == size);

    }


    void cleanUp(){
        delete main;
    }

private:

   MainWindow* main;
   GLGraphWidget* glWidget;
   GraphScene* scene;


};

QTEST_MAIN(TestUi)
#include "testui.moc"


