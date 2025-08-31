// Quan_Ly_Khach_Hang.cpp
#include "Quan_Ly_Khach_Hang.h"

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QPair>
#include <QMessageBox>
#include <algorithm>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QDate>
#include <QStatusBar>
#include <QRegularExpression>

// --- Helper parsing functions (robust parsing of a line) ---
static QDate tryParseDate(const QString &s) {
    QString t = s.trimmed();
    t.replace('-', '/'); // unify separators
    static const QStringList formats = { "dd/MM/yyyy", "d/M/yyyy", "dd/M/yyyy", "d/MM/yyyy" };
    for (const QString &fmt : formats) {
        QDate d = QDate::fromString(t, fmt);
        if (d.isValid()) return d;
    }
    return QDate();
}

// Parse a booking line and extract fields in a robust way.
// Returns true if we found at least a phone (used as key) or otherwise falls back to old indices.
static bool parseBookingLine(const QString &line,
                             QString &outPhone,
                             QString &outName,
                             QDate &outDate,
                             QString &outCourt,
                             QString &outTimeSlot)
{
    outPhone.clear();
    outName.clear();
    outCourt.clear();
    outTimeSlot.clear();
    outDate = QDate();

    // Bỏ phần dịch vụ (nếu có)
    QString bookingPart = line.split("|").first().trimmed();

    // Tách theo "-"
    QStringList parts = bookingPart.split('-');
    if (parts.size() < 7) {
        return false; // không đủ dữ liệu
    }

    QString sport    = parts[0].trimmed();
    QString court    = parts[1].trimmed();
    QString dateStr  = parts[2].trimmed();
    QString timeFrom = parts[3].trimmed();
    QString timeTo   = parts[4].trimmed();
    QString name     = parts[5].trimmed();
    QString phone    = parts[6].trimmed();

    // Parse ngày
    QDate date = QDate::fromString(dateStr, "dd/MM/yyyy");
    if (!date.isValid()) {
        date = QDate::fromString(dateStr, "dd-MM-yyyy");
    }

    outPhone    = phone;
    outName     = name;
    outDate     = date;
    outCourt    = sport + " - " + court;
    outTimeSlot = timeFrom + "-" + timeTo;

    return true;
}


// -----------------------------------------------------------------

Quan_Ly_Khach_Hang::Quan_Ly_Khach_Hang(QWidget *parent)
    : QMainWindow(parent)
{
    // Khởi tạo các widget trước khi sử dụng
    tableWidget = new QTableWidget(this);
    m_model     = new QStandardItemModel(this);
    detailWidget = new QWidget(this);
    historyTable = new QTableWidget(this);
    lineSearch   = new QLineEdit(this);
    btnSearch    = new QPushButton("Tìm kiếm", this);
    btnBack      = new QPushButton("Quay lại", this);
    titleLabel   = new QLabel("DANH SÁCH KHÁCH HÀNG", this);
    statusBar    = new QStatusBar(this);

    setupUI();
    applyDarkTheme();
    loadVIPData();
}

Quan_Ly_Khach_Hang::~Quan_Ly_Khach_Hang()
{
    delete m_model;
}

void Quan_Ly_Khach_Hang::applyDarkTheme()
{
    titleLabel->setObjectName("titleLabel");

    this->setStyleSheet(R"(
        /* === Nền chung cho cửa sổ và tất cả widget === */
        QMainWindow {
            background-color: #2c3e50;
            color: #bdc3c7;
            font-family: Segoe UI, Arial, sans-serif;
            font-size: 10pt;
        }
        QWidget {
            background-color: transparent;
            color: #bdc3c7;
            font-family: Segoe UI, Arial, sans-serif;
            font-size: 10pt;
        }

        /* === Tiêu đề chính === */
        #titleLabel {
            font-size: 20pt;
            font-weight: bold;
            color: #ffffff;
            padding: 10px;
        }

        /* === Các tiêu đề phụ trong màn hình chi tiết === */
        QLabel[objectName^="subTitleLabel"] {
            font-size: 14pt;
            font-weight: bold;
            color: #ffffff;
        }

        /* === Ô nhập liệu === */
        QLineEdit {
            background-color: #3e4a59;
            border: 1px solid #4a637d;
            border-radius: 4px;
            padding: 8px 12px;
            color: #e0e0e0;
        }
        QLineEdit:focus {
            border-color: #0d6efd;
        }

        /* === Nút bấm === */
        QPushButton {
            background-color: #0d6efd;
            color: #ffffff;
            font-weight: bold;
            padding: 8px 16px;
            border-radius: 5px;
            border: none;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: #0b5ed7;
        }

        /* === Bảng === */
        QTableWidget {
            background-color: #34495e;
            border: 1px solid #4a637d;
            gridline-color: #4a637d;
            border-radius: 5px;
        }

        /* === Header của bảng === */
        QHeaderView::section {
            background-color: #0d6efd;
            color: white;
            padding: 8px;
            border: none;
            font-weight: bold;
        }

        /* === Các ô trong bảng === */
        QTableWidget::item {
            padding: 5px;
        }

        /* === Dòng được chọn trong bảng === */
        QTableWidget::item:selected {
            background-color: #0b5ed7;
            color: #ffffff;
        }

        /* === Thanh trạng thái === */
        QStatusBar {
            background-color: transparent;
        }
    )");
}

void Quan_Ly_Khach_Hang::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // Tiêu đề chính
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Bảng chính
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "STT" << "SĐT" << "Tên" << "Số lần đặt");
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    // Resize linh hoạt
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    mainLayout->addWidget(tableWidget);

    // Ô tìm kiếm
    QHBoxLayout *searchLayout = new QHBoxLayout();
    lineSearch->setPlaceholderText("Nhập SĐT tìm kiếm...");

    lineSearch->setMaximumWidth(420);
    btnSearch->setMaximumWidth(120);

    btnSearch->setCursor(Qt::PointingHandCursor);
    searchLayout->addWidget(lineSearch);
    searchLayout->addSpacing(8);
    searchLayout->addWidget(btnSearch);

    QWidget *searchRow = new QWidget(this);
    searchRow->setLayout(searchLayout);
    searchRow->setFixedWidth(540);
    mainLayout->addWidget(searchRow, 0, Qt::AlignHCenter);

    QPushButton *btnExportCSV = new QPushButton("Xuất CSV", this);
    btnExportCSV->setCursor(Qt::PointingHandCursor);
    btnExportCSV->setFixedWidth(120);

    // Layout chứa nút xuất CSV, canh trái
    QHBoxLayout *exportLayout = new QHBoxLayout();
    exportLayout->addWidget(btnExportCSV);
    exportLayout->addStretch(); // đẩy nút về bên trái

    QWidget *exportRow = new QWidget(this);
    exportRow->setLayout(exportLayout);
    mainLayout->addWidget(exportRow, 0, Qt::AlignLeft);

    // Kết nối sự kiện xuất CSV
    connect(btnExportCSV, &QPushButton::clicked, this, &Quan_Ly_Khach_Hang::exportCustomerListToCSV);

    // Widget chi tiết (được tạo nhưng ẩn đi)
    QVBoxLayout *detailLayout = new QVBoxLayout(detailWidget);
    detailLayout->setContentsMargins(15, 15, 15, 15);
    detailLayout->setSpacing(10);

    QLabel *customerInfoLabel = new QLabel("THÔNG TIN KHÁCH HÀNG", this);
    customerInfoLabel->setObjectName("subTitleLabel_1");
    customerInfoLabel->setAlignment(Qt::AlignCenter);
    detailLayout->addWidget(customerInfoLabel);

    QTableWidget *detailTable = new QTableWidget(1, 5, this);
    detailTable->setObjectName("detailTable");
    detailTable->setFixedHeight(70);
    detailTable->setHorizontalHeaderLabels(QStringList() << "STT" << "SĐT" << "Tên" << "Số lần đặt" << "Status");
    detailTable->verticalHeader()->setVisible(false);
    detailTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Resize linh hoạt
    detailTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    detailTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    detailTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    detailTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    detailTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    detailLayout->addWidget(detailTable);
    detailLayout->addSpacing(20);

    QLabel *historyLabel = new QLabel("LỊCH SỬ ĐẶT SÂN (Mới nhất → Cũ nhất)", this);
    historyLabel->setObjectName("subTitleLabel_2");
    historyLabel->setAlignment(Qt::AlignCenter);
    detailLayout->addWidget(historyLabel);

    historyTable->setColumnCount(3);
    historyTable->setHorizontalHeaderLabels(QStringList() << "Ngày" << "Sân" << "Khung giờ");
    historyTable->verticalHeader()->setVisible(false);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Resize linh hoạt
    historyTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    historyTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    historyTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    detailLayout->addWidget(historyTable);

    btnBack->setCursor(Qt::PointingHandCursor);
    QHBoxLayout* backButtonLayout = new QHBoxLayout;
    backButtonLayout->addStretch();
    backButtonLayout->addWidget(btnBack);
    backButtonLayout->addStretch();
    detailLayout->addLayout(backButtonLayout);

    mainLayout->addWidget(detailWidget);
    detailWidget->hide();

    // Kết nối sự kiện
    connect(btnSearch, &QPushButton::clicked, this, &Quan_Ly_Khach_Hang::onSearchCustomer);
    connect(lineSearch, &QLineEdit::returnPressed, this, &Quan_Ly_Khach_Hang::onSearchCustomer);
    connect(btnBack, &QPushButton::clicked, this, &Quan_Ly_Khach_Hang::onBackButtonClicked);

    setCentralWidget(centralWidget);
    setStatusBar(statusBar);
    resize(900, 700);
    setWindowTitle("Quản lý khách hàng VIP");
}

void Quan_Ly_Khach_Hang::loadVIPData()
{
    m_model->removeRows(0, m_model->rowCount());
    QFile inputFile(QCoreApplication::applicationDirPath() + "/data/data.txt");
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        showMessage("Không thể mở file dữ liệu!");
        return;
    }

    QTextStream in(&inputFile);
    QMap<QString, QPair<QString, int>> customerData;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;

        QString phone, name, court, timeSlot;
        QDate date;
        bool ok = parseBookingLine(line, phone, name, date, court, timeSlot);
        if (!ok) continue;

        customerData[phone].first = name;
        customerData[phone].second++;
    }
    inputFile.close();

    QList<QPair<QString, QPair<QString, int>>> customers;
    for (auto it = customerData.constBegin(); it != customerData.constEnd(); ++it) {
        customers.append(qMakePair(it.key(), it.value()));
    }

    // Sắp xếp: VIP trước, sau đó theo số lần đặt giảm dần
    std::sort(customers.begin(), customers.end(),
              [](const auto& a, const auto& b) {
                  bool aVip = a.second.second >= 5;
                  bool bVip = b.second.second >= 5;
                  if (aVip != bVip) return aVip;
                  return a.second.second > b.second.second;
              });

    displayVIPCustomers(customers);
}

void Quan_Ly_Khach_Hang::displayVIPCustomers(const QList<QPair<QString, QPair<QString, int>>>& customers)
{
    tableWidget->clearContents();
    tableWidget->setRowCount(customers.size());

    for (int i = 0; i < customers.size(); ++i) {
        const QString &phone = customers[i].first;
        const QString &name  = customers[i].second.first;
        int bookingCount     = customers[i].second.second;

        QTableWidgetItem *sttItem   = new QTableWidgetItem(QString::number(i + 1));
        QTableWidgetItem *phoneItem = new QTableWidgetItem(phone);
        QTableWidgetItem *nameItem  = new QTableWidgetItem(name);
        QTableWidgetItem *countItem = new QTableWidgetItem(QString::number(bookingCount));

        sttItem->setTextAlignment(Qt::AlignCenter);
        countItem->setTextAlignment(Qt::AlignCenter);

        // Nếu là VIP => tô chữ vàng (gold)
        if (bookingCount >= 5) {
            QBrush vipBrush(QColor("#FFD700"));
            sttItem->setForeground(vipBrush);
            phoneItem->setForeground(vipBrush);
            nameItem->setForeground(vipBrush);
            countItem->setForeground(vipBrush);
        }

        tableWidget->setItem(i, 0, sttItem);
        tableWidget->setItem(i, 1, phoneItem);
        tableWidget->setItem(i, 2, nameItem);
        tableWidget->setItem(i, 3, countItem);
    }
}

void Quan_Ly_Khach_Hang::onBackButtonClicked()
{
    detailWidget->hide();

    // Hiện lại các widget của màn hình chính
    titleLabel->show();
    tableWidget->show();
    lineSearch->show();
    btnSearch->show();
}

void Quan_Ly_Khach_Hang::onSearchCustomer()
{
    const QString searchPhoneRaw = lineSearch->text().trimmed();
    if (searchPhoneRaw.isEmpty()) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng nhập SĐT tìm kiếm!");
        return;
    }
    // normalize search phone to digits-only for comparison
    QString searchPhone = searchPhoneRaw;
    searchPhone.remove(QRegularExpression("\\D"));

    QFile inputFile(QCoreApplication::applicationDirPath() + "/data/data.txt");
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Lỗi", "Không thể mở file dữ liệu!");
        return;
    }

    QMap<QString, QPair<QString, int>> customerData;
    QMultiMap<QDate, QStringList> bookingHistory;

    QTextStream in(&inputFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;

        QString phone, name, court, timeSlot;
        QDate date;
        bool ok = parseBookingLine(line, phone, name, date, court, timeSlot);
        if (!ok) continue;

        // count total bookings
        customerData[phone].first = name;
        customerData[phone].second++;

        // if this line belongs to the searched phone, add to history
        if (!phone.isEmpty() && phone == searchPhone) {
            if (date.isValid()) {
                bookingHistory.insert(date, QStringList() << date.toString("dd/MM/yyyy") << court << timeSlot);
            } else {
                bookingHistory.insert(QDate(), QStringList() << QString() << court << timeSlot);
            }
        }
    }
    inputFile.close();

    // sort customers by booking count desc
    QList<QPair<QString, QPair<QString, int>>> sortedCustomers;
    for (auto it = customerData.constBegin(); it != customerData.constEnd(); ++it) {
        sortedCustomers.append(qMakePair(it.key(), it.value()));
    }
    std::sort(sortedCustomers.begin(), sortedCustomers.end(),
              [](const auto& a, const auto& b) {
                  return a.second.second > b.second.second;
              });

    // find the searched phone rank + info
    bool found = false;
    int rank = 0;
    QString customerName;
    int bookingCount = 0;
    for (int i = 0; i < sortedCustomers.size(); ++i) {
        if (sortedCustomers[i].first == searchPhone) {
            rank = i + 1;
            customerName = sortedCustomers[i].second.first;
            bookingCount = sortedCustomers[i].second.second;
            found = true;
            break;
        }
    }

    if (found) {
        // hide main list widgets
        titleLabel->hide();
        tableWidget->hide();
        lineSearch->hide();
        btnSearch->hide();

        // show detail widget
        detailWidget->show();

        const bool isVIP = (bookingCount >= 5);
        QTableWidget* detailTable = detailWidget->findChild<QTableWidget*>("detailTable");
        detailTable->setItem(0, 0, new QTableWidgetItem(QString::number(rank)));
        detailTable->setItem(0, 1, new QTableWidgetItem(searchPhoneRaw));
        detailTable->setItem(0, 2, new QTableWidgetItem(customerName));
        detailTable->setItem(0, 3, new QTableWidgetItem(QString::number(bookingCount)));
        detailTable->setItem(0, 4, new QTableWidgetItem(isVIP ? "VIP" : "Thường"));

        for (int col = 0; col < 5; ++col) {
            if (auto *item = detailTable->item(0, col))
                item->setTextAlignment(Qt::AlignCenter);
        }
        if (auto *item = detailTable->item(0, 2))
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        if (auto *statusItem = detailTable->item(0, 4); statusItem && isVIP) {
            statusItem->setForeground(QBrush(QColor("#FFD700")));
        }

        // show booking history newest -> oldest
        historyTable->setRowCount(0);
        QList<QDate> dates = bookingHistory.keys();
        std::sort(dates.begin(), dates.end(), std::greater<QDate>());

        int row = 0;
        for (const QDate &date : dates) {
            for (const QStringList &booking : bookingHistory.values(date)) {
                historyTable->insertRow(row);
                historyTable->setItem(row, 0, new QTableWidgetItem(booking[0]));
                historyTable->setItem(row, 1, new QTableWidgetItem(booking[1]));
                historyTable->setItem(row, 2, new QTableWidgetItem(booking[2]));

                for (int col = 0; col < 3; ++col) {
                    if (auto *it = historyTable->item(row, col))
                        it->setTextAlignment(Qt::AlignCenter);
                }
                ++row;
            }
        }
    } else {
        QMessageBox::information(this, "Thông báo",
                                 "Không tìm thấy khách hàng với SĐT: " + searchPhoneRaw);
    }
}

void Quan_Ly_Khach_Hang::showMessage(const QString &message)
{
    statusBar->showMessage(message, 5000);
}

void Quan_Ly_Khach_Hang::exportCustomerListToCSV()
{
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("Xuất danh sách khách hàng"), "", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Lỗi"), tr("Không thể mở file để ghi."));
        return;
    }

    file.write("\xEF\xBB\xBF"); // BOM UTF-8
    QTextStream out(&file);

    // Ghi header CSV
    out << "STT,SĐT,Tên,Số lần đặt\n";

    QFile inputFile(QCoreApplication::applicationDirPath() + "/data/data.txt");
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Lỗi"), tr("Không thể mở file dữ liệu!"));
        return;
    }

    QTextStream in(&inputFile);
    QMap<QString, QPair<QString, int>> customerData; // SĐT -> (Tên, số lần đặt)

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QString phone, name, court, timeSlot;
        QDate date;
        if (!parseBookingLine(line, phone, name, date, court, timeSlot)) continue;

        // Thống kê số lần đặt
        if (customerData.contains(phone)) {
            customerData[phone].second += 1;
        } else {
            customerData[phone] = qMakePair(name, 1);
        }
    }
    inputFile.close();

    // Ghi dữ liệu ra CSV
    int stt = 1;
    for (auto it = customerData.constBegin(); it != customerData.constEnd(); ++it) {
        QString phone = it.key();
        QString name  = it.value().first;
        int count     = it.value().second;

        out << stt++ << "," << phone << "," << name << "," << count << "\n";
    }

    file.close();
    QMessageBox::information(this, tr("Hoàn tất"), tr("Xuất danh sách khách hàng thành công."));
}

