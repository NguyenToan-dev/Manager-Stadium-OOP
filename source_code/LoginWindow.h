#pragma once

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QPixmap>
#include <QMessageBox>
#include <QKeyEvent>

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

    QString getUsername() const;

signals:
    void loginSuccess();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onLoginClicked();

private:
    void setupUi();
    void applyStyles();

    QLabel* imageLabel_;
    QLabel* titleLabel_;
    QLineEdit* usernameEdit_;
    QLineEdit* passwordEdit_;
    QPushButton* loginButton_;
    QWidget* rightPanel_;
    QVBoxLayout* mainLayout_;
    QCheckBox* showPasswordCheckBox_;
};
