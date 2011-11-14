#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>

class QWidget;

class Algorithm : public QObject {
    Q_OBJECT

public:
    Algorithm(QObject *parent = 0,bool flag = false);
    virtual ~Algorithm();

    virtual void reset() = 0;
    virtual void addVertex() = 0;
    virtual QWidget* controlWidget(QWidget *parent = 0) = 0;
    bool getSWNFlag();

private:
    bool swnFlag;
};

#endif // ALGORITHM_H
