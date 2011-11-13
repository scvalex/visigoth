#include "graphscene.h"
#include "twitter.h"
#include "ui_twitauthdialog.h"

#include <QDebug>
#include <QDesktopServices>
#include <QSettings>
#include <QUrl>
#include <QWidget>
#include <QtOAuth>

Twitter::Twitter(GraphScene *scene) :
    Algorithm(scene),
    oauth(new QOAuth::Interface(this)),
    authD(0)
{
    oauth->setConsumerKey("zaeBj6kxsz2P0N7O1LwUWg");
    oauth->setConsumerSecret("8yf2EkBuH9vNr1D3XlM1RKZ9GkoXKAfpNLLBfwzwg");
    oauth->setRequestTimeout(10000);
    login();
}

Twitter::~Twitter() {
}

void Twitter::reset() {
}

void Twitter::addVertex() {
}

QWidget* Twitter::controlWidget(QWidget *parent) {
    return 0;
}

bool Twitter::login() {
    token = QByteArray("");
    tokenSecret = QByteArray("");
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
    return true;
}
