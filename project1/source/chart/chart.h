#ifndef CHART_H
#define CHART_H

#include "chart_model.h"
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

public slots:
private slots:
    void on_button_trade_clicked(const QList<StockItem> &list);


private:
    Ui::Chart *ui;

    QChartView* createPriceChart(int cnt, QList<StockItem> items);
};



#endif // CHART_H
