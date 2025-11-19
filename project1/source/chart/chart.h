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
    Chart(QWidget *parent = nullptr, QString = "한화비전");
    ~Chart();
    QString keyword;

    void requestStock(QString keyword);
public slots:
    void setKeyword(const QString& keyword);
private slots:
    void on_button_detail_clicked(const QList<StockItem> &list);


private:
    Ui::Chart *ui;

    QChartView *createPriceChart(int cnt, QList<StockItem> items);
};



#endif // CHART_H
