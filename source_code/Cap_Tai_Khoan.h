// Cap_Tai_Khoan.h
#ifndef CAP_TAI_KHOAN_H
#define CAP_TAI_KHOAN_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include "AuthManager.h"

class Cap_Tai_Khoan : public QDialog {
    Q_OBJECT
public:
    explicit Cap_Tai_Khoan(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* ev) override;

private slots:
    void onGeneratePassword();
    void onToggleShowPassword(bool checked);
    void onCreateAccount();

private:
    void setupUi();
    void applyPalette();
    QString generateStrongPassword(int length);

    QLineEdit* editUsername;
    QLineEdit* editFirstName;
    QLineEdit* editLastName;
    QComboBox* comboRole;
    QLineEdit* editPassword;
    QPushButton* btnGenerate;
    QPushButton* btnCreate;
    QPushButton* btnCancel;
    QCheckBox* chkShowPassword;
};

#endif // CAP_TAI_KHOAN_H
