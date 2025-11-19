#include "chart_model.h"
#include "chart.h"
#include "ui_chart.h"
#include "detaildialog.h"
#include "../util/AppManager/databasemanager.h"
#include "../util/AppManager/appmanager.h"
#include <QChart>
#include <QChartView>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <qjsonobject.h>
#include <qtablewidget.h>
#include <qvalueaxis.h>
#include <QDateTime>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSqlQuery>
#include <QSqlError>


Chart::Chart(QWidget *parent, QString stockName)
    : QWidget(parent)
    , ui(new Ui::Chart)
    , stockName(stockName)
{
    ui->setupUi(this);
    setLayout(ui->layout_0);

    // 현재 로그인한 사용자 ID 가져오기
    this->userId = AppManager::instance().db()->currentUserId();

    //로그인 여부에 따라 북마크 버튼 상태 설정
    if (this->userId.isEmpty()) {
        ui->button_bookmark->setEnabled(false);
        ui->button_bookmark->setToolTip("로그인 후 이용 가능합니다");
    } else {
        ui->button_bookmark->setEnabled(true);
    }


    disconnect(ui->button_bookmark, nullptr, nullptr, nullptr);
    // 북마크 버튼 클릭 연결
    connect(ui->button_bookmark, &QPushButton::clicked,
            this, &Chart::on_button_bookmark_clicked,
            Qt::UniqueConnection);

    QString url =
        "https://apis.data.go.kr/1160100/service/GetStockSecuritiesInfoService/getStockPriceInfo?serviceKey=7512965b9438146d549e4f26c6966672ca138df8f6b9cbe8c32047e46c8f5a24&numOfRows=100&resultType=json&itmsNm="
                  + stockName;

    connect(AppManager::instance().api(), &ApiManager::responseReceived,
            this, [this](const QByteArray &response) {

                QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                if (jsonDoc.isObject()) {

                    QJsonObject responseObj = jsonDoc.object().value("response").toObject();
                    ApiResponse api = ApiResponse::fromJson(responseObj);

                    currentItems = api.body.items;    // ⭐ 현재 종목 데이터 저장

                    this->stockName = api.body.items[0].itmsNm;

                    ui->label_title->setText(api.body.items[0].itmsNm);
                    ui->label_price->setText(api.body.items[0].clpr);
                    ui->label_increment->setText(api.body.items[0].vs);
                    ui->label_percentage->setText(api.body.items[0].fltRt);

                    ui->layout_0->addWidget(createPriceChart(api.body.numOfRows, api.body.items));

                    connect(ui->button_detail, &QPushButton::clicked,
                            this, [=]() { on_button_detail_clicked(api.body.items); });

                    disconnect(AppManager::instance().api(),
                               &ApiManager::responseReceived,
                               this, nullptr);
                }
            });


    AppManager::instance().api()->get(QUrl(url));

    //SQLite DB 파일에서 실제 테이블 스키마 확인
    QSqlQuery q(AppManager::instance().db()->database());
    q.exec("PRAGMA table_info(User_Stock)");

    qDebug() << "\n====== User_Stock Columns ======";
    while (q.next()) {
        qDebug() << "cid:" << q.value(0).toInt()
        << "name:" << q.value(1).toString()
        << "type:" << q.value(2).toString();
    }
    qDebug() << "================================";

}

QChartView* Chart::createPriceChart(int cnt, QList<StockItem> items)
{
    QLineSeries* series = new QLineSeries();

    for (const auto& item : items) {
        QDateTime x = QDateTime::fromString(item.basDt, "yyyyMMdd");
        series->append(x.toMSecsSinceEpoch(), item.clpr.toInt());
    }

    QChart *priceChart = new QChart();
    priceChart->addSeries(series);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("yyyy-MM-dd");

    if (!items.isEmpty()) {
        axisX->setRange(
            QDateTime::fromString(items.first().basDt, "yyyyMMdd"),
            QDateTime::fromString(items.last().basDt, "yyyyMMdd")
            );
    }

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%d");

    priceChart->addAxis(axisX, Qt::AlignBottom);
    priceChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    priceChart->legend()->hide();

    QChartView *chartView = new QChartView(priceChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

void Chart::on_button_detail_clicked(const QList<StockItem>& list)
{
    DetailDialog dlg(list);
    dlg.exec();
}

// ⭐ 북마크 여부 체크
bool Chart::isBookmarked(const QString &stockName, const QString userId)
{
    if (userId.isEmpty()) {
        qDebug() << "[Bookmark] Skip — user not logged in";
        return false;
    }

    QSqlQuery q(AppManager::instance().db()->database());

    q.prepare(
        "SELECT 1 FROM User_Stock "
        "WHERE userId = ? AND name = ?"
        );
    q.addBindValue(userId);
    q.addBindValue(stockName);

    if (!q.exec()) {
        qDebug() << "[Bookmark] Query Exec Error:" << q.lastError().text();
        return false;
    }

    if (!q.next()) {
        qDebug() << "[Bookmark] Not found userId:" << userId
                 << ", stockName:" << stockName;
        return false;
    }

    qDebug() << "[Bookmark] Exists! userId:" << userId
             << ", stockName:" << stockName;
    return true;
}




// ⭐ 북마크 추가
bool Chart::addBookmark(const QString userId, const StockItem& stock)
{
    QSqlQuery query(AppManager::instance().db()->database());

    query.prepare(
        "INSERT INTO User_Stock (userId, name, market, price, marketCap) "
        "VALUES (?, ?, ?, ?, ?)"
        );

    query.addBindValue(userId);
    query.addBindValue(stock.itmsNm);
    query.addBindValue(stock.mrktCtg);
    query.addBindValue(stock.clpr.toInt());
    query.addBindValue(stock.mrktTotAmt.toLongLong());

    if (!query.exec()) {
        qDebug() << "[Bookmark] Insert ERROR:" << query.lastError().text();
        return false;
    }

    qDebug() << "[Bookmark] Insert SUCCESS:" << stock.itmsNm
             << "userId:" << userId;

    printUserStockTable();

    return true;
}




// ⭐ 북마크 삭제
bool Chart::removeBookmark(const QString userId, const QString& stockName)
{
    QSqlQuery query(AppManager::instance().db()->database());

    query.prepare("DELETE FROM User_Stock WHERE userId=? AND name=?");
    query.addBindValue(userId);
    query.addBindValue(stockName);

    if (!query.exec()) {
        qDebug() << "[Bookmark] Delete Error:" << query.lastError().text();
        return false;
    }

    qDebug() << "[Bookmark] Removed:" << stockName;

    printUserStockTable();
    return true;
}



// ⭐ 버튼 UI 업데이트
void Chart::updateBookmarkUI(bool bookmarked)
{
    if (bookmarked) {
        ui->button_bookmark->setText("★");
        ui->button_bookmark->setStyleSheet("color: gold; font-size: 22px;");
    } else {
        ui->button_bookmark->setText("☆");
        ui->button_bookmark->setStyleSheet("color: gray; font-size: 22px;");
    }
}

// ⭐ 클릭하면 토글
void Chart::on_button_bookmark_clicked()
{
    bool bookmarked = isBookmarked(this->stockName, this->userId);

    if (bookmarked) {
        removeBookmark(userId, stockName);
    } else {
        addBookmark(userId, currentItems[0]);
    }

    updateBookmarkUI(!bookmarked);
}

Chart::~Chart()
{
    delete ui;
}

void Chart::refreshUserId()
{
    this->userId = AppManager::instance().id;
    qDebug() << "[Chart] refreshUserId:" << this->userId;

    if (userId.isEmpty()) {
        ui->button_bookmark->setVisible(false);
        ui->button_bookmark->setEnabled(false);
        return;
    }

    ui->button_bookmark->setVisible(true);
    ui->button_bookmark->setEnabled(true);  // ⭐⭐ 추가해야 함

    bool bookmarked = isBookmarked(stockName, userId);
    updateBookmarkUI(bookmarked);
}

void Chart::printUserStockTable()
{
    QSqlQuery q(AppManager::instance().db()->database());

    q.exec("SELECT userId, name FROM User_Stock");

    qDebug() << "----- User_Stock TABLE -----";

    while (q.next()) {
        qDebug() << "userId:" << q.value(0).toString()
        << ", stockName:" << q.value(1).toString();
    }

    qDebug() << "----------------------------";
}

