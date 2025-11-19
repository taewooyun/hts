#include "root.h"
#include "source/chart/chart.h"
#include "source/dashboard/dashboard.h"
#include "source/home/home.h"
#include "source/login/login.h"
#include "tab.h"
#include "ui_root.h"
#include <QTabWidget>
#include "./source/util/AppManager/appmanager.h"

Root::Root(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
    , ui(new Ui::Root)
{
    ui->setupUi(this);
    resize(1300, 700);

    QString url =
        "https://apis.data.go.kr/1160100/service/GetStockSecuritiesInfoService/getStockPriceInfo?serviceKey=7512965b9438146d549e4f26c6966672ca138df8f6b9cbe8c32047e46c8f5a24&numOfRows=30&resultType=json"
        ;
    connect(AppManager::instance().api(), &ApiManager::responseReceived,
            this, [this](const QByteArray &response) {
                QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                if (jsonDoc.isObject()) {
                    QJsonObject responseObj = jsonDoc.object()["response"].toObject();
                    ApiResponse api = ApiResponse::fromJson(responseObj);

                    AppManager::instance().api()->stockList = api.body.items;
                    disconnect(AppManager::instance().api(), &ApiManager::responseReceived, this, nullptr);
                }
            });

    AppManager::instance().api()->get(QUrl(url));

    TabWidget *tabBar = new TabWidget;

    login *loginPage = new login;
    signup *signupPage = new signup;
    Home *homePage = new Home(loginPage, signupPage);

    tabBar->addTab(homePage, "홈");
    tabBar->addTab(new Chart(), "차트");
    tabBar->addTab(new DashBoard(), "대시보드");
    tabBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui->verticalLayout->layout()->addWidget(tabBar);
    setLayout(ui->verticalLayout);

    tabBar->setCurrentIndex(2);

    connect(ui->quitBtn, &QPushButton::clicked, this, &Root::close);
}

void Root::closeEvent(QCloseEvent *event)
{
    QSqlDatabase::database().close();
    qDebug() << "Application is closing";

    event->accept();  // 또는 event->ignore()로 취소 가능
}
Root::~Root()
{
    delete ui;
}
