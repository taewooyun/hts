#ifndef BUYDIALOG_H
#define BUYDIALOG_H

#include "chart_model.h"

#include <QDialog>

namespace Ui {
class BuyDialog;
}

class BuyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BuyDialog(QList<StockItem>, QWidget *parent = nullptr);
    ~BuyDialog();

private:
    Ui::BuyDialog *ui;
};

#endif // BUYDIALOG_H
