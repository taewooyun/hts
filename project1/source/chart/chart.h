#ifndef CHART_H
#define CHART_H

#include "chart_model.h"
#include <QTableWidget>
#include <QWidget>
#include <qchartview.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Chart;
}
QT_END_NAMESPACE

class Chart : public QWidget
{
    Q_OBJECT

public:
    Chart(QWidget *parent = nullptr, QString stockName = "한화비전");
    void refreshUserId();
    void printUserStockTable();
    ~Chart();
    QString keyword;

    void requestStock(QString keyword);
public slots:
    void setKeyword(const QString& keyword);
private slots:
    void on_button_detail_clicked(const QList<StockItem> &list);
    void on_button_bookmark_clicked();


private:
    Ui::Chart *ui;

    // ⭐ 북마크 관련 멤버
    QString stockName;
    QString userId;
    QList<StockItem> currentItems;

    // ⭐ 북마크 DB 함수들
    bool isBookmarked(const QString& stockName, const QString userId);
    bool addBookmark(const QString userId, const StockItem& stock);
    bool removeBookmark(const QString userId, const QString& stockName);
    void updateBookmarkUI(bool bookmarked);

    // 차트 생성
    QChartView *createPriceChart(int cnt, QList<StockItem> items);
};

#endif // CHART_H
