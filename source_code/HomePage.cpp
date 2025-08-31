#include "HomePage.h"
#include "AuthManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDate>
#include <QTime>
#include <QBrush>
#include <QPushButton>
#include <QMessageBox>
#include <QDialog>
#include <QtUiTools/QUiLoader>
#include <QTextEdit>
#include <QDir>
#include <QDebug>
#include <QPixmap>
#include <QFrame>

HomePage::HomePage(QWidget* parent)
    : QWidget(parent)
{
    this->setObjectName("HomePage");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 15, 30, 15);
    mainLayout->setSpacing(15);

    // --- Header Bar ---
    QFrame* headerBar = new QFrame(this);
    headerBar->setObjectName("HeaderBar");
    headerBar->setFixedHeight(60);

    QHBoxLayout* headerLayout = new QHBoxLayout(headerBar);
    headerLayout->setContentsMargins(10, 5, 10, 5);
    headerLayout->setSpacing(15);

    // Logo
    QLabel* logoLabel = new QLabel(this);
    logoLabel->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/images/logo.jpg").scaled(80, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setFixedSize(80, 50);
    headerLayout->addWidget(logoLabel);

    // Tiêu đề
    QLabel* titleLabel = new QLabel("Quản lý Sân Bóng", this);
    titleLabel->setObjectName("HeaderTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(titleLabel, 1);

    // Nút Nội Quy
    QPushButton* btnNoiQuy = new QPushButton("Nội Quy", this);
    btnNoiQuy->setObjectName("BtnNoiQuy");
    btnNoiQuy->setFixedSize(100, 35);
    headerLayout->addWidget(btnNoiQuy);

    mainLayout->addWidget(headerBar);

    // --- Info Section ---
    QHBoxLayout* infoLayout = new QHBoxLayout();

    // Left: App info
    QVBoxLayout* leftInfoLayout = new QVBoxLayout();
    QLabel* appTitle = new QLabel("Stadium Manager", this);
    appTitle->setObjectName("AppTitle");
    leftInfoLayout->addWidget(appTitle);

    QLabel* versionLabel = new QLabel("Version 1.0", this);
    versionLabel->setObjectName("AppVersion");
    leftInfoLayout->addWidget(versionLabel);

    QLabel* descLabel = new QLabel(
        "Stadium Manager là một ứng dụng giúp quản lý sân vận động bao gồm các sân: sân bóng đá, sân bóng chuyền, sân bóng rổ,...",
        this
        );
    descLabel->setWordWrap(true);
    descLabel->setObjectName("AppDescription");
    leftInfoLayout->addWidget(descLabel);

    QLabel* githubLink = new QLabel(
        "<style>a { color:#ffa657; text-decoration:none; } a:hover { color:#58a6ff; }</style>"
        "<a href='https://github.com/NguyenToan-dev/Manager-Stadium-OOP'>GitHub</a>", this
    );
    githubLink->setOpenExternalLinks(true);
    githubLink->setObjectName("GitHubLink");
    leftInfoLayout->addWidget(githubLink);

    leftInfoLayout->addStretch(1);

    // Right: Tabs
    QTabWidget* tabs = new QTabWidget(this);

    QWidget* tabNotes = new QWidget(this);
    QVBoxLayout* tabNotesLayout = new QVBoxLayout(tabNotes);
    QTextEdit* notes = new QTextEdit(tabNotes);
    notes->setReadOnly(true);
    notes->setHtml("<b>- Update 1.0:</b><br>+ Đã thêm Home Page");
    tabNotesLayout->addWidget(notes);
    tabs->addTab(tabNotes, "What's new");

    QWidget* tabCredits = new QWidget(this);
    QVBoxLayout* tabCreditsLayout = new QVBoxLayout(tabCredits);
    QTextEdit* credits = new QTextEdit(tabCredits);
    credits->setReadOnly(true);
    tabCreditsLayout->addWidget(credits);
    tabs->addTab(tabCredits, "Credits");

    infoLayout->addLayout(leftInfoLayout, 1);
    infoLayout->addWidget(tabs, 2);

    QWidget* infoWidget = new QWidget(this);
    infoWidget->setLayout(infoLayout);
    infoWidget->setMaximumHeight(150);
    mainLayout->addWidget(infoWidget);

    // --- Date Label ---
    QLabel* dateLabel = new QLabel("Lịch đặt sân ngày " + QDate::currentDate().toString("dd/MM/yyyy"), this);
    dateLabel->setObjectName("DateLabel");
    dateLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(dateLabel);

    // --- Search Bar ---
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel* searchLabel = new QLabel("Tìm kiếm:");
    QLineEdit* searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Tìm kiếm theo thông tin sân đã đặt...");
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchInput);
    mainLayout->addLayout(searchLayout);

    // --- Table ---
    table = new QTableWidget(this);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({
        "Môn", "Sân", "Giờ bắt đầu", "Giờ kết thúc", "Người đặt", "SĐT", "Giá"
    });
    table->setSortingEnabled(true);
    table->verticalHeader()->setVisible(false);

    QHeaderView* header = table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    // --- Stylesheet (all in one place) ---
    this->setStyleSheet(R"(
        #HomePage {
            background-color: #3e4a59;
            font-family: "Segoe UI", "Arial";
            color: #e0e0e0;
        }

        QLabel {
            font-size: 13px;
            color: #e0e0e0;
        }

        #HeaderBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0d6efd, stop:1 #4dabf7);
            border-radius: 6px;
        }
        QLabel#HeaderTitle {
            font-size: 20px;
            font-weight: bold;
            color: white;
        }
        QPushButton#BtnNoiQuy {
            background-color: #ffffff;
            color: #0d6efd;
            font-weight: bold;
            font-size: 14px;
            border-radius: 6px;
            border: 2px solid #0d6efd;
        }
        QPushButton#BtnNoiQuy:hover {
            background-color: #0d6efd;
            color: white;
        }

        QLabel#AppTitle {
            font-size: 20px;
            font-weight: bold;
            color: #ffffff;
        }
        QLabel#AppVersion {
            font-size: 12px;
            color: #bdc3c7;
        }
        QLabel#AppDescription {
            font-size: 12px;
            color: #bdc3c7;
        }
        QLabel#GitHubLink {
            color: #e6edf3;
            font-weight: bold;
        }
        QLabel#GitHubLink:hover {
            color: #58a6ff;
        }

        QLabel#DateLabel {
            font-size: 14px;
            font-weight: bold;
            color: #e0e0e0;
            padding: 5px;
        }

        QTabWidget::pane {
            border: 1px solid #4a637d;
            border-radius: 4px;
            background-color: #34495e;
        }
        QTabBar::tab {
            background: #2c3e50;
            border: 1px solid #4a637d;
            border-bottom: none;
            padding: 4px 12px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            color: #bdc3c7;
            font-size: 12px;
            min-height: 22px;
        }
        QTabBar::tab:hover {
            background: #4a637d;
        }
        QTabBar::tab:selected {
            background: #0d6efd;
            color: #ffffff;
            border-color: #0d6efd;
        }
        QTextEdit {
            background-color: #34495e;
            border: none;
            color: #ecf0f1;
            font-size: 12px;
        }

        QLineEdit {
            background-color: #2c3e50;
            color: #e0e0e0;
            border: 1px solid #4a637d;
            border-radius: 4px;
            padding: 6px;
            font-size: 13px;
        }
        QLineEdit:focus {
            border-color: #0d6efd;
        }

        QTableWidget {
            background-color: #2c3e50;
            border: 1px solid #4a637d;
            border-radius: 6px;
            gridline-color: #4a637d;
            font-size: 13px;
        }
        QHeaderView::section {
            background-color: #34495e;
            color: #ffffff;
            font-weight: bold;
            padding: 6px;
            border: none;
            border-bottom: 1px solid #4a637d;
            font-size: 13px;
        }
        QTableWidget::item {
            padding: 6px;
            border-bottom: 1px solid #4a637d;
        }
        QTableWidget::item:selected {
            background-color: #0d6efd !important;
            color: #ffffff !important;
        }
        QTableWidget:focus {
            outline: 0;
        }

        QScrollBar:vertical {
            border: none;
            background: #2c3e50;
            width: 10px;
        }
        QScrollBar::handle:vertical {
            background: #4a637d;
            min-height: 20px;
            border-radius: 5px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar:horizontal {
            border: none;
            background: #2c3e50;
            height: 10px;
        }
        QScrollBar::handle:horizontal {
            background: #4a637d;
            min-width: 20px;
            border-radius: 5px;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }
    )");

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setShowGrid(false);
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setAlternatingRowColors(false);

    mainLayout->addWidget(table, 1);

    // --- Search functionality ---
    connect(searchInput, &QLineEdit::textChanged, this, [this](const QString& text){
        for (int i = 0; i < table->rowCount(); ++i) {
            bool match = false;
            for (int j = 0; j < table->columnCount(); ++j) {
                QTableWidgetItem* item = table->item(i, j);
                if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                    match = true;
                    break;
                }
            }
            table->setRowHidden(i, !match);
        }
    });

    // --- "Nội Quy" Button Functionality ---
    connect(btnNoiQuy, &QPushButton::clicked, this, [=]() {
        QUiLoader loader;
        QString uiFilePath = QCoreApplication::applicationDirPath() + "/../../../NoiQuyDialog.ui";
        QFile file(uiFilePath);
        if (!file.open(QFile::ReadOnly)) {
            qDebug() << "Failed to open UI file from resource:" << uiFilePath;
            QMessageBox::warning(this, "Lỗi", "Không thể mở file NoiQuyDialog.ui. Vui lòng kiểm tra tài nguyên Qt.");
            return;
        }

        QDialog* dialog = qobject_cast<QDialog*>(loader.load(&file, this));
        file.close();

        if (!dialog) {
            qDebug() << "Failed to load dialog from UI file. Error:" << loader.errorString();
            QMessageBox::warning(this, "Lỗi", "Không thể load dialog từ file .ui");
            return;
        }

        dialog->setStyleSheet("QDialog { background-color: #3e4a59; border-radius: 12px; }");
        if (dialog->layout()) {
            dialog->layout()->setContentsMargins(20, 20, 20, 20);
            dialog->layout()->setSpacing(15);
        }

        QPushButton* btnSua = dialog->findChild<QPushButton*>("btnSua");
        QPushButton* btnDong = dialog->findChild<QPushButton*>("btnDong");
        QTextEdit* textEdit = dialog->findChild<QTextEdit*>("textEditNoiQuy");

        if (!btnSua || !btnDong || !textEdit) {
            qDebug() << "Failed to find required widgets in dialog";
            QMessageBox::warning(this, "Lỗi", "Không tìm thấy các widget cần thiết.");
            dialog->exec();
            return;
        }

        int role = AuthManager::instance().getCurrentUser().role;
        if (role != 0) {
            btnSua->setVisible(false);
            textEdit->setReadOnly(true);
        }

        textEdit->setStyleSheet(R"(
            QTextEdit {
                font-size: 16px;
                font-weight: bold;
                color: #e0e0e0;
                background-color: #2c3e50;
                border: 1px solid #4a637d;
                padding: 10px;
            }
        )");

        btnSua->setFixedSize(100, 40);
        btnSua->setStyleSheet(R"(
            QPushButton {
                background-color: #0d6efd;
                color: white;
                font-size: 14px;
                font-weight: bold;
                border-radius: 8px;
            }
            QPushButton:hover {
                background-color: #0b5ed7;
            }
        )");

        btnDong->setFixedSize(100, 40);
        btnDong->setStyleSheet(R"(
            QPushButton {
                background-color: #6c757d;
                color: white;
                font-size: 14px;
                border-radius: 8px;
            }
            QPushButton:hover {
                background-color: #5c636a;
            }
        )");

        QString dataPath = QCoreApplication::applicationDirPath() + "/data/noi_quy.txt";
        QFile noiQuyFile(dataPath);
        if (noiQuyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&noiQuyFile);
            textEdit->setPlainText(in.readAll());
            noiQuyFile.close();
        } else {
            qDebug() << "Failed to open noi_quy.txt at:" << dataPath;
            textEdit->setPlainText("Không tìm thấy nội quy.");
        }

        connect(btnDong, &QPushButton::clicked, dialog, &QDialog::accept);

        connect(btnSua, &QPushButton::clicked, [=]() mutable {
            if (btnSua->text() == "Sửa") {
                textEdit->setReadOnly(false);
                btnSua->setText("Lưu");
                btnSua->setStyleSheet(R"(
                    QPushButton {
                        background-color: #28a745;
                        color: white;
                        font-size: 14px;
                        font-weight: bold;
                        border-radius: 8px;
                    }
                    QPushButton:hover {
                        background-color: #218838;
                    }
                )");
            } else {
                QString noiQuyMoi = textEdit->toPlainText();
                QFile outFile(dataPath);
                if (outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
                    QTextStream out(&outFile);
                    out << noiQuyMoi;
                    outFile.close();
                    QMessageBox msgBox(dialog);
                    msgBox.setWindowTitle("Đã lưu");
                    msgBox.setText("Nội dung đã được lưu thành công.");
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    QPixmap iconPixmap = dialog->style()->standardPixmap(QStyle::SP_MessageBoxInformation).scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    msgBox.setIconPixmap(iconPixmap);
                    msgBox.setStyleSheet(R"(
                        QMessageBox QLabel {
                            color: #e0e0e0;
                            font-size: 14px;
                        }
                        QMessageBox QPushButton {
                            min-width: 80px;
                            padding: 6px;
                            background-color: #0d6efd;
                            color: white;
                            border-radius: 6px;
                        }
                        QMessageBox QPushButton:hover {
                            background-color: #0b5ed7;
                        }
                    )");
                    msgBox.exec();
                } else {
                    qDebug() << "Failed to write to noi_quy.txt at:" << dataPath;
                    QMessageBox::warning(dialog, "Lỗi", "Không thể ghi vào file noi_quy.txt");
                }
                textEdit->setReadOnly(true);
                btnSua->setText("Sửa");
                btnSua->setStyleSheet(R"(
                    QPushButton {
                        background-color: #0d6efd;
                        color: white;
                        font-size: 14px;
                        font-weight: bold;
                        border-radius: 8px;
                    }
                    QPushButton:hover {
                        background-color: #0b5ed7;
                    }
                )");
            }
        });

        dialog->exec();
    });

    loadBookings();
}

void HomePage::loadBookings()
{
    table->setRowCount(0);

    QDate currentDate = QDate::currentDate();
    QString currentDateStr = currentDate.toString("dd/MM/yyyy");

    QList<QPair<QStringList, QString>> bookings;

    // Đọc từ data.txt trước
    QString dataFilePath = QCoreApplication::applicationDirPath() + "/data/data.txt";
    readBookingsFromFile(dataFilePath, currentDateStr, bookings);

    // Đọc từ booking.txt sau
    QString bookingFilePath = QCoreApplication::applicationDirPath() + "/data/booking.txt";
    readBookingsFromFile(bookingFilePath, currentDateStr, bookings);

    // Sắp xếp và hiển thị
    sortAndDisplayBookings(bookings);
}

void HomePage::readBookingsFromFile(const QString &filePath, const QString &currentDateStr,
                                    QList<QPair<QStringList, QString>> &bookings)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open:" << filePath;
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split("-");
        if (parts.size() != 8) continue;

        // Lọc theo ngày hiện tại
        if (parts[2] == currentDateStr) {
            QStringList adjustedParts = parts;
            adjustedParts.removeAt(2); // bỏ cột "Ngày"
            bookings.append(qMakePair(adjustedParts, line));
        }
    }

    file.close();
}


void HomePage::sortAndDisplayBookings(const QList<QPair<QStringList, QString>>& bookings)
{
    table->setRowCount(0);
    QTime currentTime = QTime::currentTime();

    // Phân loại và sắp xếp
    QList<QPair<QStringList, QString>> activeBookings;  // Đang diễn ra
    QList<QPair<QStringList, QString>> futureBookings; // Chưa tới
    QList<QPair<QStringList, QString>> pastBookings;   // Đã qua

    for (const auto& booking : bookings) {
        QString startTimeStr = booking.first[2]; // Giờ bắt đầu (chỉ số 2 thay vì 3 do đã bỏ "Ngày")
        QString endTimeStr = booking.first[3];   // Giờ kết thúc (chỉ số 3 thay vì 4)
        QTime startTime = QTime::fromString(startTimeStr, "HH:mm");
        QTime endTime = QTime::fromString(endTimeStr, "HH:mm");

        if (startTime.isValid() && endTime.isValid()) {
            if (currentTime >= startTime && currentTime <= endTime) {
                activeBookings.append(booking); // Đang diễn ra
            } else if (currentTime < startTime) {
                futureBookings.append(booking); // Chưa tới
            } else {
                pastBookings.append(booking);   // Đã qua
            }
        } else {
            qDebug() << "Invalid time format in booking:" << booking.first[2] << "-" << booking.first[3];
        }
    }

    // Sắp xếp theo giờ bắt đầu
    auto compareByStartTime = [](const QPair<QStringList, QString>& a, const QPair<QStringList, QString>& b) {
        QTime timeA = QTime::fromString(a.first[2], "HH:mm"); // Chỉ số 2 thay vì 3
        QTime timeB = QTime::fromString(b.first[2], "HH:mm");
        return timeA < timeB;
    };
    std::sort(activeBookings.begin(), activeBookings.end(), compareByStartTime);
    std::sort(futureBookings.begin(), futureBookings.end(), compareByStartTime);
    std::sort(pastBookings.begin(), pastBookings.end(), compareByStartTime);

    // Hiển thị các đặt sân: active -> future -> past
    int row = 0;

    // Đang diễn ra (highlight bằng màu chữ xanh lá)
    for (const auto& booking : activeBookings) {
        table->insertRow(row);
        for (int i = 0; i < 7; ++i) {
            QTableWidgetItem* item = new QTableWidgetItem(booking.first[i]);
            item->setTextAlignment(Qt::AlignCenter);
            item->setData(Qt::ForegroundRole, QColor(0, 255, 0)); // Highlight bằng xanh lá
            table->setItem(row, i, item);
        }
        ++row;
    }

    // Chưa tới (bình thường)
    for (const auto& booking : futureBookings) {
        table->insertRow(row);
        for (int i = 0; i < 7; ++i) {
            QTableWidgetItem* item = new QTableWidgetItem(booking.first[i]);
            item->setTextAlignment(Qt::AlignCenter);
            item->setData(Qt::ForegroundRole, QColor(255, 255, 255)); // Màu chữ trắng
            table->setItem(row, i, item);
        }
        ++row;
    }

    // Đã qua (làm mờ)
    for (const auto& booking : pastBookings) {
        table->insertRow(row);
        for (int i = 0; i < 7; ++i) {
            QTableWidgetItem* item = new QTableWidgetItem(booking.first[i]);
            item->setTextAlignment(Qt::AlignCenter);
            item->setData(Qt::ForegroundRole, QColor(100, 100, 100)); // Làm mờ bằng màu xám
            table->setItem(row, i, item);
        }
        ++row;
    }

    table->resizeColumnsToContents();
}
