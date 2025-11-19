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
    QString currentUserId() const;
    void setCurrentUserId(const QString &id) { m_currentUserId = id; }

private:
    QSqlDatabase m_db;
    QString m_currentUserId;

};

#endif // DATABASEMANAGER_H
