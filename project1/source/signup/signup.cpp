#include "signup.h"
#include "ui_signup.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
signup::signup(QWidget *parent) : QDialog(parent), ui(new Ui::signup) {
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);  // 전체 앱 모달
    setWindowFlag(Qt::WindowStaysOnTopHint);  // 위로 오기
    setWindowFlag(Qt::Dialog);                // 대화상자 스타일


    ui->pw_signup->setEchoMode(QLineEdit::Password);


    QRegularExpression re("^[0-9]*$");
    ui->accnum_signup->setValidator(new QRegularExpressionValidator(re, this));
}


signup::~signup() {
    delete ui;
}


void signup::on_signup_btn_clicked() {
    QString name = ui->name_signup->text();
    QString acc = ui->accnum_signup->text();
    QString id = ui->id_signup->text();
    QString pw = ui->pw_signup->text();

    if (name.isEmpty() || acc.isEmpty() || id.isEmpty() || pw.isEmpty()) {
        QMessageBox::warning(this, "오류", "모든 정보를 입력하세요.");
        return;
    }

    // 🔹 1. ID 중복검사
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM User WHERE id=?");
    checkQuery.addBindValue(id);
    checkQuery.exec();

    if (checkQuery.next()) {
        QMessageBox::warning(this, "중복", "이미 존재하는 ID입니다.");
        return;
    }

    // 🔹 2. ACC(계좌번호) 중복검사
    QSqlQuery accQuery;
    accQuery.prepare("SELECT acc FROM users WHERE acc=?");
    accQuery.addBindValue(acc);
    accQuery.exec();

    if (accQuery.next()) {
        QMessageBox::warning(this, "중복", "이미 존재하는 계좌번호입니다.");
        return;
    }

    // 🔹 3. DB 삽입
    QSqlQuery query;
    query.prepare("INSERT INTO User (id, pw, name, acc) VALUES (?,?,?,?)");
    query.addBindValue(id);
    query.addBindValue(pw);
    query.addBindValue(name);
    query.addBindValue(acc);

    if (query.exec()) {
        QMessageBox::information(this, "완료", "회원가입이 완료되었습니다!");
        emit gotoLogin();
    } else {
        QMessageBox::warning(this, "실패", "DB 저장 오류가 발생했습니다.");
    }
}



void signup::on_signupPwShow_btn_clicked() {
    bool isPassword = (ui->pw_signup->echoMode() == QLineEdit::Password);
    ui->pw_signup->setEchoMode(isPassword ? QLineEdit::Normal : QLineEdit::Password);
}
