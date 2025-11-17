#include "home.h"
#include "ui_home.h"
#include <QMessageBox>
#include "../login/login.h"     // 폴더 구조에 맞게 조정


Home::Home(login *loginPage, signup *signupPage, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Home)
    , loginPage(loginPage)   // main.cpp에서 받은 loginPage 사용
{
    ui->setupUi(this);

    ui->news_1->setOpenExternalLinks(true);
    ui->news_1->setHtml(
        "<div style='text-align:center;'>"
        "<a href='https://n.news.naver.com/mnews/article/057/0001919433' "
        "style='font-size:14px; text-decoration:none; color:#0066cc;'>"
        "여기도 저기도 전기차…중국 차도 가세, 전기차 연간 20만 대 첫 돌파"
        "</a>"
        "</div>"
        );

    ui->news_2->setOpenExternalLinks(true);
    ui->news_2->setHtml(
        "<div style='text-align:center;'>"
        "<a href='https://n.news.naver.com/mnews/article/014/0005435708' "
        "style='font-size:14px; text-decoration:none; color:#0066cc;'>"
        "외국인이 팔면 개인은 샀다… 롤러코스터 탄 반도체株"
        "</a>"
        "</div>"
        );

    balanceTimer = new QTimer(this);
    connect(balanceTimer, &QTimer::timeout, this, &Home::updateBalance);

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

// 🔥 로그인 버튼 눌렀을 때 실행될 부분
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
        // ★ 로그아웃 처리
        ui->btnLogin->setText("로그인");
        QMessageBox::information(this, "로그아웃", "로그아웃되었습니다.");
    }
}

void Home::onLoginSuccess(const QString &name, int balance, QString id)
{
    currentUserId = id;
    ui->btnLogin->setText(QString("%1님\n로그아웃").arg(name));
    ui->btnLogin->setStyleSheet("text-align: center;");
    ui->btnLogin->setFixedSize(110, 50);

    ui->balance_label->setText(QString("보유 잔액: %1원").arg(balance));
    ui->balance_label->setAlignment(Qt::AlignCenter);
    ui->balance_label->setMinimumWidth(260);
    ui->balance_label->setFixedHeight(50);
    ui->balance_label->setStyleSheet(
        "background:#fefefe;"
        "border:2px solid #87a8f5;"
        "border-radius:10px;"
        "padding:10px 20px;"
        "font-size:18px;"
        );
    balanceTimer->start(1000);
}

void Home::updateBalance()
{
    if (currentUserId.isEmpty()) return;

    QSqlQuery q;
    q.prepare("SELECT balance FROM users WHERE id=?");
    q.addBindValue(currentUserId);
    q.exec();

    if (q.next()) {
        int balance = q.value("balance").toInt();
        ui->balance_label->setText(QString("💰 보유 잔액: <b>%1 원</b>").arg(balance));

    }
}
