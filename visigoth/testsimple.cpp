#include <QList>
#include <QObject>
#include <QString>
#include <QtTest/QtTest>

#include "graphscene.h"

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
    void initTestCase() {
        scene = new GraphScene();
    }

    void nopTest() {
        QVERIFY(1 == 1);
    }

    void setAlgoTest() {
        QList<QString> algos = scene->algorithms();
        foreach (QString algo, algos) {
            scene->chooseAlgorithm(algo);
        }
    }

    void cleanupTestCase() {
        delete scene;
    }

private:
    GraphScene *scene;
};

QTEST_MAIN(TestSimple)
#include "testsimple.moc"
