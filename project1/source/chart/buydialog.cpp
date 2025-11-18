#include "buydialog.h"
#include "chart_model.h"
#include "ui_buydialog.h"
#include <qtablewidget.h>

BuyDialog::BuyDialog(QList<StockItem> list, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BuyDialog)
{
    ui->setupUi(this);
    resize(800, 600);

    QTableWidget *table = new QTableWidget(list.count(), 5, this);
    table->setHorizontalHeaderLabels({"종가", "대비", "고가", "저가", "거래량"});

    for(int i=0; i<list.count(); ++i){
        table->setItem(i, 0, new QTableWidgetItem(list[i].clpr));
        table->setItem(i, 1, new QTableWidgetItem(list[i].vs));
        table->setItem(i, 2, new QTableWidgetItem(list[i].hipr));
        table->setItem(i, 3, new QTableWidgetItem(list[i].lopr));
        table->setItem(i, 4, new QTableWidgetItem(list[i].trqu));
    }

    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableLayout->addWidget(table);
}

BuyDialog::~BuyDialog()
{
    delete ui;
}
