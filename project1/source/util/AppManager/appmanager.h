#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QScopedPointer>

#include "DatabaseManager.h"
#include "apimanager.h"

class AppManager : public QObject
{
    Q_OBJECT

public:
    static AppManager &instance(); // 싱글톤 인스턴스 반환

    DatabaseManager *db() const;
    ApiManager *api() const;
    QString getConstant(const QString &key) const;

private:
    explicit AppManager(QObject *parent = nullptr);
    ~AppManager();

    QScopedPointer<DatabaseManager> m_dbManager;
    QScopedPointer<ApiManager> m_apiManager;
    QHash<QString, QString> m_constants;
};

#endif // APPMANAGER_H
