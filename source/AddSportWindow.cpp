#include "AddSportWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QPixmap>
#include <QFileInfo>
#include <QPainter>
#include <QDir>
#include <QFile>
#include <QCoreApplication>

AddSportWindow::AddSportWindow(QWidget* parent)
    : QWidget(parent), editing(false) {
    setWindowTitle("Thêm môn thể thao");
    setWindowFlag(Qt::Window);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(360, 360);

    nameEdit = new QLineEdit;
    nameEdit->setMaxLength(18);
    nameEdit->setPlaceholderText("VD: Bóng đá");

    QLabel* nameLabel = new QLabel("Tên môn:");

    confirmButton = new QPushButton("Xác nhận");
    confirmButton->setObjectName("confirmButton");
    backButton = new QPushButton("← Quay lại");
    backButton->setObjectName("backButton");

    selectImageButton = new QPushButton("Chọn ảnh môn");
    imagePathLabel = new QLabel("Chưa chọn ảnh");
    imagePreviewLabel = new QLabel;
    imagePreviewLabel->setFixedSize(320, 150);
    imagePreviewLabel->setObjectName("imagePreviewLabel");
    imagePreviewLabel->setAlignment(Qt::AlignCenter);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->addRow(nameLabel, nameEdit);

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

    connect(confirmButton, &QPushButton::clicked, this, &AddSportWindow::onConfirmClicked);
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit backToSportList();
        this->close();
    });

    connect(selectImageButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Chọn ảnh môn", "", "Ảnh (*.png *.jpg *.jpeg)");
        if (!filePath.isEmpty() && QFile::exists(filePath)) {
            imagePath = filePath;
            imagePathLabel->setText("Đã chọn: " + QFileInfo(filePath).fileName());

            QPixmap originalPixmap(imagePath);
            if (!originalPixmap.isNull()) {
                QSize labelSize = imagePreviewLabel->size();

                // Scale giữ tỉ lệ, vừa với label
                QPixmap scaledPixmap = originalPixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                // Tạo pixmap đúng size label, nền trong suốt
                QPixmap fittedPixmap(labelSize);
                fittedPixmap.fill(Qt::transparent);

                QPainter painter(&fittedPixmap);
                painter.setRenderHint(QPainter::SmoothPixmapTransform);

                // Vẽ ở giữa
                int x = (labelSize.width() - scaledPixmap.width()) / 2;
                int y = (labelSize.height() - scaledPixmap.height()) / 2;
                painter.drawPixmap(x, y, scaledPixmap);

                imagePreviewLabel->setPixmap(fittedPixmap);
                imagePreviewLabel->show();
            } else {
                imagePreviewLabel->setText("Không thể tải ảnh.");
            }
        }
    });
}

void AddSportWindow::setExistingSportsNames(const QList<QString>& existing) {
    sportList = existing;
}

void AddSportWindow::setEditMode(const QString& sportName, QList<QString>& existing) {
    editing = true;
    originalName = sportName;
    setExistingSportsNames(existing);

    nameEdit->setText(sportName);
    nameEdit->setDisabled(true);
    setWindowTitle("Sửa ảnh môn thể thao");

    QString imageFolder = QCoreApplication::applicationDirPath() + "/images/sport/";
    QStringList extensions = {"png", "jpg", "jpeg"};
    for (const QString& ext : extensions) {
        QString path = imageFolder + "/" + sportName + "." + ext;
        if (QFile::exists(path)) {
            QPixmap pixmap(path);
            if (!pixmap.isNull()) {
                QSize labelSize = imagePreviewLabel->size();
                QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                QPixmap fittedPixmap(labelSize);
                fittedPixmap.fill(Qt::transparent);

                QPainter painter(&fittedPixmap);
                painter.setRenderHint(QPainter::SmoothPixmapTransform);

                int x = (labelSize.width() - scaledPixmap.width()) / 2;
                int y = (labelSize.height() - scaledPixmap.height()) / 2;
                painter.drawPixmap(x, y, scaledPixmap);

                imagePreviewLabel->setPixmap(fittedPixmap);

                imagePathLabel->setText("Ảnh hiện tại: " + sportName + "." + ext);
                break;
            }
        }
    }
}

void AddSportWindow::onConfirmClicked() {
    if (imagePath.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn ảnh mới!");
        return;
    }

    QString imageFolder = QCoreApplication::applicationDirPath() + "/images/sport/";
    QString fileExtension = QFileInfo(imagePath).suffix().toLower();

    if (fileExtension != "png" && fileExtension != "jpg" && fileExtension != "jpeg") {
        QMessageBox::warning(this, "Lỗi", "Định dạng ảnh không hợp lệ!");
        return;
    }

    QString savedFileName = removeAccents(originalName).replace(" ", "_");
    savedFileName += "." + fileExtension;
    QString savedPath = imageFolder + "/" + savedFileName;

    QPixmap originalPixmap(imagePath);
    if (originalPixmap.isNull()) {
        QMessageBox::warning(this, "Lỗi", "Không thể đọc ảnh đã chọn!");
        return;
    }

    QSize targetSize(320, 150);

    // Scale giữ tỉ lệ
    QPixmap scaledPixmap = originalPixmap.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Tạo pixmap đúng size label, nền trong suốt
    QPixmap fittedPixmap(targetSize);
    fittedPixmap.fill(Qt::transparent);

    QPainter painter(&fittedPixmap);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Vẽ ảnh ở giữa
    int x = (targetSize.width() - scaledPixmap.width()) / 2;
    int y = (targetSize.height() - scaledPixmap.height()) / 2;
    painter.drawPixmap(x, y, scaledPixmap);

    // Lưu ảnh đã fit
    if (!fittedPixmap.save(savedPath)) {
        QMessageBox::warning(this, "Lỗi", "Không thể lưu ảnh!");
        return;
    }

    emit sportConfirmed(originalName);
    this->close();
}

QString AddSportWindow::removeAccents(const QString& input) {
    QString output = input;
    static const QMap<QChar, QChar> accentMap = {
        // a
        {L'á', L'a'}, {L'à', L'a'}, {L'ả', L'a'}, {L'ã', L'a'}, {L'ạ', L'a'},
        {L'ă', L'a'}, {L'ắ', L'a'}, {L'ằ', L'a'}, {L'ẳ', L'a'}, {L'ẵ', L'a'}, {L'ặ', L'a'},
        {L'â', L'a'}, {L'ấ', L'a'}, {L'ầ', L'a'}, {L'ẩ', L'a'}, {L'ẫ', L'a'}, {L'ậ', L'a'},
        // e
        {L'é', L'e'}, {L'è', L'e'}, {L'ẻ', L'e'}, {L'ẽ', L'e'}, {L'ẹ', L'e'},
        {L'ê', L'e'}, {L'ế', L'e'}, {L'ề', L'e'}, {L'ể', L'e'}, {L'ễ', L'e'}, {L'ệ', L'e'},
        // i
        {L'í', L'i'}, {L'ì', L'i'}, {L'ỉ', L'i'}, {L'ĩ', L'i'}, {L'ị', L'i'},
        // o
        {L'ó', L'o'}, {L'ò', L'o'}, {L'ỏ', L'o'}, {L'õ', L'o'}, {L'ọ', L'o'},
        {L'ô', L'o'}, {L'ố', L'o'}, {L'ồ', L'o'}, {L'ổ', L'o'}, {L'ỗ', L'o'}, {L'ộ', L'o'},
        {L'ơ', L'o'}, {L'ớ', L'o'}, {L'ờ', L'o'}, {L'ở', L'o'}, {L'ỡ', L'o'}, {L'ợ', L'o'},
        // u
        {L'ú', L'u'}, {L'ù', L'u'}, {L'ủ', L'u'}, {L'ũ', L'u'}, {L'ụ', L'u'},
        {L'ư', L'u'}, {L'ứ', L'u'}, {L'ừ', L'u'}, {L'ử', L'u'}, {L'ữ', L'u'}, {L'ự', L'u'},
        // y
        {L'ý', L'y'}, {L'ỳ', L'y'}, {L'ỷ', L'y'}, {L'ỹ', L'y'}, {L'ỵ', L'y'},
        // đ
        {L'đ', L'd'},

        // A
        {L'Á', L'A'}, {L'À', L'A'}, {L'Ả', L'A'}, {L'Ã', L'A'}, {L'Ạ', L'A'},
        {L'Ă', L'A'}, {L'Ắ', L'A'}, {L'Ằ', L'A'}, {L'Ẳ', L'A'}, {L'Ẵ', L'A'}, {L'Ặ', L'A'},
        {L'Â', L'A'}, {L'Ấ', L'A'}, {L'Ầ', L'A'}, {L'Ẩ', L'A'}, {L'Ẫ', L'A'}, {L'Ậ', L'A'},
        // E
        {L'É', L'E'}, {L'È', L'E'}, {L'Ẻ', L'E'}, {L'Ẽ', L'E'}, {L'Ẹ', L'E'},
        {L'Ê', L'E'}, {L'Ế', L'E'}, {L'Ề', L'E'}, {L'Ể', L'E'}, {L'Ễ', L'E'}, {L'Ệ', L'E'},
        // I
        {L'Í', L'I'}, {L'Ì', L'I'}, {L'Ỉ', L'I'}, {L'Ĩ', L'I'}, {L'Ị', L'I'},
        // O
        {L'Ó', L'O'}, {L'Ò', L'O'}, {L'Ỏ', L'O'}, {L'Õ', L'O'}, {L'Ọ', L'O'},
        {L'Ô', L'O'}, {L'Ố', L'O'}, {L'Ồ', L'O'}, {L'Ổ', L'O'}, {L'Ỗ', L'O'}, {L'Ộ', L'O'},
        {L'Ơ', L'O'}, {L'Ớ', L'O'}, {L'Ờ', L'O'}, {L'Ở', L'O'}, {L'Ỡ', L'O'}, {L'Ợ', L'O'},
        // U
        {L'Ú', L'U'}, {L'Ù', L'U'}, {L'Ủ', L'U'}, {L'Ũ', L'U'}, {L'Ụ', L'U'},
        {L'Ư', L'U'}, {L'Ứ', L'U'}, {L'Ừ', L'U'}, {L'Ử', L'U'}, {L'Ữ', L'U'}, {L'Ự', L'U'},
        // Y
        {L'Ý', L'Y'}, {L'Ỳ', L'Y'}, {L'Ỷ', L'Y'}, {L'Ỹ', L'Y'}, {L'Ỵ', L'Y'},
        // Đ
        {L'Đ', L'D'}
    };

    for (int i = 0; i < output.length(); ++i) {
        if (accentMap.contains(output[i])) {
            output[i] = accentMap[output[i]];
        }
    }

    return output;
}
