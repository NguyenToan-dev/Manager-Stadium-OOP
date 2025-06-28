#include "LoginWindow.h"
#include "AuthManager.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QPixmap>
#include <QFile>
#include <QMessageBox>
#include <QKeyEvent>

// Constructor của cửa sổ đăng nhập
LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(700, 400); // Đặt kích thước cố định cho cửa sổ
    setWindowTitle("Đăng nhập hệ thống"); // Tiêu đề cửa sổ
    setStyleSheet("background-color: white;"); // Màu nền
    setupUi();     // Tạo giao diện
    applyStyles(); // Áp dụng style cho các thành phần

    // Kết nối nút với sự kiện
    connect(loginButton_, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(clearButton_, &QPushButton::clicked, this, &LoginWindow::onClearClicked);

    // Cài đặt bộ lọc sự kiện để xử lý phím tắt
    usernameEdit_->installEventFilter(this);
    passwordEdit_->installEventFilter(this);
}

// Destructor
LoginWindow::~LoginWindow() {}

// Hàm tạo bố cục giao diện
void LoginWindow::setupUi()
{
    // Tạo ảnh bên trái
    imageLabel_ = new QLabel(this);
    QPixmap pix("D:/LABOOP/SportVenueManager/image/stadium.jpg");
    if (!pix.isNull())
        imageLabel_->setPixmap(pix.scaled(350, 400, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    else
        imageLabel_->setText("Không tìm thấy ảnh");

    imageLabel_->setFixedWidth(350);
    imageLabel_->setScaledContents(true);

    // Tạo khung bên phải chứa form đăng nhập
    rightPanel_ = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel_);
    rightLayout->setContentsMargins(30, 30, 30, 30);
    rightLayout->setSpacing(20);

    // Tiêu đề
    titleLabel_ = new QLabel("Đăng nhập hệ thống", this);
    titleLabel_->setAlignment(Qt::AlignCenter);

    // Layout nhập tên người dùng
    QHBoxLayout* userLayout = new QHBoxLayout();
    QLabel* userIcon = new QLabel("👤", this);
    userIcon->setFixedWidth(24);
    usernameEdit_ = new QLineEdit(this);
    usernameEdit_->setPlaceholderText("Tên đăng nhập");
    userLayout->addWidget(userIcon);
    userLayout->addWidget(usernameEdit_);

    // Layout nhập mật khẩu
    QHBoxLayout* passLayout = new QHBoxLayout();
    QLabel* passIcon = new QLabel("🔒", this);
    passIcon->setFixedWidth(24);
    passwordEdit_ = new QLineEdit(this);
    passwordEdit_->setPlaceholderText("Mật khẩu");
    passwordEdit_->setEchoMode(QLineEdit::Password); // Ẩn ký tự
    passLayout->addWidget(passIcon);
    passLayout->addWidget(passwordEdit_);

    // Các nút
    loginButton_ = new QPushButton("Đăng nhập", this);
    clearButton_ = new QPushButton("Xoá", this);

    // Thêm vào bố cục
    rightLayout->addWidget(titleLabel_);
    rightLayout->addLayout(userLayout);
    rightLayout->addLayout(passLayout);
    rightLayout->addWidget(loginButton_);
    rightLayout->addWidget(clearButton_);
    rightLayout->addStretch(); // Đẩy nội dung lên trên

    // Bố cục chính gồm ảnh + form
    QHBoxLayout* fullLayout = new QHBoxLayout();
    fullLayout->setSpacing(0);
    fullLayout->setContentsMargins(0, 0, 0, 0);
    fullLayout->addWidget(imageLabel_);
    fullLayout->addWidget(rightPanel_);

    // Layout tổng thể
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->addLayout(fullLayout);
    setLayout(mainLayout_);
}

// Hàm áp dụng style cho các widget
void LoginWindow::applyStyles()
{
    // Font và màu tiêu đề
    QFont titleFont("Segoe UI", 20, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setStyleSheet("color: #22223B;");

    // Style cho ô nhập liệu
    QFont inputFont("Segoe UI", 12);
    QString inputStyle = R"(
        QLineEdit {
            padding: 8px;
            border: 1.5px solid #4A4E69;
            border-radius: 6px;
            background-color: #F8F8FF;
            color: #22223B;
        }
        QLineEdit:focus {
            border: 1.5px solid #0078D7;
            background-color: #fff;
        }
    )";
    usernameEdit_->setFont(inputFont);
    usernameEdit_->setStyleSheet(inputStyle);
    passwordEdit_->setFont(inputFont);
    passwordEdit_->setStyleSheet(inputStyle);

    // Style cho nút Đăng nhập
    loginButton_->setFixedHeight(38);
    loginButton_->setStyleSheet(R"(
        QPushButton {
            background-color: #0078D7;
            color: white;
            border: none;
            border-radius: 6px;
            font-weight: bold;
            font-size: 15px;
        }
        QPushButton:hover {
            background-color: #005A9E;
        }
    )");

    // Style cho nút Xoá
    clearButton_->setFixedHeight(32);
    clearButton_->setStyleSheet(R"(
        QPushButton {
            background-color: #E63946;
            color: white;
            border: none;
            border-radius: 6px;
            font-weight: bold;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #B22234;
        }
    )");
}

// Hàm xử lý khi bấm nút đăng nhập
void LoginWindow::onLoginClicked()
{
    QString username = usernameEdit_->text();
    QString password = passwordEdit_->text();

    // Kiểm tra xác thực thông qua AuthManager
    if (AuthManager::instance().validate(username, password)) {
        // Thông báo thành công
        QMessageBox msgBox;
        msgBox.setWindowTitle("Thông báo");
        msgBox.setText("Đăng nhập thành công!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(R"(...)"); // Định dạng thông báo
        msgBox.exec();

        emit loginSuccess(); // Gửi tín hiệu đăng nhập thành công
        close(); // Đóng cửa sổ
    }
    else {
        // Thông báo thất bại
        QMessageBox msgBox;
        msgBox.setWindowTitle("Cảnh báo");
        msgBox.setText("Sai tên đăng nhập hoặc mật khẩu!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(R"(...)"); // Định dạng thông báo
        msgBox.exec();
    }
}

// Hàm xử lý khi bấm nút Xoá
void LoginWindow::onClearClicked()
{
    usernameEdit_->clear();
    passwordEdit_->clear();
    usernameEdit_->setFocus(); // Đặt lại con trỏ tại username
}

// Hàm xử lý phím tắt và di chuyển giữa các ô nhập
bool LoginWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        // Nếu đang ở ô username
        if (obj == usernameEdit_) {
            if (keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                passwordEdit_->setFocus(); // Chuyển xuống password
                return true;
            }
        }
        // Nếu đang ở ô password
        else if (obj == passwordEdit_) {
            if (keyEvent->key() == Qt::Key_Up) {
                usernameEdit_->setFocus(); // Quay lại username
                return true;
            }
            else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                onLoginClicked(); // Gọi đăng nhập
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event); // Trả về xử lý mặc định
}
