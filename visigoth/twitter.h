#ifndef TWITTER_H
#define TWITTER_H

#include "algorithm.h"

#include <QList>
#include <QMap>
#include <QQueue>
#include <QString>

class GraphScene;
class Node;

class QNetworkAccessManager;
class QNetworkReply;
class QWidget;

namespace QOAuth {
    class Interface;
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

protected slots:
    void replyGot(QNetworkReply *reply);
    void clearPrivateData();
    void setRootUser(const QString &newUser);

private:
    bool login();
    void getFollowers(QString username, bool numeric = false);

    GraphScene *graph;
    QOAuth::Interface *oauth;
    QNetworkAccessManager *net;
    QByteArray token;
    QByteArray tokenSecret;
    QWidget *ctlW;

    QString lastUserQueried;
    QMap<QString, Node*> nodes;
    QQueue<QString> unexpanded;
    QString rootUser;
};

#endif // TWITTER_H
