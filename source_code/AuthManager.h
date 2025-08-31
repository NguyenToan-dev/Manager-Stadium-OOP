#pragma once
#include <QString>
#include <QCryptographicHash>
#include <QCoreApplication>

class AuthManager {
public:
    static AuthManager& instance();

    bool registerUser(const QString& username, const QString& password, int roleIndex, const QString& firstName, const QString& lastName);
    bool validate(const QString& username, const QString& password);
    bool isUnique(const QString& username);
    QString encryptMessage(const QString& chatKey, const QString& plainText);
    QString decryptMessage(const QString& chatKey, const QString& cipherText);
    QString generateSalt(int length = 16);
    QByteArray hashPassword(const QString& password, const QString& salt, int iterations = 100000);

    struct UserInfo {
        QString username;
        int role;
        QString firstName;
        QString lastName;
        bool valid;
    };
    const UserInfo& getCurrentUser() const { return currentUser; }
protected:
    AuthManager() {}
private:
    UserInfo currentUser;
    const QString FILE_NAME = QCoreApplication::applicationDirPath() + "/data/Username.txt";
};


