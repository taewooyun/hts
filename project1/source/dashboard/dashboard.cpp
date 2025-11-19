#include "dashboard.h"
#include "ui_dashboard.h"
#include <QTableWidget>
#include <qheaderview.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include "../util/AppManager/appmanager.h"

DashBoard::DashBoard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashBoard)
{
    ui->setupUi(this);



    // QSqlQuery qBank(AppManager::instance().db()->database());
    // qBank.prepare("SELECT type, target, amount, memo FROM Banking WHERE userId = ?");
    // qBank.addBindValue(AppManager::instance().id);
    // qBank.exec();

    // QTableWidget *table = new QTableWidget(0, 4, this);
    // table->setHorizontalHeaderLabels({"종류", "계좌", "금액", "메모"});
    // ui->layout_banking->addWidget(table);

    // for (int i = 0; i < table->columnCount(); i++)
    //     table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    // table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // table->horizontalHeader()->setSectionsMovable(false);
    // table->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // if(query.next()){


    //     for(int i=0; i<list.count(); ++i){
    //         table->setItem(i, 0, new QTableWidgetItem(list[i].basDt));
    //         table->setItem(i, 1, new QTableWidgetItem(list[i].clpr));
    //         table->setItem(i, 2, new QTableWidgetItem(list[i].vs));
    //         table->setItem(i, 3, new QTableWidgetItem(list[i].hipr));
    //     }
    // }


    // query.value(0);

}

void DashBoard::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    static bool initialized = false;
    qDebug() << "call";

    if (!initialized) {
        initialized = true;

        QSqlQuery qUser(AppManager::instance().db()->database());
        qUser.prepare(
            "SELECT id, name, acc, balance "
            "FROM User "
            "WHERE id = ?"
            );
        qUser.addBindValue(AppManager::instance().id);

        if (!qUser.exec()) {
            qDebug() << "Query Error:" << qUser.lastError().text();
            return;
        }

        if (!qUser.next()) {
            qDebug() << "No user found for id:" << AppManager::instance().id;
            return;
        }

        qDebug() << "Current User ID =" << AppManager::instance().id;
        qDebug() << "exec() =" << qUser.exec();
        qDebug() << "next() =" << qUser.next();

        QString id = qUser.value("id").toString();
        ui->lbl_name->setText(qUser.value("name").toString());
        ui->lbl_balance->setText(qUser.value("balance").toString());

        qDebug() << qUser.value(0).toString();
    }
}

DashBoard::~DashBoard()
{
    delete ui;
}
