#include "BookingInfoWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QHBoxLayout>

BookingInfoWindow::BookingInfoWindow(QWidget* parent)
    : QWidget(parent) {
    setWindowTitle("Thông tin người đặt");
    setFixedSize(360, 200);

    QLabel* nameLabel = new QLabel("Họ và tên:");
    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText("Nhập họ tên");
    nameEdit->setMaxLength(20);

    QLabel* phoneLabel = new QLabel("Số điện thoại:");
    phoneEdit = new QLineEdit;
    phoneEdit->setPlaceholderText("10 chữ số");
    phoneEdit->setMaxLength(10);

    confirmButton = new QPushButton("Xác nhận");
    confirmButton->setObjectName("confirmButton");
    backButton = new QPushButton("← Quay lại");
    backButton->setObjectName("backButton");

    // Layout form
    QFormLayout* formLayout = new QFormLayout;
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setFormAlignment(Qt::AlignCenter);
    formLayout->setHorizontalSpacing(12);
    formLayout->setVerticalSpacing(10);
    formLayout->addRow(nameLabel, nameEdit);
    formLayout->addRow(phoneLabel, phoneEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(backButton, 0, Qt::AlignLeft);
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton, 0, Qt::AlignRight);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(8);
    mainLayout->addLayout(buttonLayout);

    setStyleSheet(R"(
        /* === Main Widget Background === */
        QWidget {
            background-color: #3e4a59;
        }

        /* === Labels === */
        QLabel {
            background-color: transparent;
            color: #bdc3c7;
            font-size: 13px;
            font-weight: 500;
        }

        /* === Input Fields (LineEdit) === */
        QLineEdit {
            background-color: #3e4a59;
            border: 1px solid #4a637d;
            border-radius: 4px;
            padding: 0 12px;
            font-size: 14px;
            color: #e0e0e0;
            min-height: 40px;
        }
        QLineEdit:focus {
            border-color: #0d6efd;
        }

        /* === Button Base Style === */
        QPushButton {
            padding: 10px 20px;
            font-size: 14px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
        }

        /* === Primary Action Button (Confirm) === */
        #confirmButton {
            background-color: #0d6efd;
            color: white;
        }
        #confirmButton:hover {
            background-color: #0b5ed7;
        }

        /* === Secondary Button (Back) === */
        #backButton {
            background-color: #34495e;
            color: #e0e0e0;
            border: 1px solid #4a637d;
        }
        #backButton:hover {
            background-color: #4a637d;
        }
    )");

    connect(confirmButton, &QPushButton::clicked, this, &BookingInfoWindow::onConfirmClicked);
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit backToTimeSelection();
        this->close();
    });
}

void BookingInfoWindow::onConfirmClicked() {
    QString name = nameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();

    if (name.isEmpty() || phone.isEmpty()) {
        QMessageBox::warning(this, "Thiếu thông tin", "Vui lòng nhập đầy đủ họ tên và số điện thoại!");
        return;
    }

    QRegularExpression regex("^\\d{10}$");
    if (!regex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Sai định dạng", "Số điện thoại phải gồm đúng 10 chữ số.");
        return;
    }

    emit infoConfirmed(name, phone);
    this->close();
}
