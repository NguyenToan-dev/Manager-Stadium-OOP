#include "Cap_Tai_Khoan.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QMessageBox>
#include <QKeyEvent>
#include <QRandomGenerator>
#include "AuthManager.h"

Cap_Tai_Khoan::Cap_Tai_Khoan(QWidget* parent)
    : QDialog(parent)
{
    setFixedSize(800, 650);
    setupUi();
    applyPalette();

    editUsername->installEventFilter(this);
    editFirstName->installEventFilter(this);
    editLastName->installEventFilter(this);
    comboRole->installEventFilter(this);
    editPassword->installEventFilter(this);

    connect(btnCreate, &QPushButton::clicked, this, &Cap_Tai_Khoan::onCreateAccount);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(btnGenerate, &QPushButton::clicked, this, &Cap_Tai_Khoan::onGeneratePassword);
    connect(chkShowPassword, &QCheckBox::toggled, this, &Cap_Tai_Khoan::onToggleShowPassword);
}

bool Cap_Tai_Khoan::eventFilter(QObject* obj, QEvent* ev)
{
    if (ev->type() == QEvent::KeyPress) {
        auto* keyEv = static_cast<QKeyEvent*>(ev);
        if (keyEv->key() == Qt::Key_Return || keyEv->key() == Qt::Key_Enter) {
            if (obj == editUsername)      editFirstName->setFocus();
            else if (obj == editFirstName) editLastName->setFocus();
            else if (obj == editLastName)  comboRole->setFocus();
            else if (obj == comboRole)     editPassword->setFocus();
            else if (obj == editPassword)  btnCreate->setFocus();
            return true;
        }
    }
    return QDialog::eventFilter(obj, ev);
}

void Cap_Tai_Khoan::setupUi()
{
    setWindowTitle(tr("Tạo Tài Khoản Mới"));

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addStretch();

    QFrame* formContainer = new QFrame();
    formContainer->setObjectName("formContainer");
    formContainer->setMaximumWidth(420);

    QVBoxLayout* formLayout = new QVBoxLayout(formContainer);
    formLayout->setSpacing(15);
    formLayout->setContentsMargins(30, 30, 30, 30);

    QLabel* titleLabel = new QLabel(tr("Tạo Tài Khoản"));
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* lblUser = new QLabel(tr("Tên người dùng (*):"));
    editUsername = new QLineEdit;
    editUsername->setPlaceholderText(tr("VD: nguyenvana"));

    QGridLayout* nameLayout = new QGridLayout();
    nameLayout->setHorizontalSpacing(10);
    nameLayout->setVerticalSpacing(5);
    QLabel* lblFirst = new QLabel(tr("Họ:"));
    editFirstName = new QLineEdit;
    editFirstName->setPlaceholderText(tr("Nguyễn Văn"));
    QLabel* lblLast = new QLabel(tr("Tên:"));
    editLastName = new QLineEdit;
    editLastName->setPlaceholderText(tr("An"));
    nameLayout->addWidget(lblFirst, 0, 0);
    nameLayout->addWidget(lblLast, 0, 1);
    nameLayout->addWidget(editFirstName, 1, 0);
    nameLayout->addWidget(editLastName, 1, 1);

    QLabel* lblRole = new QLabel(tr("Vai trò:"));
    comboRole = new QComboBox;
    comboRole->addItems({ tr("Chủ"), tr("Quản lý sân"), tr("Quản lý nhân sự"), tr("Quản lý khách hàng"), tr("Kế toán") });

    QLabel* lblPass = new QLabel(tr("Mật khẩu (*):"));
    editPassword = new QLineEdit;
    editPassword->setEchoMode(QLineEdit::Password);
    editPassword->setPlaceholderText(tr("Nhập mật khẩu an toàn"));

    chkShowPassword = new QCheckBox(tr("Hiện mật khẩu"));
    btnGenerate = new QPushButton(tr("Tạo ngẫu nhiên"));
    btnGenerate->setObjectName("generateButton"); // Added for specific styling

    btnCreate = new QPushButton(tr("Tạo tài khoản"));
    btnCreate->setObjectName("createButton");
    btnCreate->setCursor(Qt::PointingHandCursor);

    btnCancel = new QPushButton(tr("Huỷ bỏ"));
    btnCancel->setObjectName("cancelButton");
    btnCancel->setCursor(Qt::PointingHandCursor);

    QHBoxLayout* passOptionsLayout = new QHBoxLayout();
    passOptionsLayout->setContentsMargins(0, 5, 0, 0);
    passOptionsLayout->addWidget(chkShowPassword);
    passOptionsLayout->addStretch();
    passOptionsLayout->addWidget(btnGenerate);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(btnCancel);
    buttonLayout->addWidget(btnCreate);
    buttonLayout->setSpacing(10);

    formLayout->addWidget(titleLabel);
    formLayout->addSpacing(10);
    formLayout->addWidget(lblUser);
    formLayout->addWidget(editUsername);
    formLayout->addLayout(nameLayout);
    formLayout->addSpacing(15);
    formLayout->addWidget(lblRole);
    formLayout->addWidget(comboRole);
    formLayout->addWidget(lblPass);
    formLayout->addWidget(editPassword);
    formLayout->addLayout(passOptionsLayout);
    formLayout->addStretch(1);
    formLayout->addLayout(buttonLayout);

    mainLayout->addWidget(formContainer);
    mainLayout->addStretch();
}

void Cap_Tai_Khoan::applyPalette()
{
    this->setStyleSheet(R"(
        /* === Main Dialog Background === */
        QDialog {
            background-color: #3e4a59;
        }

        /* === Form Container === */
        #formContainer {
            background-color: #2c3e50;
            border-radius: 8px;
            border: 1px solid #4a637d;
        }

        /* === Labels & Text === */
        #formContainer QLabel {
            background-color: transparent;
            color: #bdc3c7;
            font-size: 13px;
            font-weight: 500;
        }
        #formContainer #titleLabel {
            font-size: 24px;
            font-weight: bold;
            color: #ffffff;
            margin-bottom: 10px;
        }

        /* === Input Fields (LineEdit, ComboBox) === */
        QLineEdit, QComboBox {
            background-color: #3e4a59;
            border: 1px solid #4a637d;
            border-radius: 4px;
            padding: 0 12px;
            font-size: 14px;
            color: #e0e0e0;
            min-height: 40px;
        }
        QLineEdit:focus, QComboBox:focus {
            border-color: #0d6efd; /* Bright blue highlight */
        }
        QComboBox::drop-down { border: none; }
        QComboBox QAbstractItemView {
            border: 1px solid #4a637d;
            background-color: #3e4a59;
            selection-background-color: #0d6efd;
            color: #e0e0e0;
            padding: 4px;
        }

        /* === CheckBox === */
        QCheckBox {
            color: #bdc3c7;
            font-size: 13px;
            spacing: 5px;
            background-color: transparent;
        }
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 1px solid #4a637d;
            border-radius: 3px;
        }
        QCheckBox::indicator:checked {
            background-color: #0d6efd;
        }

        /* === Button Base Style === */
        QPushButton {
             padding: 10px 20px;
             font-size: 14px;
             font-weight: bold;
             border-radius: 5px;
             border: none;
        }

        /* === Primary Action Button (Create) === */
        #createButton {
            background-color: #0d6efd;
            color: white;
        }
        #createButton:hover { background-color: #0b5ed7; }

        /* === Secondary Button (Cancel) === */
        #cancelButton {
             background-color: #34495e;
             color: #e0e0e0;
             border: 1px solid #4a637d;
        }
        #cancelButton:hover { background-color: #4a637d; }

        /* === Tertiary/Link-style Button (Generate) === */
        #generateButton {
            font-size: 13px;
            font-weight: 500;
            color: #3498db; /* Link blue */
            background-color: transparent;
            border: none;
            padding: 5px;
        }
        #generateButton:hover {
            color: #5dade2;
            text-decoration: underline;
        }
    )");
}

void Cap_Tai_Khoan::onGeneratePassword() {
    editPassword->setText(generateStrongPassword(12));
}

void Cap_Tai_Khoan::onToggleShowPassword(bool checked) {
    editPassword->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void Cap_Tai_Khoan::onCreateAccount() {
    QString username = editUsername->text().trimmed();
    QString password = editPassword->text();
    int roleIndex = comboRole->currentIndex();
    QString firstName = editFirstName->text().trimmed();
    QString lastName = editLastName->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, tr("Thiếu thông tin"), tr("Bạn phải nhập đủ tên người dùng và mật khẩu."));
        return;
    }
    if (!AuthManager::instance().isUnique(username)) {
        QMessageBox::critical(this, tr("Lỗi"), tr("Tên người dùng đã tồn tại."));
        return;
    }
    if (!AuthManager::instance().registerUser(username, password, roleIndex, firstName, lastName)) {
        QMessageBox::critical(this, tr("Lỗi"), tr("Không thể tạo tài khoản. Vui lòng thử lại."));
        return;
    }

    QMessageBox::information(this, tr("Thành công"), tr("Đã tạo tài khoản %1").arg(username));

    editUsername->clear();
    editPassword->clear();
    editFirstName->clear();
    editLastName->clear();
    comboRole->setCurrentIndex(0);
    chkShowPassword->setChecked(false);
}

QString Cap_Tai_Khoan::generateStrongPassword(int length) {
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+";
    QString pwd;
    for (int i = 0; i < length; ++i) {
        int idx = QRandomGenerator::global()->bounded(chars.size());
        pwd.append(chars.at(idx));
    }
    return pwd;
}
