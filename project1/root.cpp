#include "root.h"
#include "source/chart/chart.h"
#include "source/dashboard/dashboard.h"
#include "source/home/home.h"
#include "source/login/login.h"
#include "tab.h"
#include "ui_root.h"
#include <QTabWidget>

Root::Root(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
    , ui(new Ui::Root)
{
    ui->setupUi(this);
    resize(1300, 700);

    TabWidget *tabBar = new TabWidget;
    login *loginPage = new login;
    signup *signupPage = new signup;
    Home *homePage = new Home(loginPage, signupPage);
    Chart *chartPage = new Chart();

    tabBar->addTab(homePage, "홈");
    tabBar->addTab(chartPage, "차트");
    tabBar->addTab(new DashBoard(), "로그인필요");
    tabBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tabBar->setTabEnabled(2, false);
    ui->verticalLayout->layout()->addWidget(tabBar);
    setLayout(ui->verticalLayout);
    connect(homePage, &Home::logined, this, [=](){
        tabBar->setTabEnabled(2, true);
        tabBar->setTabText(2, "대시보드");
    });
    connect(homePage, &Home::logout, this, [=](){
        tabBar->setTabEnabled(2, false);
        tabBar->setTabText(2, "로그인 필요");
    });
    connect(homePage, &Home::search, this, [=](const QString& keyword){
        tabBar->setCurrentIndex(1);
        chartPage->setKeyword(keyword);
    });

    //ui connection
    connect(ui->quitBtn, &QPushButton::clicked, this, &Root::close);
}

void Root::closeEvent(QCloseEvent *event)
{
    QSqlDatabase::database().close();
    qDebug() << "Application is closing";

    event->accept();
}
Root::~Root()
{
    delete ui;
}
