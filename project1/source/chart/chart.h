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
    Chart(QWidget *parent = nullptr);
    ~Chart();

private:
    Ui::Chart *ui;

    QChartView* createPriceChart(int cnt, QList<StockItem> items);
    QChartView *createVolumeChart(int cnt, QList<StockItem> items);
};



#endif // CHART_H
