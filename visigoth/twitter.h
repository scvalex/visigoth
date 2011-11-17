#ifndef TWITTER_H
#define TWITTER_H

#include "algorithm.h"

#include <QMap>
#include <QSet>
#include <QString>

class GraphScene;
class Node;

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
    bool canAddVertex();
    QWidget* controlWidget(QWidget *parent);

    bool login();
    void getFollowers(QString username, bool numeric = false);

protected slots:
    void replyGot(QNetworkReply *reply);

private:
    GraphScene *graph;
    QOAuth::Interface *oauth;
    Ui::TwitterAuthDialog *authD;
    QNetworkAccessManager *net;
    QByteArray token;
    QByteArray tokenSecret;

    QString lastUserQueried;
    QMap<QString, Node*> nodes;
    QSet<QString> unexpanded;
};

#endif // TWITTER_H
