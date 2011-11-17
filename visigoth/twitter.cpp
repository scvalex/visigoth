#include "graphscene.h"
#include "twitter.h"
#include "ui_twitauthdialog.h"
#include "ui_twittercontrol.h"

#include <QtCore/qmath.h>
#include <QDebug>
#include <QDesktopServices>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>
#include <QUrl>
#include <QWidget>
#include <QtOAuth>

Twitter::Twitter(GraphScene *scene) :
    Algorithm(scene),
    graph(scene),
    oauth(new QOAuth::Interface(this)),
    authD(0),
    net(new QNetworkAccessManager(this)),
    ctlW(0),
    rootUser("scvalex")
{
    oauth->setConsumerKey("zaeBj6kxsz2P0N7O1LwUWg");
    oauth->setConsumerSecret("8yf2EkBuH9vNr1D3XlM1RKZ9GkoXKAfpNLLBfwzwg");
    oauth->setRequestTimeout(10000);
    connect(net, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyGot(QNetworkReply*)));
    reset();
}

Twitter::~Twitter() {
}

void Twitter::reset() {
    lastUserQueried = "";
    nodes.clear();
    unexpanded.clear();
    if (!login()) {
        qDebug("Login failed");
    }
    getFollowers(rootUser);
}

void Twitter::addVertex() {
    if (unexpanded.empty())
        return;
    getFollowers(unexpanded.dequeue(), true);
}

bool Twitter::canAddVertex() {
    return true;
}

QWidget* Twitter::controlWidget(QWidget *parent) {
    if (!ctlW) {
        ctlW = new QWidget(parent);
        Ui::TwitterControl *twitCtl = new Ui::TwitterControl();
        twitCtl->setupUi(ctlW);
        twitCtl->userEdit->setText(rootUser);
        connect(twitCtl->clearButton, SIGNAL(clicked()), this, SLOT(clearPrivateData()));
        connect(twitCtl->userEdit, SIGNAL(textChanged(const QString&)), this, SLOT(setRootUser(const QString&)));
    }
    return ctlW;
}

bool Twitter::login() {
    QSettings settings;
    token = settings.value("token").toByteArray();
    tokenSecret = settings.value("tokenSecret").toByteArray();
    if (token.size() > 0)
        return true;
    QOAuth::ParamMap reply = oauth->requestToken("https://api.twitter.com/oauth/request_token",
                                                 QOAuth::GET, QOAuth::HMAC_SHA1);
    if (oauth->error() == QOAuth::NoError) {
        token = reply.value(QOAuth::tokenParameterName());
        tokenSecret = reply.value(QOAuth::tokenSecretParameterName());
        qDebug("Got unauthrized request token");
    } else {
        qDebug("Could not get request token");
        return false;
    }

    QUrl twitterAuthorizationURL("https://api.twitter.com/oauth/authorize");
    twitterAuthorizationURL.addQueryItem(QOAuth::tokenParameterName(), token);
    twitterAuthorizationURL.addQueryItem("oauth_callback_url", "oob");
    QDesktopServices::openUrl(twitterAuthorizationURL);

    qDebug("Opening the Dialog of Secrets");
    QDialog d;
    authD = new Ui::TwitterAuthDialog();
    authD->setupUi(&d);
    authD->urlLabel->setText("<a href=\"" + twitterAuthorizationURL.toString() + "\">Authorize</a>");
    if (d.exec() == QDialog::Rejected) {
        qDebug("User is a spoil-sport");
        return false;
    }

    QString secret = authD->pinEdit->text();
    qDebug() << "Secret is" << secret;

    QOAuth::ParamMap map;
    map.insert("oauth_verifier", secret.toAscii());
    reply = oauth->accessToken("https://api.twitter.com/oauth/access_token", QOAuth::POST, token,
                               tokenSecret, QOAuth::HMAC_SHA1, map);

    if (oauth->error() == QOAuth::NoError) {
        token = reply.value(QOAuth::tokenParameterName());
        tokenSecret = reply.value(QOAuth::tokenSecretParameterName());
        qDebug("Access token received");
    } else {
        qDebug("(access token)Got error: %d", oauth->error());
        return false;
    }

    settings.setValue("token", token);
    settings.setValue("tokenSecret", tokenSecret);
    return true;
}

void Twitter::getFollowers(QString username, bool numeric) {
    lastUserQueried = username;

    QString requestUrl("http://api.twitter.com/1/followers/ids.xml");

    QOAuth::ParamMap map;
    QByteArray authH = oauth->createParametersString(requestUrl, QOAuth::GET, token, tokenSecret, QOAuth::HMAC_SHA1, map,
                                                     QOAuth::ParseForHeaderArguments);

    QUrl url(requestUrl);
    url.addQueryItem(numeric ? "user_id" : "screen_name", username.toAscii());

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", authH);
    net->get(request);
}

void Twitter::replyGot(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug("Network error: %d.  Huh.", reply->error());
        qDebug() << reply->readAll();
        return;
    }

    if (lastUserQueried.size() == 0) {
        // whoosh
        return;
    }
    if (!nodes.contains(lastUserQueried)) {
        nodes[lastUserQueried] = graph->newNode();
    }

    QDomDocument doc;
    if (!doc.setContent(reply)) {
        qDebug("Error parsing reply");
        return;
    }

    QDomNodeList ns = doc.elementsByTagName("id");
    int count = ns.length();
    if (count > 2)
        count = qSqrt(count);
    for (int i(0); i < count; ++i) {
        QDomNode node = ns.at(i);
        QString userid = node.firstChild().toText().data();
        nodes[userid] = graph->newNode();
        unexpanded.enqueue(userid);
        graph->newEdge(nodes[lastUserQueried], nodes[userid]);
    }
}

void Twitter::clearPrivateData() {
    QSettings settings;
    settings.setValue("token", "");
    settings.setValue("tokenSecret", "");
    settings.sync();
    graph->reset();
}

void Twitter::setRootUser(const QString &newUser) {
    rootUser = newUser;
}
