#include "detaildialog.h"
#include "chart_model.h"
#include "ui_detaildialog.h"
#include <qtablewidget.h>
#include <QHeaderView>

DetailDialog::DetailDialog(QList<StockItem> list, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DetailDialog)
{
    ui->setupUi(this);
    resize(800, 600);

    QTableWidget *table = new QTableWidget(list.count(), 5, this);
    table->setHorizontalHeaderLabels({"날짜", "종가", "대비", "고가", "저가", "거래량"});

    for(int i=0; i<list.count(); ++i){
        table->setItem(i, 0, new QTableWidgetItem(list[i].basDt));
        table->setItem(i, 1, new QTableWidgetItem(list[i].clpr));
        table->setItem(i, 2, new QTableWidgetItem(list[i].vs));
        table->setItem(i, 3, new QTableWidgetItem(list[i].hipr));
        table->setItem(i, 4, new QTableWidgetItem(list[i].lopr));
        table->setItem(i, 5, new QTableWidgetItem(list[i].trqu));
    }

    for (int i = 0; i < table->columnCount(); i++)
        table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionsMovable(false);
    table->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    ui->verticalLayout->addWidget(table);
}

DetailDialog::~DetailDialog()
{
    delete ui;
}
