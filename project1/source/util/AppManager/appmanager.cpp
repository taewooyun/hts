#include "appmanager.h"

// 싱글톤 인스턴스 생성
AppManager &AppManager::instance() {
    static AppManager _instance;
    return _instance;
}

AppManager::AppManager(QObject *parent) :
    QObject(parent),
    m_dbManager(new DatabaseManager),
    m_apiManager(new ApiManager),
    m_constants({{"id", ""},

                 })
{
}

AppManager::~AppManager() {}

DatabaseManager *AppManager::db() const {
    return m_dbManager.data();
}

ApiManager *AppManager::api() const {
    return m_apiManager.data();
}

QString AppManager::getConstant(const QString &key) const {
    return m_constants.value(key, "");
}
