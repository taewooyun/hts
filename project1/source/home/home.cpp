#include "home.h"
#include "ui_home.h"
#include <QMessageBox>
#include "../login/login.h"     // 폴더 구조에 맞게 조정
#include "../util/AppManager/appmanager.h"


Home::Home(login *loginPage, signup *signupPage, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Home)
    , loginPage(loginPage)   // main.cpp에서 받은 loginPage 사용
{
    ui->setupUi(this);

    QString title = "여기도 저기도 전기차…중국 차도 가세, 전기차 연간 20만 대 첫 돌파";
    QString discription = "여기도 저기도 전기차…중국 차도 가세, 전기차 연간 20만 대 첫 돌파전기차 연간 20만 대 첫 돌파전기차 연간 20만 대 첫 돌파전기차 연간 20만 대 첫 돌파전기차 연간 20만 대 첫 돌파";
    QString date = "";

    // 삭제 방지
    loginPage->setAttribute(Qt::WA_DeleteOnClose, false);

    connect(ui->btnLogin, &QPushButton::clicked, this, &Home::on_btnLogin_clicked);
    connect(loginPage, &login::loginSuccess, this, &Home::onLoginSuccess);

    // 회원가입 버튼 눌렀을 때 signup 창 띄우게
    connect(loginPage, &login::openSignup, signupPage, &signup::show);
}


Home::~Home()
{
    delete ui;
}

//  로그인 버튼 눌렀을 때 실행될 부분
void Home::on_btnLogin_clicked()
{
    // 로그아웃 상태일 때만 로그인 창 띄우기
    if (ui->btnLogin->text() == "로그인")
    {
        loginPage->setWindowModality(Qt::ApplicationModal);
        loginPage->show();
    }
    else
    {
        // 로그아웃 처리
        ui->btnLogin->setText("로그인");
        emit logout();
        QMessageBox::information(this, "로그아웃", "로그아웃되었습니다.");
    }
}

void Home::onLoginSuccess(const QString &name, int balance, QString id)
{
    currentUserId = id;
    AppManager::instance().db()->setCurrentUserId(id);
    ui->btnLogin->setText(QString("로그아웃"));
    ui->btnLogin->setStyleSheet("text-align: center;");
    ui->btnLogin->setFixedSize(110, 50);

    emit logined();
}


void Home::on_searchButton_clicked()
{
    ui->lineEdit->textEdited("");

    emit search(ui->lineEdit->text());
}

