#pragma once

#include <QString>

// Singleton AuthManager, extensible for Strategy pattern
class AuthManager
{
public:
    static AuthManager& instance();

    // Strategy-ready: validate credentials (could be replaced with different strategies)
    bool validate(const QString& username, const QString& password);

private:
    AuthManager() = default;
    AuthManager(const AuthManager&) = delete;
    AuthManager& operator=(const AuthManager&) = delete;
};