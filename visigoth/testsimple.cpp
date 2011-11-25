#include <QtTest/QtTest>
#include <QObject>

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
    }

    void nopTest() {
        QVERIFY(1 == 1);
    }

    void cleanupTestCase() {
    }
};

QTEST_MAIN(TestSimple)
#include "testsimple.moc"
