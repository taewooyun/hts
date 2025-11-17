#include "login.h"
#include "ui_login.h"
#include <QSqlQuery>
#include <QRegularExpressionValidator>
#include <QKeyEvent>


login::login(QWidget *parent) : QWidget(parent), ui(new Ui::login) {
    ui->setupUi(this);

    signupPage = new signup(nullptr);
    ui->pw_login->setEchoMode(QLineEdit::Password);


    connect(ui->pw_login, &QLineEdit::returnPressed, this, &login::tryLogin);
    connect(ui->id_login, &QLineEdit::returnPressed, this, &login::tryLogin);
    connect(signupPage, &signup::gotoLogin, signupPage, &QWidget::hide);
}


login::~login() {
    delete ui;
}


void login::on_login_btn_clicked() {
    tryLogin();
}


void login::tryLogin() {
    QString id = ui->id_login->text();
    QString pw = ui->pw_login->text();

    if (id.isEmpty() || pw.isEmpty()) {
        QMessageBox::warning(this, "오류", "ID와 비밀번호를 입력하세요.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT name, balance FROM users WHERE id=? AND pw=?");
    query.addBindValue(id);
    query.addBindValue(pw);
    query.exec();

    if (query.next()) {
        QString name = query.value(0).toString();  // ← DB에서 사용자 이름 가져오기
        int balance = query.value("balance").toInt();

        QMessageBox::information(this, "로그인 성공", "환영합니다!");

        emit loginSuccess(name, balance, id);   // ← 이름을 함께 보냄
        this->hide();
    }
    else {
        QMessageBox::warning(this, "로그인 실패", "ID 또는 비밀번호가 틀렸습니다.");
    }
}



void login::on_pwShow_btn_clicked() {
    bool isPassword = (ui->pw_login->echoMode() == QLineEdit::Password);
    ui->pw_login->setEchoMode(isPassword ? QLineEdit::Normal : QLineEdit::Password);
}


void login::on_signup_btn_clicked() {
    signupPage->show();
    signupPage->raise();
    signupPage->activateWindow();
}
