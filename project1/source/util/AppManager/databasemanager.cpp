#include "databasemanager.h"
#include <QSqlError>
#include <QDebug>
#include <QDir>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {
}

bool DatabaseManager::connectDB() {

    QString folderPath = "./data";
    QDir dir;
    if (!dir.exists(folderPath)) {
        dir.mkpath(folderPath);  // 폴더 없으면 생성
    }
    QString dbPath = folderPath + "/mydb.sqlite";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    qDebug() << "Current working directory:" << QDir::currentPath();
    if (!m_db.open()) {
        qWarning() << "Failed to open DB:" << m_db.lastError();
        return false;
    }

    return true;
}

QSqlDatabase &DatabaseManager::database() {
    return m_db;
}

QString DatabaseManager::currentUserId() const {
    return m_currentUserId;
}
