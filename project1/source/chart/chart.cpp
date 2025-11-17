#include "chart_model.h"
#include "chart.h"
#include "ui_chart.h"
#include <QChart>
#include <QChartView>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <qjsonobject.h>
#include <qvalueaxis.h>
#include <QDateTime>
#include <qnetworkaccessmanager.h>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QBarSet>

Chart::Chart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chart)
{
    ui->setupUi(this);
    setLayout(ui->layout_0);

    ui->layout_0->setContentsMargins(50, 20, 50, 20);


    QNetworkAccessManager* manager = new QNetworkAccessManager();

    QString url = "https://apis.data.go.kr/1160100/service/GetStockSecuritiesInfoService/getStockPriceInfo?serviceKey=7512965b9438146d549e4f26c6966672ca138df8f6b9cbe8c32047e46c8f5a24&numOfRows=100&resultType=json&itmsNm=%ED%95%9C%ED%99%94%EB%B9%84%EC%A0%84";
    QNetworkRequest request;
    request.setUrl(QUrl(url));

    QObject::connect(manager, &QNetworkAccessManager::finished,
                     [=](QNetworkReply* reply) {
                         if (reply->error()) {
                             qDebug() << "Error:" << reply->errorString();
                         } else {
                             QByteArray response = reply->readAll();

                             // JSON 파싱
                             QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                             if (jsonDoc.isObject()) {
                                 QJsonObject obj = jsonDoc.object();
                                 QJsonObject responseObj = obj["response"].toObject();

                                 ApiResponse api = ApiResponse::fromJson(responseObj);
                                 // qDebug() << api.body.numOfRows;

                                 ui->layout_0->addWidget(createPriceChart(api.body.numOfRows, api.body.items));
                             }
                         }
                         reply->deleteLater();
                     }
                     );

    manager->get(request);


}

QChartView* Chart::createPriceChart(int cnt, QList<StockItem> items){
    QLineSeries* series = new QLineSeries();
    for (const auto& item : items) {
        QDateTime x = QDateTime::fromString(item.basDt, "yyyyMMdd");
        int y = item.clpr.toInt();
        series->append(x.toMSecsSinceEpoch(), y);
    }

    QChart *priceChart = new QChart();
    priceChart->addSeries(series);
    // Axis 스타일
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("yyyy-MM-dd");

    if (!items.isEmpty()) {
        QDateTime first = QDateTime::fromString(items.first().basDt, "yyyyMMdd");
        QDateTime last = QDateTime::fromString(items.last().basDt, "yyyyMMdd");
        axisX->setRange(first, last);
    }

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%d");

    priceChart->addAxis(axisX, Qt::AlignBottom);
    priceChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    priceChart->setBackgroundVisible(false);
    priceChart->setPlotAreaBackgroundVisible(false);

    QChartView *chartView = new QChartView(priceChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    priceChart->legend()->hide();
    // priceChart->setBackgroundBrush(Qt::white);

    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    return chartView;
}

Chart::~Chart()
{
    delete ui;
}

