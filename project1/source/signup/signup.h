#ifndef SIGNUP_H
#define SIGNUP_H


#include <QWidget>
#include <QSqlQuery>
#include <QMessageBox>


namespace Ui {
class signup;
}


class signup : public QDialog {
    Q_OBJECT
public:
    explicit signup(QWidget *parent = nullptr);
    ~signup();


signals:
    void gotoLogin();


private slots:
    void on_signup_btn_clicked();
    void on_signupPwShow_btn_clicked();


private:
    Ui::signup *ui;
};


#endif // SIGNUP_H
