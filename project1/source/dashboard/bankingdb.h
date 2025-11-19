#ifndef BANKINGDB_H
#define BANKINGDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QVector>

struct AccountItem {
    int id;
    QString accountNumber;
    int balance;
};

class bankingdb : public QObject
{
    Q_OBJECT
public:
    explicit bankingdb(QSqlDatabase sharedDB, QObject *parent = nullptr);
    ~bankingdb();

    // 계정 관련
    QVector<AccountItem> getUserAccounts(int userId);
    int getAccountBalance(int userId);
    int getAccountIdByNumber(const QString &accNumber);

    // 은행 기능
    void deposit(int userId, int amount, const QString &memo = "");
    void send(int fromUserId, const QString &toAccount, int amount, const QString &memo = "");
    bool transfer(int fromId, int toId, int amount, const QString &memo);

    void printAll();

private:
    QSqlDatabase db;
};

#endif // BANKINGDB_H
