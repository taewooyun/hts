#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "bankingdb.h"
#include <QWidget>

namespace Ui {
class DashBoard;
}

class DashBoard : public QWidget
{
    Q_OBJECT

public:
    explicit DashBoard(QWidget *parent = nullptr);
    ~DashBoard();

public slots:
    void showEvent(QShowEvent *event);

private:
    Ui::DashBoard *ui;
    bankingdb *bankDB;
     int m_userId;
    void loadBankingHistory();

private slots:
    void onSend();
};

#endif // DASHBOARD_H
