#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "root.h"
#include "source/home/home.h"
#include "source/login/login.h"
#include "source/signup/signup.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("E:/github/hts/project1/users.db");
    db.open();

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "name TEXT, "
               "acc TEXT, "
               "id TEXT PRIMARY KEY, "
               "pw TEXT, "
               "balance INTEGER DEFAULT 100000"
               ")");

    // -----------------------------
    // UI 생성
    // -----------------------------
    TabWidget *mainWindow = new TabWidget;
    mainWindow->resize(1300, 700);

    login *loginPage = new login;
    signup *signupPage = new signup;

    Home *homePage = new Home(loginPage, signupPage);  // 🔥 전달됨

    mainWindow->addTab(homePage, "Home");
    mainWindow->addTab(new QWidget, "tab2");
    mainWindow->addTab(new QWidget, "tab3");

    mainWindow->show();
    return a.exec();
}
