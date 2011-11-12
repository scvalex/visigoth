#include "graphscene.h"
#include "twitter.h"

#include <QDesktopServices>
#include <QSettings>
#include <QUrl>
#include <QWidget>
#include <QtOAuth>

Twitter::Twitter(GraphScene *scene) :
    Algorithm(scene),
    oauth(new QOAuth::Interface(this))
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

void Twitter::login() {
    QSettings settings;
    token = settings.value("token", "").toByteArray();
    tokenSecret = settings.value("tokenSecret", "").toByteArray();
    if (token.size() == 0) {
        QOAuth::ParamMap map;
        map.insert("oauth_callback_url", "oob");
        QOAuth::ParamMap reply = oauth->requestToken("https://api.twitter.com/oauth/request_token",
                                                     QOAuth::GET, QOAuth::HMAC_SHA1);
        if (oauth->error() == QOAuth::NoError) {
            token = reply.value(QByteArray("oauth_token"));
            tokenSecret = reply.value(QByteArray("oauth_token_secret"));
            qDebug("Got unauthrized request token");
        } else {
            qDebug("Could not get request token");
        }
        settings.setValue("token", token);
        settings.setValue("tokenSecret", tokenSecret);
    } else {
        qDebug("Using saved request token");
    }

    QUrl twitterAuthorizationURL("https://api.twitter.com/oauth/authorize");
    twitterAuthorizationURL.addQueryItem("oauth_token", token);
    QDesktopServices::openUrl(twitterAuthorizationURL);
}
