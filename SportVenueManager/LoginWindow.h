#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccess();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onLoginClicked();
    void onClearClicked();

private:
    void setupUi();
    void applyStyles();

    QLabel* imageLabel_;
    QLabel* titleLabel_;
    QLineEdit* usernameEdit_;
    QLineEdit* passwordEdit_;
    QPushButton* loginButton_;
    QPushButton* clearButton_;
    QWidget* rightPanel_;
    QVBoxLayout* mainLayout_;
};

#endif // LOGINWINDOW_H
