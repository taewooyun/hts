#include "root.h"
#include <QApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "./source/util/AppManager/appmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppManager::instance().db()->connectDB();

    QSqlQuery query(AppManager::instance().db()->database());
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "name TEXT, "
               "acc TEXT, "
               "id TEXT PRIMARY KEY, "
               "pw TEXT, "
               "balance INTEGER DEFAULT 100000"
               ")");


    Root r;
    r.show();

    return a.exec();
}
