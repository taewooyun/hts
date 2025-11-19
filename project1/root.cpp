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

    // ------------------------------
    // 1) API 호출
    // ------------------------------
    QString url =
        "https://apis.data.go.kr/1160100/service/GetStockSecuritiesInfoService/getStockPriceInfo?serviceKey=7512965b9438146d549e4f26c6966672ca138df8f6b9cbe8c32047e46c8f5a24&numOfRows=30&resultType=json";

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

    // ------------------------------
    // 2) 탭 구성
    // ------------------------------
    TabWidget *tabBar = new TabWidget;

    login *loginPage = new login;
    signup *signupPage = new signup;
    Home *homePage = new Home(loginPage, signupPage);

    // ⭐ Chart는 단 한 번만 생성해야 한다!!
    Chart *chartPage = new Chart();

    tabBar->addTab(homePage, "홈");
    tabBar->addTab(chartPage, "차트");  // 여기 chartPage 넣기
    tabBar->addTab(new DashBoard(), "대시보드");
    tabBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui->verticalLayout->layout()->addWidget(tabBar);
    setLayout(ui->verticalLayout);

    tabBar->setCurrentIndex(0);   // 처음에는 홈 페이지

    // ------------------------------
    // 3) 탭 이동 시 차트에 userId 갱신
    // ------------------------------
    connect(tabBar, &QTabWidget::currentChanged, this, [chartPage](int index) {
        if (index == 1) {  // 차트 탭 index
            chartPage->refreshUserId();
        }
    });

    // ------------------------------
    // 4) 종료 버튼
    // ------------------------------
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
