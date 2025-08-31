#include "FieldSelectionWindow.h"
#include "CalendarWindow.h"
#include "AddFieldWindow.h"
#include "PriceWindow.h"
#include "mainwindow.h"
#include "ManageSport.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QCoreApplication>
#include <QScrollArea>
#include <QDebug>
#include <QFrame>
#include <fstream>
#include <string>
#include <QTimer>
#include <QMessageBox>
#include <QScrollBar>
#include <QFile>
#include <QMenu>
#include <QStackedLayout>

using namespace std;

QString FieldSelectionWindow::removeAccents(const QString& input) {
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

FieldSelectionWindow::FieldSelectionWindow(const QString& sport, QWidget* parent)
    : QWidget(parent), selectedSport(sport) {
    setWindowTitle("Chọn sân");
    resize(660, 560);
    setFixedSize(660, 560);

    setStyleSheet(R"(
        /* === Menu === */
        QMenu {
            background-color: #2c3e50;
            border: 1px solid #4a637d;
            padding: 6px;
            color: #e0e0e0;
            font-size: 13px;
            font-weight: 500;
        }
        QMenu::item {
            padding: 6px 16px;
        }
        QMenu::item:selected {
            background-color: #0d6efd;
            color: #ffffff;
        }
        /* === Main Widget Background === */
        QWidget {
            background-color: #3e4a59;
            font-family: "Segoe UI", sans-serif;
            font-size: 14px;
        }

        /* === Labels === */
        QLabel {
            background-color: transparent;
            color: #bdc3c7;
            font-size: 13px;
            font-weight: 500;
        }

        /* === Title Label === */
        #titleLabel {
            font-size: 24px;
            font-weight: bold;
            color: #ffffff;
            margin-bottom: 10px;
        }

        /* === Button Base Style === */
        QPushButton {
            padding: 10px 20px;
            font-size: 14px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
        }

        /* === Primary Action Button (Add Field, Select) === */
        #addFieldButton, #selectButton {
            padding: 8px 15px;
            background-color: #0d6efd;
            color: white;
        }
        #addFieldButton:hover, #selectButton:hover {
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

        /* === Menu Button === */
        #menuButton {
            background-color: rgba(52, 73, 94, 180);
            color: #e0e0e0;
            border: none;
            border-radius: 4px;
        }
        #menuButton:hover {
            background-color: rgba(74, 99, 125, 200);
        }

        /* === Scroll Area === */
        QScrollArea {
            background-color: #2c3e50;
            border: 1px solid #4a637d;
            border-radius: 8px;
        }

        /* === ScrollBar === */
        QScrollBar:vertical {
            border: none;
            background: #2c3e50;
            width: 8px;
            margin: 4px 0;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #0d6efd;
            min-height: 24px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: #0b5ed7;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
            subcontrol-origin: margin;
        }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
        }
    )");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);

    QHBoxLayout* headerLayout = new QHBoxLayout;
    QLabel* titleLabel = new QLabel("Các sân hiện có");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setContentsMargins(20, 10, 0, 10);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    QPushButton* addFieldButton = new QPushButton("+ Thêm sân");
    addFieldButton->setObjectName("addFieldButton");
    addFieldButton->setFixedWidth(120);
    addFieldButton->setCursor(Qt::PointingHandCursor);
    headerLayout->addWidget(addFieldButton);
    headerLayout->addSpacing(25);

    mainLayout->addLayout(headerLayout);

    QWidget* scrollContent = new QWidget;
    scrollContent->setObjectName("scrollContent");
    gridLayout = new QGridLayout(scrollContent);
    gridLayout->setSpacing(18);
    gridLayout->setContentsMargins(10, 10, 20, 10);

    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollContent);
    scrollArea->setFrameShape(QFrame::NoFrame);

    mainLayout->addWidget(scrollArea);

    connect(addFieldButton, &QPushButton::clicked, this, [=]() {
        auto* window = new AddFieldWindow(selectedSport);
        window->setExistingFieldNames(existingFields);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->show();

        connect(window, &AddFieldWindow::fieldConfirmed, this, [=](const QString& name, int price) {
            addField(name, price);
        });

        connect(window, &AddFieldWindow::backToFieldList, this, [=]() {
            this->show();
        });
    });

    backButton = new QPushButton("← Quay lại");
    backButton->setObjectName("backButton");
    backButton->setCursor(Qt::PointingHandCursor);
    mainLayout->addWidget(backButton, 0, Qt::AlignLeft);

    connect(backButton, &QPushButton::clicked, this, [this]() {
        MainWindow* mainWin = qobject_cast<MainWindow*>(this->window());
        QWidget *sportPage = new ManageSport(mainWin);
        mainWin->setCentralWidgetContent(sportPage);
    });

    loadFieldsFromFile();
}

void FieldSelectionWindow::loadFieldsFromFile() {
    string filePath = QCoreApplication::applicationDirPath().toStdString() + "/data/" + removeAccents(selectedSport).replace(" ", "_").toStdString()  + ".txt";
    ifstream file(filePath);

    string line;
    while (getline(file, line)) {
        size_t dash = line.find('-');
        if (dash == string::npos) continue;

        QString name = QString::fromStdString(line.substr(0, dash));
        existingFields.append(name);
        int price = QString::fromStdString(line.substr(dash + 1)).toInt();

        int count = gridLayout->count();
        int row = count / 4;
        int col = count % 4;

        createFieldCard(name, price, row, col);
    }
    file.close();
}

void FieldSelectionWindow::addField(const QString& name, int price) {
    string filePath = QCoreApplication::applicationDirPath().toStdString() + "/data/" + removeAccents(selectedSport).replace(" ", "_").toStdString()  + ".txt";
    ofstream file(filePath, ios::app);
    if (file.is_open()) {
        file << name.toStdString() << "-" << price << '\n';
        file.close();
    } else {
        return;
    }

    int count = gridLayout->count();
    int row = count / 4;
    int col = count % 4;

    createFieldCard(name, price, row, col);

    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
}

void FieldSelectionWindow::createFieldCard(const QString& name, int price, int row, int col) {
    QWidget* card = new QWidget;
    card->setFixedSize(180, 200);
    card->setStyleSheet(R"(
        background-color: #2c3e50;
        border: 1px solid #4a637d;
        border-radius: 8px;
    )");

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);
    layout->setAlignment(Qt::AlignTop);

    QWidget* imageContainer = new QWidget;
    imageContainer->setFixedSize(160, 100);
    imageContainer->setStyleSheet("background-color: #3e4a59; border: 1px solid #4a637d; border-radius: 4px;");

    QStackedLayout* imageStack = new QStackedLayout(imageContainer);
    imageStack->setContentsMargins(0, 0, 0, 0);

    QLabel* imageLabel = new QLabel;
    imageLabel->setFixedSize(160, 100);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border-radius: 4px; color: #bdc3c7;");

    imageStack->addWidget(imageLabel);

    QPushButton* menuButton = new QPushButton("⋮", imageContainer);
    menuButton->setObjectName("menuButton");
    menuButton->setFixedSize(24, 24);
    menuButton->move(imageLabel->width() - 28, 4);
    menuButton->raise();
    menuButton->setCursor(Qt::PointingHandCursor);

    QMenu* menu = new QMenu(menuButton);
    menu->addAction("Sửa sân", [=]() {
        auto* editWindow = new AddFieldWindow(selectedSport);
        editWindow->setEditMode(name, price, existingFields);
        editWindow->setAttribute(Qt::WA_DeleteOnClose);
        editWindow->show();

        connect(editWindow, &AddFieldWindow::fieldConfirmed, this, [=](const QString& newName, int newPrice) {
            if (newName != name) existingFields.removeAll(name);
            existingFields.append(newName);
            updateField(name, newName, newPrice);
        });
    });

    menu->addAction("Tùy chỉnh giá", [=](){
        auto* priceWindow = new PriceWindow(selectedSport, name);
        priceWindow->setAttribute(Qt::WA_DeleteOnClose);
        priceWindow->show();
    });

    menu->addAction("Xóa sân", [=]() {
        int confirm = QMessageBox::question(this, "Xoá", "Bạn có chắc chắn muốn xoá sân này?");
        if (confirm == QMessageBox::Yes)
        {
            deleteField(name);
            QWidget* w = card;
            if (w) {
                gridLayout->removeWidget(w);
                w->deleteLater();
            }
        }
    });

    menuButton->setMenu(menu);

    QStringList extensions = {"png", "jpg", "jpeg"};
    QString imagePath;
    for (const QString& ext : extensions) {
        QString candidatePath = QCoreApplication::applicationDirPath() + "/images/field/" + selectedSport + '/' + name + "." + ext;
        if (QFile::exists(candidatePath)) {
            imagePath = candidatePath;
            break;
        }
    }

    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            QPixmap scaled = pixmap.scaled(imageLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            imageLabel->setPixmap(scaled);
        } else {
            imageLabel->setText("Ảnh lỗi");
            imageLabel->setStyleSheet("background-color: #4a637d; border-radius: 4px; color: #e0e0e0;");
        }
    } else {
        imageLabel->setText(name);
        imageLabel->setStyleSheet("background-color: #3e4a59; border-radius: 4px; font-size: 14px; color: #bdc3c7;");
    }

    layout->addWidget(imageContainer);

    QLabel* nameLabel = new QLabel(name);
    nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    nameLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: #e0e0e0;");
    nameLabel->setFixedHeight(28);

    QLabel* priceLabel = new QLabel(QString("%1đ").arg(price));
    priceLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    priceLabel->setStyleSheet("font-size: 13px; color: #bdc3c7;");
    priceLabel->setMinimumWidth(90);
    priceLabel->setFixedHeight(28);

    QPushButton* selectButton = new QPushButton("Chọn");
    selectButton->setObjectName("selectButton");
    selectButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    selectButton->setCursor(Qt::PointingHandCursor);
    selectButton->setStyleSheet(R"(
        QPushButton {
            padding: 8px 15px;
            background-color: #0d6efd;
            color: white;
            border-radius: 6px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #0b5ed7;
        }
        QPushButton:pressed {
            background-color: #094db1;
        }
    )");

    QHBoxLayout* priceLayout = new QHBoxLayout;
    priceLayout->addWidget(priceLabel);
    priceLayout->addStretch();
    priceLayout->addWidget(selectButton);

    layout->addWidget(nameLabel);
    layout->addLayout(priceLayout);

    gridLayout->addWidget(card, row, col);

    connect(selectButton, &QPushButton::clicked, this, [=]() {
        calendarWindow = new CalendarWindow(selectedSport, name);
        calendarWindow->setAttribute(Qt::WA_DeleteOnClose);
        calendarWindow->show();
    });
}

void FieldSelectionWindow::updateField(const QString& oldName, const QString& newName, int newPrice) {
    QString path = QCoreApplication::applicationDirPath() + "/data/" + (removeAccents(selectedSport).replace(" ", "_")) + ".txt";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QStringList lines;
    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        if (line.startsWith(oldName + "-")) {
            lines.append(newName + "-" + QString::number(newPrice));
        } else {
            lines.append(line);
        }
    }
    file.close();

    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&file);
        for (const QString& line : lines) {
            out << line << "\n";
        }
        file.close();
    }

    renameFieldInBookingFile(oldName, newName);
    renameFieldImage(oldName, newName);
    reloadFields();
}

void FieldSelectionWindow::reloadFields() {
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }
    existingFields.clear();
    loadFieldsFromFile();
}

void FieldSelectionWindow::deleteField(const QString& name) {
    existingFields.removeAll(name);
    QString path = QCoreApplication::applicationDirPath() + "/data/" + removeAccents(selectedSport).replace(" ", "_")  + ".txt";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QStringList lines;
    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        if (!line.startsWith(name + "-")) {
            lines.append(line);
        }
    }
    file.close();

    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&file);
        for (const QString& line : lines) {
            out << line << "\n";
        }
        file.close();
    }

    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
    removeFieldFromBookingFile(name);
    deleteFieldImage(name);
    loadFieldsFromFile();
}

void FieldSelectionWindow::removeFieldFromBookingFile(const QString& fieldName) {
    QString filePath = QCoreApplication::applicationDirPath() + "/data/booking.txt";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QStringList keptLines;
    while (!file.atEnd()) {
        QString line = file.readLine();
        if (!line.startsWith(fieldName + "-")) {
            keptLines.append(line);
        }
    }
    file.close();

    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&file);
        for (const QString& line : keptLines) {
            out << line;
        }
        file.close();
    }
}

void FieldSelectionWindow::renameFieldInBookingFile(const QString& oldName, const QString& newName) {
    QString filePath = QCoreApplication::applicationDirPath() + "/data/booking.txt";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QStringList updatedLines;
    while (!file.atEnd()) {
        QString line = file.readLine();
        if (line.startsWith(oldName + "-")) {
            line.replace(0, oldName.length(), newName);
        }
        updatedLines.append(line);
    }
    file.close();

    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&file);
        for (const QString& line : updatedLines) {
            out << line;
        }
        file.close();
    }
}

void FieldSelectionWindow::deleteFieldImage(const QString& fieldName) {
    QString basePath = QCoreApplication::applicationDirPath() + "/images/field/" + selectedSport + '/';
    QStringList extensions = {"png", "jpg", "jpeg"};

    for (const QString& ext : extensions) {
        QString imagePath = basePath + fieldName + "." + ext;
        if (QFile::exists(imagePath)) {
            QFile::remove(imagePath);
            break;
        }
    }
}

void FieldSelectionWindow::renameFieldImage(const QString& oldName, const QString& newName) {
    QString basePath = QCoreApplication::applicationDirPath() + "/images/field/" + selectedSport + '/';
    QStringList extensions = {"png", "jpg", "jpeg"};

    for (const QString& ext : extensions) {
        QString oldPath = basePath + oldName + "." + ext;
        QString newPath = basePath + newName + "." + ext;

        if (QFile::exists(oldPath)) {
            QFile::rename(oldPath, newPath);
            break;
        }
    }
}
