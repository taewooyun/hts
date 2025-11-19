#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QTimer>
#include <QChartView>
#include "../login/login.h"
#include "../signup/signup.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Home; }
QT_END_NAMESPACE



class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(login *loginPage, signup *signupPage, QWidget *parent = nullptr);

    ~Home();
signals:
    void logined();
    void logout();
    void search(QString);

private slots:
    void on_btnLogin_clicked();   // 로그인 버튼 슬롯 추가
    void onLoginSuccess(const QString &name, int balance, QString id);

    void on_searchButton_clicked();

private:
    Ui::Home *ui;
    login *loginPage;
    QString currentUserId;
    QTimer *balanceTimer;
};

#endif // HOME_H
