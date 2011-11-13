#ifndef TWITTER_H
#define TWITTER_H

#include "algorithm.h"

#include <QObject>

class GraphScene;
class QWidget;

namespace QOAuth {
    class Interface;
};

namespace Ui {
    class TwitterAuthDialog;
};

class Twitter : public Algorithm
{
    Q_OBJECT
public:
    explicit Twitter(GraphScene *scene = 0);
    virtual ~Twitter();

    void reset();
    void addVertex();
    QWidget* controlWidget(QWidget *parent);

    bool login();

private:
    QOAuth::Interface *oauth;
    Ui::TwitterAuthDialog *authD;
    QByteArray token;
    QByteArray tokenSecret;
};

#endif // TWITTER_H
