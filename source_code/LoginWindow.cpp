#include "LoginWindow.h"
#include "AuthManager.h"
#include <QCoreApplication>

LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(800, 500);
    setWindowTitle("Đăng nhập hệ thống");

    setupUi();
    applyStyles();

    connect(loginButton_, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);

    connect(showPasswordCheckBox_, &QCheckBox::toggled, [=](bool checked) {
        passwordEdit_->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });

    usernameEdit_->installEventFilter(this);
    passwordEdit_->installEventFilter(this);
}

LoginWindow::~LoginWindow() {}

QString LoginWindow::getUsername() const { return usernameEdit_->text(); }

void LoginWindow::setupUi()
{
    imageLabel_ = new QLabel(this);
    QPixmap pix(QCoreApplication::applicationDirPath() + "/images/siu.jpg");
    if (!pix.isNull())
        imageLabel_->setPixmap(pix.scaled(350, 400, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    else
        imageLabel_->setText("Không tìm thấy ảnh");

    imageLabel_->setFixedWidth(350);
    imageLabel_->setScaledContents(true);

    rightPanel_ = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel_);
    rightLayout->setContentsMargins(40, 30, 40, 30);
    rightLayout->setSpacing(20);

    titleLabel_ = new QLabel("Đăng nhập hệ thống", this);
    titleLabel_->setObjectName("titleLabel"); // ADDED: Object name for styling
    titleLabel_->setAlignment(Qt::AlignCenter);

    QHBoxLayout* userLayout = new QHBoxLayout();
    QLabel* userIcon = new QLabel("👤", this);
    userIcon->setObjectName("iconLabel"); // ADDED: Object name for styling
    userIcon->setFixedWidth(24);
    usernameEdit_ = new QLineEdit(this);
    usernameEdit_->setPlaceholderText("Tên đăng nhập");
    userLayout->addWidget(userIcon);
    userLayout->addWidget(usernameEdit_);

    QHBoxLayout* passLayout = new QHBoxLayout();
    QLabel* passIcon = new QLabel("🔒", this);
    passIcon->setObjectName("iconLabel"); // ADDED: Object name for styling
    passIcon->setFixedWidth(24);
    passwordEdit_ = new QLineEdit(this);
    passwordEdit_->setPlaceholderText("Mật khẩu");
    passwordEdit_->setEchoMode(QLineEdit::Password);
    passLayout->addWidget(passIcon);
    passLayout->addWidget(passwordEdit_);

    showPasswordCheckBox_ = new QCheckBox("Hiển thị mật khẩu", this);

    loginButton_ = new QPushButton("Đăng nhập", this);

    rightLayout->addWidget(titleLabel_);
    rightLayout->addLayout(userLayout);
    rightLayout->addLayout(passLayout);
    rightLayout->addWidget(showPasswordCheckBox_);
    rightLayout->addStretch();
    rightLayout->addWidget(loginButton_);

    QHBoxLayout* fullLayout = new QHBoxLayout();
    fullLayout->setSpacing(0);
    fullLayout->setContentsMargins(0, 0, 0, 0);
    fullLayout->addWidget(imageLabel_);
    fullLayout->addWidget(rightPanel_);

    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->addLayout(fullLayout);
    setLayout(mainLayout_);
}

void LoginWindow::applyStyles()
{
    this->setStyleSheet(R"(
        QWidget {
            font-family: "Segoe UI";
            background-color: #2c3e50;
        }

        QLabel {
            color: #bdc3c7;
            background-color: transparent;
            font-size: 11pt;
        }

        #titleLabel {
            color: #ffffff;
            font-size: 20pt;
            font-weight: bold;
        }

        #iconLabel {
            font-size: 14pt;
        }

        QLineEdit {
            color: #ecf0f1;
            background-color: #34495e;
            border: 1px solid #4a637d;
            border-radius: 5px;
            padding: 8px;
            font-size: 12pt;
        }

        QLineEdit:focus {
            border: 1px solid #0d6efd;
        }

        QPushButton {
            color: white;
            background-color: #0d6efd;
            border: none;
            border-radius: 5px;
            padding: 10px;
            font-size: 13pt;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #0b5ed7;
        }

        QCheckBox {
            color: #bdc3c7;
            spacing: 10px;
            font-size: 11pt;
        }

        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border-radius: 4px;
            border: 1px solid #4a637d;
            background-color: #34495e;
        }

        QCheckBox::indicator:checked {
            background-color: #0d6efd;
        }
    )");
}

void LoginWindow::onLoginClicked()
{
    QString u = usernameEdit_->text(), p = passwordEdit_->text();
    if (AuthManager::instance().validate(u, p)) {
        QMessageBox::information(this, "Thông báo", "Đăng nhập thành công");
        emit loginSuccess();
    }
    else {
        QMessageBox::warning(this, "Cảnh báo", "Sai tên hoặc mật khẩu");
    }
}

bool LoginWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (obj == usernameEdit_) {
            if (keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                passwordEdit_->setFocus();
                return true;
            }
        }
        else if (obj == passwordEdit_) {
            if (keyEvent->key() == Qt::Key_Up) {
                usernameEdit_->setFocus();
                return true;
            }
            else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                onLoginClicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}
