#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    bool connectDB();
    QSqlDatabase &database();

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
