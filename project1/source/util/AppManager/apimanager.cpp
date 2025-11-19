#include "apimanager.h"
#include <QDebug>

ApiManager::ApiManager(QObject *parent) : QObject(parent) {
    connect(&m_networkManager, &QNetworkAccessManager::finished,
            this, &ApiManager::onReplyFinished);
}

void ApiManager::get(const QUrl &url) {
    m_networkManager.get(QNetworkRequest(url));
}
void ApiManager::get(const QNetworkRequest &request)
{
    m_networkManager.get(request);
}

void ApiManager::onReplyFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        emit responseReceived(reply->readAll());
    } else {
        qWarning() << "HTTP error:" << reply->errorString();
    }
    reply->deleteLater();
}
