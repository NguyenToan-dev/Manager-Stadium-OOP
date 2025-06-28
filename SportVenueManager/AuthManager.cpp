#include "AuthManager.h"

AuthManager& AuthManager::instance()
{
    static AuthManager instance;
    return instance;
}

bool AuthManager::validate(const QString& username, const QString& password)
{
    // Simple hardcoded check; replace with strategy for extensibility
    return username == "admin" && password == "admin";
}