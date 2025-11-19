#ifndef LOGIN_H
#define LOGIN_H


#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include "../signup/signup.h"

namespace Ui {
class login;
}


class login : public QWidget {
    Q_OBJECT
public:
    explicit login(QWidget *parent = nullptr);
    signup *signupPage;
    ~login();


signals:
    void gotoSignup();
    void loginSuccess(const QString &name, int balance, QString id);
    void openSignup();

private slots:
    void on_login_btn_clicked();
    void on_pwShow_btn_clicked();
    void on_signup_btn_clicked();
    void tryLogin();


private:
    Ui::login *ui;
};


#endif // LOGIN_H
