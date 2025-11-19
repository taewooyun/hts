#include "chart_model.h"
#include "chart.h"
#include "ui_chart.h"
#include "detaildialog.h"
#include <QChart>
#include <QChartView>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <qjsonobject.h>
#include <qtablewidget.h>
#include <qvalueaxis.h>
#include <QDateTime>
#include <qnetworkaccessmanager.h>
#include <QJsonDocument>
#include <QNetworkReply>
#include "../util/AppManager/appmanager.h"

Chart::Chart(QWidget *parent, QString stockName)
    : QWidget(parent)
    , ui(new Ui::Chart)
{

    ui->setupUi(this);
    setLayout(ui->layout_0);

    QString url =
        "https://apis.data.go.kr/1160100/service/GetStockSecuritiesInfoService/getStockPriceInfo?serviceKey=7512965b9438146d549e4f26c6966672ca138df8f6b9cbe8c32047e46c8f5a24&numOfRows=100&resultType=json&itmsNm="
        +stockName;

    connect(AppManager::instance().api(), &ApiManager::responseReceived,
            this, [this](const QByteArray &response) {

                QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                if (jsonDoc.isObject()) {
                    QJsonObject responseObj = jsonDoc.object().value("response").toObject();
                    ApiResponse api = ApiResponse::fromJson(responseObj);

                    ui->label_title->setText(api.body.items[0].itmsNm);
                    ui->label_price->setText(api.body.items[0].clpr);
                    QString vs = api.body.items[0].vs;
                    if(0 < vs.toInt()){
                        ui->label_increment->setStyleSheet("color: #E20000;");
                        ui->label_percentage->setStyleSheet("color: #E20000;");
                        ui->label_percentage_symbol->setStyleSheet("color: #E20000;");
                    } else {
                        ui->label_increment->setStyleSheet("color: #1700E2;");
                        ui->label_percentage->setStyleSheet("color: #1700E2;");
                        ui->label_percentage_symbol->setStyleSheet("color: #1700E2;");
                    }

                    ui->layout_0->setStretch(0,1);
                    ui->layout_0->setStretch(1,5);

                    ui->label_increment->setText(api.body.items[0].vs);
                    ui->label_percentage->setText(api.body.items[0].fltRt);
                    ui->layout_0->addWidget(createPriceChart(api.body.numOfRows, api.body.items));

                    connect(ui->button_detail, &QPushButton::clicked, this, [=]() {
                        on_button_detail_clicked(api.body.items);
                    });

                    disconnect(AppManager::instance().api(), &ApiManager::responseReceived, this, nullptr);
                }
            });

    AppManager::instance().api()->get(QUrl(url));

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


void Chart::on_button_detail_clicked(const QList<StockItem>& list)
{
    DetailDialog dlg(list);
    dlg.exec();
}

Chart::~Chart()
{
    delete ui;
}


