#ifndef DETAILDIALOG_H
#define DETAILDIALOG_H

#include "chart_model.h"

#include <QDialog>

namespace Ui {
class DetailDialog;
}

class DetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DetailDialog(QList<StockItem>, QWidget *parent = nullptr);
    ~DetailDialog();

private:
    Ui::DetailDialog *ui;
};

#endif
