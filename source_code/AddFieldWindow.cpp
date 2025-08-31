#include "AddFieldWindow.h"
#include "TimeWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QIntValidator>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QImageReader>
#include <QCoreApplication>
#include <QPainter>

AddFieldWindow::AddFieldWindow(const QString& sport, QWidget* parent)
    : QWidget(parent), editing(false), sportName(sport) {
    setWindowTitle("Thêm sân mới");
    setFixedSize(360, 400);

    QWidget* nameContainer = new QWidget;
    QHBoxLayout* nameContainerLayout = new QHBoxLayout(nameContainer);
    nameContainerLayout->setContentsMargins(4, 4, 4, 4);
    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText("VD: Sân A");
    nameContainerLayout->addWidget(nameEdit);

    QWidget* priceContainer = new QWidget;
    QHBoxLayout* priceContainerLayout = new QHBoxLayout(priceContainer);
    priceContainerLayout->setContentsMargins(4, 4, 4, 4);
    priceEdit = new QLineEdit;
    priceEdit->setPlaceholderText("VD: 150000");
    priceEdit->setValidator(new QIntValidator(0, 100000000, this));
    priceContainerLayout->addWidget(priceEdit);

    nameContainer->setObjectName("NameContainer");
    priceContainer->setObjectName("PriceContainer");
    nameEdit->setObjectName("NameInput");
    priceEdit->setObjectName("PriceInput");

    QLabel* nameLabel = new QLabel("Tên sân:");
    QLabel* priceLabel = new QLabel("Giá sân:");

    confirmButton = new QPushButton("Xác nhận");
    confirmButton->setObjectName("confirmButton");
    confirmButton->setCursor(Qt::PointingHandCursor);

    backButton = new QPushButton("← Quay lại");
    backButton->setObjectName("backButton");
    backButton->setCursor(Qt::PointingHandCursor);

    selectImageButton = new QPushButton("Chọn ảnh sân");
    selectImageButton->setObjectName("selectImageButton");
    selectImageButton->setCursor(Qt::PointingHandCursor);

    imagePathLabel = new QLabel("Chưa chọn ảnh");
    imagePreviewLabel = new QLabel;
    imagePreviewLabel->setFixedSize(320, 150);
    imagePreviewLabel->setObjectName("imagePreviewLabel");
    imagePreviewLabel->setAlignment(Qt::AlignCenter);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->addRow(nameLabel, nameContainer);
    formLayout->addRow(priceLabel, priceContainer);

    QVBoxLayout* imageLayout = new QVBoxLayout;
    imageLayout->addWidget(selectImageButton);
    imageLayout->addWidget(imagePathLabel);
    imageLayout->addWidget(imagePreviewLabel);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(backButton, 0, Qt::AlignLeft);
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton, 0, Qt::AlignRight);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(8);
    mainLayout->addLayout(imageLayout);
    mainLayout->addSpacing(8);
    mainLayout->addLayout(buttonLayout);

    setStyleSheet(R"(
        /* === Main Widget Background === */
        QWidget {
            background-color: #3e4a59;
        }

        /* === Containers === */
        #NameContainer, #PriceContainer {
            background-color: #3e4a59;
            border: 1px solid #4a637d;
            border-radius: 4px;
        }

        /* === Labels === */
        QLabel {
            background-color: transparent;
            color: #bdc3c7;
            font-size: 13px;
            font-weight: 500;
        }

        /* === Input Fields === */
        #NameInput, #PriceInput {
            background-color: #3e4a59;
            border: 1px solid #4a637d;
            border-radius: 4px;
            padding: 0 12px;
            font-size: 14px;
            color: #e0e0e0;
            min-height: 40px;
        }
        #NameInput:focus, #PriceInput:focus {
            border-color: #0d6efd;
        }

        /* === Image Preview === */
        #imagePreviewLabel {
            border: 1px dashed #4a637d;
            background-color: #2c3e50;
            color: #bdc3c7;
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

        /* === Tertiary/Link-style Button (Select Image) === */
        #selectImageButton {
            font-size: 13px;
            font-weight: 500;
            color: #3498db;
            background-color: transparent;
            border: none;
            padding: 5px;
        }
        #selectImageButton:hover {
            color: #5dade2;
            text-decoration: underline;
        }
    )");

    connect(confirmButton, &QPushButton::clicked, this, &AddFieldWindow::onConfirmClicked);
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit backToFieldList();
        this->close();
    });

    connect(selectImageButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Chọn ảnh sân", "", "Ảnh (*.png *.jpg *.jpeg)");
        if (!filePath.isEmpty() && QFile::exists(filePath)) {
            imagePath = filePath;
            imagePathLabel->setText("Đã chọn: " + QFileInfo(filePath).fileName());

            QPixmap originalPixmap(imagePath);
            if (!originalPixmap.isNull()) {
                QSize labelSize = imagePreviewLabel->size();
                QPixmap displayPixmap = originalPixmap;

                // Nếu ảnh lớn hơn label, scale xuống vừa với label, giữ tỷ lệ
                if (originalPixmap.width() > labelSize.width() || originalPixmap.height() > labelSize.height()) {
                    displayPixmap = originalPixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                }

                imagePreviewLabel->setPixmap(displayPixmap);
            }
        }
    });
}

void AddFieldWindow::setExistingFieldNames(const QList<QString>& existingFieldNames) {
    fieldList = existingFieldNames;
}

void AddFieldWindow::setEditMode(const QString& fieldName, int fieldPrice, QList<QString>& existing) {
    editing = true;
    originalName = fieldName;
    setExistingFieldNames(existing);
    nameEdit->setText(fieldName);
    priceEdit->setText(QString::number(fieldPrice));
    setWindowTitle("Sửa sân");

    QString imageFolder = QCoreApplication::applicationDirPath() + "/images/field/" + sportName + '/';
    QStringList extensions = {"png", "jpg", "jpeg"};
    for (const QString& ext : extensions) {
        QString path = imageFolder + "/" + fieldName + "." + ext;
        if (QFile::exists(path)) {
            QPixmap pixmap(path);
            if (!pixmap.isNull()) {
                QPixmap scaledPixmap = pixmap.scaled(320, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                QPixmap fittedPixmap(320, 180);
                fittedPixmap.fill(Qt::transparent);
                QPainter painter(&fittedPixmap);
                painter.setRenderHint(QPainter::SmoothPixmapTransform);
                int x = (320 - scaledPixmap.width()) / 2;
                int y = (180 - scaledPixmap.height()) / 2;
                painter.drawPixmap(x, y, scaledPixmap);
                imagePreviewLabel->setPixmap(fittedPixmap);
                imagePathLabel->setText("Ảnh hiện tại: " + fieldName + "." + ext);
                break;
            }
        }
    }
}

void AddFieldWindow::onConfirmClicked() {
    QString name = nameEdit->text().trimmed();
    QString priceStr = priceEdit->text().trimmed();
    int price = priceStr.toInt();

    if (name.isEmpty() || price <= 0) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Lỗi");
        msgBox.setText("Vui lòng nhập tên sân và giá hợp lệ!");
        msgBox.setIcon(QMessageBox::Warning);

        // --- Style riêng cho QMessageBox ---
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background-color: #3e4a59;
                color: #e0e0e0;
                font-size: 13px;
                font-weight: 500;
            }
            QPushButton {
                background-color: #0d6efd;
                color: white;
                border-radius: 4px;
                padding: 6px 12px;
                min-width: 80px;
            }
            QPushButton:hover {
                background-color: #0b5ed7;
            }
        )");

        msgBox.exec();

        return;
    }

    bool isDuplicate = std::any_of(fieldList.begin(), fieldList.end(), [&](const QString& existingName) {
        if (editing && existingName.compare(originalName, Qt::CaseInsensitive) == 0)
            return false;
        return existingName.compare(name, Qt::CaseInsensitive) == 0;
    });

    if (isDuplicate) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Lỗi");
        msgBox.setText("Sân đã tồn tại!");
        msgBox.setIcon(QMessageBox::Warning);

        // --- Style riêng cho QMessageBox ---
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background-color: #3e4a59;
                color: #e0e0e0;
                font-size: 13px;
                font-weight: 500;
            }
            QPushButton {
                background-color: #0d6efd;
                color: white;
                border-radius: 4px;
                padding: 6px 12px;
                min-width: 80px;
            }
            QPushButton:hover {
                background-color: #0b5ed7;
            }
        )");

        msgBox.exec();
        return;
    }

    QString imageFolder = QCoreApplication::applicationDirPath() + "/images/field/" + sportName + '/';
    QDir dir;
    if (!dir.exists(imageFolder)) {
        dir.mkpath(imageFolder);
    }

    if (!imagePath.isEmpty()) {
        QString fileExtension = QFileInfo(imagePath).suffix().toLower();
        if (fileExtension != "png" && fileExtension != "jpg" && fileExtension != "jpeg") {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Lỗi");
            msgBox.setText("Định dạng ảnh không hợp lệ!");
            msgBox.setIcon(QMessageBox::Warning);

            // --- Style riêng cho QMessageBox ---
            msgBox.setStyleSheet(R"(
                QMessageBox {
                    background-color: #3e4a59;
                    color: #e0e0e0;
                    font-size: 13px;
                    font-weight: 500;
                }
                QPushButton {
                    background-color: #0d6efd;
                    color: white;
                    border-radius: 4px;
                    padding: 6px 12px;
                    min-width: 80px;
                }
                QPushButton:hover {
                    background-color: #0b5ed7;
                }
            )");

            msgBox.exec();
            return;
        }

        QString savedFileName = name + "." + fileExtension;
        QString savedPath = imageFolder + "/" + savedFileName;

        QPixmap originalPixmap(imagePath);
        if (originalPixmap.isNull()) {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Lỗi");
            msgBox.setText("Không thể đọc được ảnh đã chọn!");
            msgBox.setIcon(QMessageBox::Warning);

            // --- Style riêng cho QMessageBox ---
            msgBox.setStyleSheet(R"(
                QMessageBox {
                    background-color: #3e4a59;
                    color: #e0e0e0;
                    font-size: 13px;
                    font-weight: 500;
                }
                QPushButton {
                    background-color: #0d6efd;
                    color: white;
                    border-radius: 4px;
                    padding: 6px 12px;
                    min-width: 80px;
                }
                QPushButton:hover {
                    background-color: #0b5ed7;
                }
            )");

            msgBox.exec();
            return;
        }

        QPixmap scaled = originalPixmap.scaled(320, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        if (!scaled.save(savedPath)) {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Lỗi");
            msgBox.setText("Không thể lưu ảnh!");
            msgBox.setIcon(QMessageBox::Warning);

            // --- Style riêng cho QMessageBox ---
            msgBox.setStyleSheet(R"(
                QMessageBox {
                    background-color: #3e4a59;
                    color: #e0e0e0;
                    font-size: 13px;
                    font-weight: 500;
                }
                QPushButton {
                    background-color: #0d6efd;
                    color: white;
                    border-radius: 4px;
                    padding: 6px 12px;
                    min-width: 80px;
                }
                QPushButton:hover {
                    background-color: #0b5ed7;
                }
            )");

            msgBox.exec();
            return;
        }
    }

    QString priceFolder = QCoreApplication::applicationDirPath() + "/data/price/" + sportName + "/";
    QDir().mkpath(priceFolder);

    // Tạo tên file từ tên sân: loại bỏ dấu và thay khoảng trắng bằng '_'
    QString sanitizedFieldName = TimeWindow::removeAccents(name).replace(" ", "_");
    QString rateFilePath = priceFolder + sanitizedFieldName + ".txt";

    QFile rateFile(rateFilePath);
    if (rateFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&rateFile);
        // Mặc định khung giờ toàn sân: 05:00-22:00
        out << "05:00-22:00-" << price << "\n";
        rateFile.close();
    }


    emit fieldConfirmed(name, price);
    this->close();
}
