#include "root.h"
#include <QApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <qsqlerror.h>
#include "./source/util/AppManager/appmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");

    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Base, Qt::white);
    lightPalette.setColor(QPalette::AlternateBase, QColor(225, 225, 225));
    lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    QApplication::setPalette(lightPalette);


    AppManager::instance().db()->connectDB();

    QSqlQuery query(AppManager::instance().db()->database());
    query.prepare(
        "CREATE TABLE IF NOT EXISTS User ("
        "id TEXT PRIMARY KEY,"
        "pw TEXT NOT NULL,"
        "name TEXT NOT NULL,"
        "acc TEXT NOT NULL,"
        "balance INTEGER DEFAULT 0"
        ")"
        );
    if(!query.exec()){
        qDebug() << "User ERR: "+query.lastError().text();
    }

    query.prepare(
        "CREATE TABLE IF NOT EXISTS User_Stock ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "userId INTEGER NOT NULL,"
        "name TEXT NOT NULL,"
        "market TEXT NOT NULL,"
        "price INTEGER NOT NULL,"
        "marketCap INTEGER,"
        "FOREIGN KEY (userId) REFERENCES User(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE"
        ")"
        );
    if(!query.exec()){
        qDebug() << "Stock ERR: "+query.lastError().text();
    }

    query.prepare(
        "CREATE TABLE IF NOT EXISTS Banking ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "userId INTEGER NOT NULL,"
        "type TEXT NOT NULL,"
        "target TEXT,"
        "amount INTEGER NOT NULL,"
        "memo TEXT,"
        "FOREIGN KEY (userId) REFERENCES User(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE"
        ")"
        );
    if(!query.exec()){
        qDebug() << "Banking ERR: "+query.lastError().text();
    }

    Root r;
    r.show();

    return a.exec();
}
