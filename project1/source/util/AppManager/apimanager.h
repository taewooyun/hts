#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "../../chart/chart_model.h"

class ApiManager : public QObject
{
    Q_OBJECT

public:
    explicit ApiManager(QObject *parent = nullptr);
    void get(const QUrl &url);
    void get(const QNetworkRequest &request);

    // data
    QList<StockItem> stockList;

    StockItem searchItem;
signals:
    void responseReceived(const QByteArray &data);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager m_networkManager;
};

#endif // APIMANAGER_H
