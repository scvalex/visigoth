#ifndef TWITTER_H
#define TWITTER_H

#include "algorithm.h"

#include <QSet>
#include <QString>

class GraphScene;

class QNetworkAccessManager;
class QNetworkReply;
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
    void getFollowers(QString username);

protected slots:
    void replyGot(QNetworkReply *reply);

private:
    QOAuth::Interface *oauth;
    Ui::TwitterAuthDialog *authD;
    QNetworkAccessManager *net;
    QByteArray token;
    QByteArray tokenSecret;
};

#endif // TWITTER_H
