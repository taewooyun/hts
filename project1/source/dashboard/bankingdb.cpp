#include "bankingdb.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bankingdb::bankingdb(QSqlDatabase sharedDB, QObject *parent)
    : QObject(parent), db(sharedDB)
{
    qDebug() << "[bankingdb] Using shared main DB";
}

bankingdb::~bankingdb()
{
}

QVector<AccountItem> bankingdb::getUserAccounts(int userId)
{
    QVector<AccountItem> result;

    QSqlQuery q(db);
    q.prepare("SELECT id, acc, balance FROM User WHERE id = ?");
    q.addBindValue(userId);

    if (q.exec() && q.next()) {
        AccountItem item;
        item.id = q.value(0).toInt();
        item.accountNumber = q.value(1).toString();
        item.balance = q.value(2).toInt();
        result.push_back(item);
    }
    return result;
}

int bankingdb::getAccountBalance(int userId)
{
    QSqlQuery query;
    query.prepare("SELECT balance FROM User WHERE id=?");
    query.addBindValue(userId);
    query.exec();

    if (query.next()) {
        QVariant v = query.value(0);
        if (v.isNull()) return 0;      // ← NULL 방지
        return v.toInt();
    }
    return 0;
}

int bankingdb::getAccountIdByNumber(const QString &accNumber)
{
    QSqlQuery q(db);
    q.prepare("SELECT id FROM User WHERE acc = ?");
    q.addBindValue(accNumber);

    if (q.exec() && q.next())
        return q.value(0).toInt();

    return -1;
}

void bankingdb::deposit(int userId, int amount, const QString &memo)
{
    QSqlQuery q(db);

    // Banking 기록
    q.prepare("INSERT INTO Banking (userId, type, target, amount, memo) "
              "VALUES (?, '입금', '', ?, ?)");
    q.addBindValue(userId);
    q.addBindValue(amount);
    q.addBindValue(memo);
    q.exec();

    // User.balance 증가
    q.prepare("UPDATE User SET balance = balance + ? WHERE id = ?");
    q.addBindValue(amount);
    q.addBindValue(userId);

    if (!q.exec())
        qDebug() << "[banking] Deposit FAIL update balance:" << q.lastError();

    printAll();
}

void bankingdb::send(int fromUserId, const QString &toAccount, int amount, const QString &memo)
{
    int toId = getAccountIdByNumber(toAccount);

    if (fromUserId < 0 || toId < 0) {
        qDebug() << "[banking] Invalid account(s)";
        return;
    }

    transfer(fromUserId, toId, amount, memo);
}

bool bankingdb::transfer(int fromId, int toId, int amount, const QString &memo)
{
    db.transaction();
    QSqlQuery q(db);

    // 0) 출금자 잔액 확인
    q.prepare("SELECT balance FROM User WHERE id = ?");
    q.addBindValue(fromId);

    if (!q.exec() || !q.next()) {
        qDebug() << "[transfer] Sender not found!";
        db.rollback();
        return false;
    }

    int senderBalance = q.value(0).toInt();
    if (senderBalance < amount) {
        qDebug() << "[transfer] Not enough money!";
        db.rollback();
        return false;
    }

    // 1) Banking 기록(송금)
    q.prepare("INSERT INTO Banking (userId, type, target, amount, memo) "
              "VALUES (?, '송금', ?, ?, ?)");
    q.addBindValue(fromId);
    q.addBindValue(QString::number(toId));
    q.addBindValue(amount);
    q.addBindValue(memo);

    if (!q.exec()) {
        qDebug() << "[transfer] Insert send FAIL:" << q.lastError();
        db.rollback();
        return false;
    }

    // 2) 출금 처리
    q.prepare("UPDATE User SET balance = balance - ? WHERE id = ?");
    q.addBindValue(amount);
    q.addBindValue(fromId);

    if (!q.exec()) {
        qDebug() << "[transfer] Update sender balance FAIL:" << q.lastError();
        db.rollback();
        return false;
    }

    // 3) 상대방 Banking 기록(입금)
    q.prepare("INSERT INTO Banking (userId, type, target, amount, memo) "
              "VALUES (?, '입금', ?, ?, ?)");
    q.addBindValue(toId);
    q.addBindValue(QString::number(fromId));
    q.addBindValue(amount);
    q.addBindValue(memo);

    if (!q.exec()) {
        qDebug() << "[transfer] Insert receive FAIL:" << q.lastError();
        db.rollback();
        return false;
    }

    // 4) 수신자 잔액 증가
    q.prepare("UPDATE User SET balance = balance + ? WHERE id = ?");
    q.addBindValue(amount);
    q.addBindValue(toId);

    if (!q.exec()) {
        qDebug() << "[transfer] Update receiver balance FAIL:" << q.lastError();
        db.rollback();
        return false;
    }

    db.commit();
    qDebug() << "[transfer] SUCCESS";

    return true;
}

void bankingdb::printAll()
{
    QSqlQuery q(db);
    q.exec("SELECT userId, type, target, amount, memo FROM Banking");

    qDebug() << "----- BANKING TABLE -----";
    while (q.next()) {
        qDebug()
        << "userId:" << q.value(0).toInt()
        << ", type:" << q.value(1).toString()
        << ", target:" << q.value(2).toString()
        << ", amount:" << q.value(3).toInt()
        << ", memo:" << q.value(4).toString();
    }
    qDebug() << "--------------------------";
}
