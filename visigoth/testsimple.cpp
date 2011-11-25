#include <QtTest/QtTest>
#include <QObject>

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
        scene = new GraphScene(0);
    }

    void nopTest() {
        QVERIFY(1 == 1);
    }

    void setAlgoTest() {
    }

    void cleanupTestCase() {
        delete scene;
    }

private:
    GraphScene *scene;
};

QTEST_MAIN(TestSimple)
#include "testsimple.moc"
