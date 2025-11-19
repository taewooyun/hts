#ifndef SENDDIALOG_H
#define SENDDIALOG_H

#include <QDialog>
#include "bankingdb.h"

namespace Ui {
class SendDialog;
}

class SendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SendDialog(bankingdb *db, int userId, QWidget *parent = nullptr);
    ~SendDialog();

private slots:
    void onSendClicked();

private:
    void loadAccounts();
    void updateBalanceDisplay(int index);

private:
    Ui::SendDialog *ui;
    bankingdb *bankDB;
    int m_userId;
    QVector<AccountItem> m_accList;
};

#endif // SENDDIALOG_H
