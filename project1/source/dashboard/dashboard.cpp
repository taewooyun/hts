// dashboard.cpp
#include "dashboard.h"
#include "ui_dashboard.h"
#include "senddialog.h"
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

    // 메인 DB 전달
    auto mainDB = AppManager::instance().db()->database();
    bankDB = new bankingdb(mainDB, this);

    // 🔥 로그인한 사용자 ID 저장
    m_userId = AppManager::instance().id.toInt();

    ui->list_banking->setColumnCount(5);
    ui->list_banking->setHorizontalHeaderLabels(
        {"User ID", "Type", "Target", "Amount", "Memo"});

    connect(ui->btn_send, &QPushButton::clicked, this, &DashBoard::onSend);

}

DashBoard::~DashBoard()
{
    delete ui;
}

/*--------------------------------------------------------
    showEvent: 탭이 보일 때마다 사용자/내역 갱신
--------------------------------------------------------*/
void DashBoard::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    qDebug() << "[DashBoard] showEvent called";

    // 사용자 정보 갱신
    auto dbPtr = AppManager::instance().db();
    if (!dbPtr) {
        qDebug() << "[DashBoard] DB pointer is NULL";
        return;
    }
    if (!dbPtr->database().isOpen()) {
        qDebug() << "[DashBoard] DB is not open";
        return;
    }

    // 사용자 정보 조회 (id가 비어있을 수도 있음)
    QString currentId = AppManager::instance().id;
    if (currentId.isEmpty()) {
        qDebug() << "[DashBoard] No current user id";
        ui->lbl_name->setText("비회원");
        ui->lbl_balance->setText("-");
    } else {
        QSqlQuery qUser(dbPtr->database());
        qUser.prepare("SELECT id, name, acc, balance FROM User WHERE id = ?");
        qUser.addBindValue(currentId);

        if (!qUser.exec()) {
            qDebug() << "[DashBoard] qUser.exec() Error:" << qUser.lastError().text();
            ui->lbl_name->setText("오류");
            ui->lbl_balance->setText("-");
        } else if (qUser.next()) {
            ui->lbl_name->setText(qUser.value("name").toString());
            ui->lbl_balance->setText(qUser.value("balance").toString());
            qDebug() << "[DashBoard] Loaded user:" << qUser.value("name").toString()
                     << "balance:" << qUser.value("balance").toString();
        } else {
            ui->lbl_name->setText("알수없음");
            ui->lbl_balance->setText("-");
            qDebug() << "[DashBoard] user not found for id" << currentId;
        }
    }

    // 거래 내역(리스트) 갱신
    loadBankingHistory();
}

/*--------------------------------------------------------
    loadBankingHistory:
    - UI에 QListWidget (ui->list_banking)이 있으면 사용.
    - 없으면 fallback으로 QTableWidget을 만들거나 재사용.
--------------------------------------------------------*/
void DashBoard::loadBankingHistory()
{
    qDebug() << "[DashBoard] loadBankingHistory() called";

    // 안전 체크: DB 포인터/연결 상태
    auto dbPtr = AppManager::instance().db();
    if (!dbPtr) {
        qDebug() << "[DashBoard] DB pointer NULL";
        return;
    }
    if (!dbPtr->database().isOpen()) {
        qDebug() << "[DashBoard] DB not open";
        return;
    }

    /* -------------- 여기에 debug 코드 넣기 ---------------- */
    QSqlQuery debugQ(dbPtr->database());
    debugQ.exec("SELECT COUNT(*) FROM banking");
    if (debugQ.next()) {
        qDebug() << "[Debug] banking total rows =" << debugQ.value(0).toInt();
    }
    /* ----------------------------------------------------- */


    QString currentId = AppManager::instance().id;
    if (currentId.isEmpty()) {
        qDebug() << "[DashBoard] No login user";
        ui->list_banking->setRowCount(0);
        return;
    }

    // 테이블 초기화
    ui->list_banking->setRowCount(0);

    // DB 조회
    QSqlQuery q(dbPtr->database());
    q.prepare("SELECT userid, type, target, amount, memo FROM banking WHERE userid = ? ORDER BY ROWID DESC");
    q.addBindValue(currentId);

    if (!q.exec()) {
        qDebug() << "[DashBoard] Query error :" << q.lastError().text();
        return;
    }

    int row = 0;
    while (q.next()) {
        ui->list_banking->insertRow(row);

        ui->list_banking->setItem(row, 0, new QTableWidgetItem(q.value("userid").toString()));
        ui->list_banking->setItem(row, 1, new QTableWidgetItem(q.value("type").toString()));
        ui->list_banking->setItem(row, 2, new QTableWidgetItem(q.value("target").toString()));
        ui->list_banking->setItem(row, 3, new QTableWidgetItem(q.value("amount").toString()));
        ui->list_banking->setItem(row, 4, new QTableWidgetItem(q.value("memo").toString()));

        row++;
    }

    qDebug() << "[DashBoard] list_banking filled, rows =" << row;
}

void DashBoard::onSend()
{
    SendDialog dlg(bankDB, m_userId, this);
    dlg.exec();
}


