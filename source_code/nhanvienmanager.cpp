// nhanvienmanager.cpp
#include "NhanVienManager.h"
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDoubleValidator>
#include <QLocale>
#include <algorithm>  // For std::sort

void NhanVienManager::applyDarkTheme() {
    m_luongThucNhanLabel->setObjectName("luongThucNhanLabel");
    m_resetNghiBtn->setObjectName("resetNghiBtn");

    this->setStyleSheet(R"(
        /* === Nền chung === */
        QMainWindow{
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

        /* === Bảng === */
        QTableWidget {
            background-color: #34495e;
            border: 1px solid #4a637d;
            gridline-color: #4a637d;
        }
        QHeaderView::section {
            background-color: #0d6efd;
            color: white;
            padding: 8px;
            border: none;
            font-weight: bold;
        }
        QTableWidget::item {
            color: #ecf0f1;
            padding: 5px;
        }
        QTableWidget::item:selected {
            background-color: #0b5ed7;
            color: #ffffff;
        }

        /* === Group Box Panel === */
        QGroupBox {
           background-color: #34495e;
           border: 1px solid #4a637d;
           border-radius: 5px;
           margin-top: 1ex;
           font-size: 11pt;
           color: #ffffff;
        }
        QGroupBox::title {
           subcontrol-origin: margin;
           subcontrol-position: top center;
           padding: 0 5px;
           color: #ffffff;
        }

        /* === Labels & Text === */
        QLabel {
            color: #bdc3c7;
            background-color: transparent; /* Đảm bảo label trong suốt */
        }
        #luongThucNhanLabel {
            font-weight: bold;
            color: #2ecc71; /* Màu xanh lá cây sáng */
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

        /* === Nút bấm chung === */
        QPushButton {
            background-color: #0d6efd;
            color: #ffffff;
            font-weight: bold;
            padding: 8px 16px;
            border-radius: 5px;
            border: none;
        }
        QPushButton:hover {
            background-color: #0b5ed7;
        }

        /* === Nút đặc biệt === */
        #resetNghiBtn {
            background-color: #e67e22; /* Cam */
        }
        #resetNghiBtn:hover {
            background-color: #d35400;
        }
    )");
}

void NhanVienManager::onToggleInfoPanel() {
    m_panelVisible = !m_panelVisible;
    m_infoPanel->setVisible(m_panelVisible);

    if (m_panelVisible) {
        m_togglePanelBtn->setText("Ẩn chi tiết nhân viên");
        m_togglePanelBtn->setStyleSheet(
            "background-color: #e74c3c; hover: { background-color: #c0392b; }"
            );
    } else {
        m_togglePanelBtn->setText("Hiện chi tiết nhân viên");
        m_togglePanelBtn->setStyleSheet(
            "background-color: #27ae60; hover: { background-color: #229954; }"
            );
    }
}

NhanVienManager::NhanVienManager(QWidget *parent)
    : QMainWindow(parent), m_currentRow(-1), m_panelVisible(true) {
    setupUi();

    applyDarkTheme();
    connectSignals();
    loadData();
    refreshTable();
}

void NhanVienManager::setupUi() {
    // Widget chính
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // Layout chính
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // Search bar
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *searchLabel = new QLabel("Tìm kiếm:");
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Tìm theo ID hoặc SDT...");
    m_searchEdit->setMinimumWidth(300);
    m_searchBtn = new QPushButton("Tìm Kiếm");  // Nút tìm kiếm mới
    m_clearSearchBtn = new QPushButton("Trở Lại");
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_searchBtn);  // Thêm nút tìm kiếm bên trái nút Trở Lại
    searchLayout->addWidget(m_clearSearchBtn);
    searchLayout->addStretch();
    mainLayout->addLayout(searchLayout);

    // Bảng dữ liệu
    m_table = new QTableWidget(this);
    m_table->setColumnCount(9);
    m_table->setHorizontalHeaderLabels({"ID","Họ Tên","Giới Tính","Ngày Sinh",
                                        "Chức Vụ","Lương","Ngày Nghỉ ",
                                        "SDT","Lương Thực Nhận"});

    // Thiết lập resize policy
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed); // 🔹 Quan trọng
    m_table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Set fixed width cho từng cột
    m_table->setColumnWidth(0, 80);   // ID
    m_table->setColumnWidth(1, 180);  // Họ Tên
    m_table->setColumnWidth(2, 90);   // Giới Tính
    m_table->setColumnWidth(3, 130);  // Ngày Sinh
    m_table->setColumnWidth(4, 120);  // Chức Vụ
    m_table->setColumnWidth(5, 150);  // Lương
    m_table->setColumnWidth(6, 100);  // Ngày Nghỉ
    m_table->setColumnWidth(7, 120);  // SDT
    m_table->setColumnWidth(8, 180);  // Lương Thực Nhận

    mainLayout->addWidget(m_table, 7);

    // Panel thông tin
    m_infoPanel = new QGroupBox(); // Tiêu đề sẽ được set trong QSS

    QHBoxLayout *panelLayout = new QHBoxLayout(m_infoPanel);
    panelLayout->setContentsMargins(10, 10, 10, 10);

    // Form nhập liệu
    QWidget *formContainer = new QWidget();
    QHBoxLayout *formLayout = new QHBoxLayout(formContainer);
    formLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->setSpacing(20);

    // Form bên trái
    QWidget *leftForm = new QWidget();
    QFormLayout *leftLayout = new QFormLayout(leftForm);
    leftLayout->setSpacing(10);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    // Form bên phải
    QWidget *rightForm = new QWidget();
    QFormLayout *rightLayout = new QFormLayout(rightForm);
    rightLayout->setSpacing(10);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    // Khởi tạo các field nhập liệu
    m_idEdit = new QLineEdit();
    m_idEdit->setMaximumWidth(100);

    m_hoTenEdit = new QLineEdit();
    m_hoTenEdit->setMinimumWidth(250);

    m_gioiTinhEdit = new QLineEdit();
    m_gioiTinhEdit->setMaximumWidth(80);

    m_ngaySinhEdit = new QLineEdit();
    m_chucVuEdit = new QLineEdit();

    m_luongEdit = new QLineEdit();
    m_luongEdit->setMaximumWidth(150);
    m_luongEdit->setValidator(new QDoubleValidator(0, 10000000000, 0, this));

    m_soNgayNghiEdit = new QLineEdit();
    m_soNgayNghiEdit->setMaximumWidth(80);
    m_soNgayNghiEdit->setValidator(new QIntValidator(0, 31, this));

    m_sdtEdit = new QLineEdit();
    m_luongThucNhanLabel = new QLabel("0 VND");

    // Thêm các field vào form
    leftLayout->addRow("ID:", m_idEdit);
    leftLayout->addRow("Họ Tên:", m_hoTenEdit);
    leftLayout->addRow("Giới Tính:", m_gioiTinhEdit);

    rightLayout->addRow("Ngày Sinh:", m_ngaySinhEdit);
    rightLayout->addRow("Chức Vụ:", m_chucVuEdit);
    rightLayout->addRow("Lương (VND):", m_luongEdit);
    rightLayout->addRow("Số ngày nghỉ:", m_soNgayNghiEdit);
    rightLayout->addRow("Số ĐT:", m_sdtEdit);
    rightLayout->addRow("Lương thực:", m_luongThucNhanLabel);

    formLayout->addWidget(leftForm);
    formLayout->addWidget(rightForm);
    panelLayout->addWidget(formContainer);
    mainLayout->addWidget(m_infoPanel, 2);

    // Thanh công cụ với các nút chức năng
    QWidget *toolbar = new QWidget();
    QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(0, 0, 0, 0);
    toolbarLayout->setSpacing(15);

    // Tạo các nút chức năng
    m_addBtn = new QPushButton("Thêm");
    m_editBtn = new QPushButton("Sửa");
    m_deleteBtn = new QPushButton("Xóa");
    m_saveBtn = new QPushButton("Lưu");
    m_exportBtn = new QPushButton("Xuất Word");
    m_resetNghiBtn = new QPushButton("Reset ngày nghỉ");
    m_togglePanelBtn = new QPushButton("Ẩn chi tiết nhân viên");
    onToggleInfoPanel(); // Gọi để set màu ban đầu

    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_addBtn);
    toolbarLayout->addWidget(m_editBtn);
    toolbarLayout->addWidget(m_deleteBtn);
    toolbarLayout->addWidget(m_saveBtn);
    toolbarLayout->addWidget(m_exportBtn);
    toolbarLayout->addWidget(m_resetNghiBtn);
    toolbarLayout->addWidget(m_togglePanelBtn);
    toolbarLayout->addStretch();

    mainLayout->addWidget(toolbar, 0);
}

void NhanVienManager::connectSignals() {
    connect(m_addBtn, &QPushButton::clicked, this, &NhanVienManager::onAdd);
    connect(m_editBtn, &QPushButton::clicked, this, &NhanVienManager::onEdit);
    connect(m_deleteBtn, &QPushButton::clicked, this, &NhanVienManager::onDelete);
    connect(m_saveBtn, &QPushButton::clicked, this, &NhanVienManager::onSave);
    connect(m_exportBtn, &QPushButton::clicked, this, &NhanVienManager::onExportToWord);
    connect(m_resetNghiBtn, &QPushButton::clicked, this, &NhanVienManager::onResetNghi);
    connect(m_togglePanelBtn, &QPushButton::clicked, this, &NhanVienManager::onToggleInfoPanel);
    connect(m_table, &QTableWidget::cellClicked, this, &NhanVienManager::onRowSelected);
    connect(m_luongEdit, &QLineEdit::textChanged, this, &NhanVienManager::onLuongTextChanged);
    connect(m_soNgayNghiEdit, &QLineEdit::textChanged, this, &NhanVienManager::onSoNgayNghiChanged);

    // Connect mới cho tìm kiếm
    connect(m_searchBtn, &QPushButton::clicked, this, &NhanVienManager::onSearch);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &NhanVienManager::onSearch);  // Hỗ trợ Enter
    connect(m_clearSearchBtn, &QPushButton::clicked, this, &NhanVienManager::onClearSearch);
}

void NhanVienManager::loadData() {
    QFile file(QCoreApplication::applicationDirPath() + "/data/nhanvien.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(",");
        if (parts.size() >= 8) {
            NhanVien nv(parts[0].trimmed(),
                        parts[1].trimmed(),
                        parts[2].trimmed(),
                        parts[3].trimmed(),
                        parts[4].trimmed(),
                        parts[5].trimmed().toLongLong(),
                        parts[6].trimmed().toInt(),
                        parts[7].trimmed());
            m_list.append(nv);
        }
    }
    file.close();

    // Sort theo ID sau khi load
    std::sort(m_list.begin(), m_list.end(), [](const NhanVien &a, const NhanVien &b) {
        return a.id().toInt() < b.id().toInt();
    });
}

void NhanVienManager::refreshTable() {
    m_table->setRowCount(0);
    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);

    for (const NhanVien &nv : m_list) {
        addRowToTable(nv, locale);
    }

    clearInputs();
    m_currentRow = -1;
}

void NhanVienManager::addRowToTable(const NhanVien &nv, const QLocale &locale) {
    int row = m_table->rowCount();
    m_table->insertRow(row);

    m_table->setItem(row, 0, new QTableWidgetItem(nv.id()));
    m_table->setItem(row, 1, new QTableWidgetItem(nv.hoTen()));
    m_table->setItem(row, 2, new QTableWidgetItem(nv.gioiTinh()));
    m_table->setItem(row, 3, new QTableWidgetItem(nv.ngaySinh()));
    m_table->setItem(row, 4, new QTableWidgetItem(nv.chucVu()));
    m_table->setItem(row, 5, new QTableWidgetItem(locale.toString(nv.luong()) + " VND"));
    m_table->setItem(row, 6, new QTableWidgetItem(QString::number(nv.soNgayNghi())));
    m_table->setItem(row, 7, new QTableWidgetItem(nv.sdt()));
    m_table->setItem(row, 8, new QTableWidgetItem(locale.toString(nv.luongThucNhan()) + " VND"));
}

NhanVien NhanVienManager::currentInput() const {
    QString luongText = m_luongEdit->text().remove('.').remove(' ').remove("VND");
    return NhanVien(m_idEdit->text(),
                    m_hoTenEdit->text(),
                    m_gioiTinhEdit->text(),
                    m_ngaySinhEdit->text(),
                    m_chucVuEdit->text(),
                    luongText.toLongLong(),
                    m_soNgayNghiEdit->text().toInt(),
                    m_sdtEdit->text());
}

void NhanVienManager::populateInputs(const NhanVien &nv) {
    m_idEdit->setText(nv.id());
    m_hoTenEdit->setText(nv.hoTen());
    m_gioiTinhEdit->setText(nv.gioiTinh());
    m_ngaySinhEdit->setText(nv.ngaySinh());
    m_chucVuEdit->setText(nv.chucVu());
    m_luongEdit->setText(QString::number(nv.luong()));
    m_soNgayNghiEdit->setText(QString::number(nv.soNgayNghi()));
    m_sdtEdit->setText(nv.sdt());
    m_luongThucNhanLabel->setText(QString::number(nv.luongThucNhan()) + " VND");

    m_currentRow = m_list.indexOf(nv);
}

void NhanVienManager::clearInputs() {
    m_idEdit->clear();
    m_hoTenEdit->clear();
    m_gioiTinhEdit->clear();
    m_ngaySinhEdit->clear();
    m_chucVuEdit->clear();
    m_luongEdit->clear();
    m_soNgayNghiEdit->clear();
    m_sdtEdit->clear();
    m_luongThucNhanLabel->setText("0 VND");
    m_currentRow = -1;
}

void NhanVienManager::onAdd() {
    NhanVien nv = currentInput();
    if (nv.id().isEmpty() || nv.hoTen().isEmpty()) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng nhập ít nhất ID và Họ Tên!");
        return;
    }

    // Kiểm tra ID trùng
    for (const NhanVien &existing : m_list) {
        if (existing.id() == nv.id()) {
            QMessageBox::warning(this, "Cảnh báo", "ID đã tồn tại!");
            return;
        }
    }

    m_list.append(nv);

    // Sort lại sau khi thêm
    std::sort(m_list.begin(), m_list.end(), [](const NhanVien &a, const NhanVien &b) {
        return a.id().toInt() < b.id().toInt();
    });

    refreshTable();
    clearInputs();
}

void NhanVienManager::onEdit() {
    if (m_currentRow >= 0 && m_currentRow < m_list.size()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Xác nhận sửa", "Bạn có chắc muốn cập nhật thông tin nhân viên?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            m_list[m_currentRow] = currentInput();
            // Re-sort after edit (in case ID changed)
            std::sort(m_list.begin(), m_list.end(), [](const NhanVien &a, const NhanVien &b) {
                return a.id().toInt() < b.id().toInt();
            });
            refreshTable();
        }
    } else {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một nhân viên để sửa!");
    }
}

void NhanVienManager::onDelete() {
    if (m_currentRow >= 0 && m_currentRow < m_list.size()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Xác nhận xóa", "Bạn có chắc muốn xóa nhân viên này?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            m_list.removeAt(m_currentRow);
            refreshTable();
        }
    } else {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một nhân viên để xóa!");
    }
}

void NhanVienManager::onResetNghi() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận reset", "Bạn có chắc muốn reset số ngày nghỉ của TẤT CẢ nhân viên về 0?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        for (NhanVien &nv : m_list) {
            nv.setSoNgayNghi(0);
        }
        refreshTable();
        QMessageBox::information(this, "Thành công", "Đã reset số ngày nghỉ về 0 cho tất cả nhân viên");
    }
}

void NhanVienManager::onRowSelected(int row, int) {
    if (row >= 0 && row < m_table->rowCount()) {
        // Find the original index in m_list based on displayed row
        QString id = m_table->item(row, 0)->text();
        for (int i = 0; i < m_list.size(); ++i) {
            if (m_list[i].id() == id) {
                populateInputs(m_list[i]);
                break;
            }
        }
    }
}

void NhanVienManager::onLuongTextChanged(const QString &text) {
    Q_UNUSED(text);
    updateLuongThucNhan();
}

void NhanVienManager::onSoNgayNghiChanged(const QString &text) {
    Q_UNUSED(text);
    updateLuongThucNhan();
}

void NhanVienManager::updateLuongThucNhan() {
    QString luongText = m_luongEdit->text().remove('.').remove(' ').remove("VND");
    qlonglong luong = luongText.isEmpty() ? 0 : luongText.toLongLong();
    int soNgayNghi = m_soNgayNghiEdit->text().isEmpty() ? 0 : m_soNgayNghiEdit->text().toInt();

    qlonglong luongThucNhan = luong - (luong / 30 * soNgayNghi);
    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
    m_luongThucNhanLabel->setText(locale.toString(luongThucNhan) + " VND");
}

void NhanVienManager::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
}

void NhanVienManager::adjustColumnWidths() {
    if (m_table) {
        // Tính toán tổng chiều rộng khả dụng
        int tableWidth = m_table->viewport()->width();

        // Thiết lập chiều rộng cơ bản cho các cột
        int baseWidths[] = {
            80,   // ID (0) - Giữ nguyên
            140,  // Họ Tên (1) - Giữ nguyên
            100,  // Giới Tính (2) - Giữ nguyên
            130,  // Ngày Sinh (3) - Giữ nguyên
            60,   // Chức Vụ (4) - Giảm 1/2 (từ 120 xuống 60)
            130,  // Lương (5) - Tăng 30% (từ 100 lên 130)
            100,  // Số ngày nghỉ (6) - Giữ nguyên
            130,  // SDT (7) - Tăng 30% (từ 100 lên 130)
            130   // Lương thực nhận (8) - Bằng cột Lương (130)
        };

        // Tính tổng chiều rộng cơ bản
        int totalBaseWidth = 0;
        for (int w : baseWidths) totalBaseWidth += w;

        // Nếu bảng rộng hơn tổng cơ bản, phân phối thêm khoảng trống
        if (tableWidth > totalBaseWidth) {
            int extraWidth = tableWidth - totalBaseWidth;
            // Ưu tiên phân phối thêm cho các cột
            baseWidths[1] += extraWidth * 0.3;  // Họ Tên
            baseWidths[5] += extraWidth * 0.2;  // Lương
            baseWidths[7] += extraWidth * 0.2;  // SDT
            baseWidths[8] += extraWidth * 0.2;  // Lương thực nhận
            baseWidths[4] += extraWidth * 0.1;  // Chức Vụ
        }

        // Áp dụng chiều rộng cho các cột
        for (int i = 0; i < m_table->columnCount(); ++i) {
            m_table->setColumnWidth(i, baseWidths[i]);
        }

        // Thiết lập chế độ hiển thị khi text quá dài
        m_table->setTextElideMode(Qt::ElideRight);

        // Đảm bảo cột Lương và Lương thực nhận luôn bằng nhau
        m_table->setColumnWidth(8, m_table->columnWidth(5));
    }
}

QString NhanVienManager::toRtfString(const QString &text) const {
    QString result;
    for (const QChar &c : text) {
        ushort unicode = c.unicode();
        if (unicode < 128) {
            if (c == '\\' || c == '{' || c == '}') {
                result += '\\';
            }
            result += c;
        } else {
            result += "\\u" + QString::number(unicode) + "?";
        }
    }
    return result;
}

void NhanVienManager::exportListToWord(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Lỗi xuất file", "Không thể tạo file để xuất dữ liệu!");
        return;
    }

    QTextStream out(&file);
    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);

    // Header RTF với font Unicode
    out << "{\\rtf1\\ansi\\ansicpg1258\\deff0\\deflang1033\n"
        << "{\\fonttbl{\\f0\\fswiss\\fcharset0 Arial;}{\\f1\\fnil\\fcharset0 Times New Roman;}}\n"
        << "{\\info{\\title Danh sách nhân viên}}\n"
        << "\\viewkind4\\uc1\\pard\\sa200\\sl276\\slmult1\\f0\\fs24\\lang1066\n\n";

    // Tiêu đề bảng
    out << "\\qc{\\b\\f1 DANH SACH NHAN VIEN}\\par\\par\n";

    // Thiết lập bảng
    out << "\\trowd\\trgaph100\\trleft0\\trqc\n";

    // Độ rộng các cột (tính bằng twips)
    int widths[] = {1000, 2500, 800, 1300, 1400, 1500, 1000, 1500, 1500};
    int pos = 0;
    for (int w : widths) {
        pos += w;
        out << "\\clbrdrl\\brdrs\\clbrdrt\\brdrs\\clbrdrb\\brdrs\\clbrdrr\\brdrs\\cellx" << pos;
    }
    out << "\n";

    // Header bảng (sử dụng tiếng Việt trực tiếp)
    QStringList headers = {"ID", "Họ và tên", "Giới tính", "Ngày sinh",
                           "Chức vụ", "Lương",
                           "Số ngày nghỉ", "Số ĐT", "Lương thực nhận"};

    out << "\\intbl {\\b\\f1 ";
    for (const QString &header : headers) {
        out << toRtfString(header) << "\\cell ";
    }
    out << "}\\row\n";

    // Dữ liệu nhân viên
    for (const NhanVien &nv : m_list) {
        out << "\\trowd\\trgaph100\\trleft0\\trqc\n";
        pos = 0;
        for (int w : widths) {
            pos += w;
            out << "\\clbrdrl\\brdrs\\clbrdrt\\brdrs\\clbrdrb\\brdrs\\clbrdrr\\brdrs\\cellx" << pos;
        }
        out << "\n";

        out << "\\intbl\\f1 "
            << toRtfString(nv.id()) << "\\cell "
            << toRtfString(nv.hoTen()) << "\\cell "
            << toRtfString(nv.gioiTinh()) << "\\cell "
            << toRtfString(nv.ngaySinh()) << "\\cell "
            << toRtfString(nv.chucVu()) << "\\cell "
            << locale.toString(nv.luong()) << " VND\\cell "
            << locale.toString(nv.soNgayNghi()) << "\\cell "
            << toRtfString(nv.sdt()) << "\\cell "
            << locale.toString(nv.luongThucNhan()) << " VND\\cell \\row\n";
    }

    out << "\\pard\\sa200\\sl276\\slmult1\\par\n";
    out << "}";
    file.close();

    QMessageBox::information(this, "Xuất file thành công",
                             "Đã xuất dữ liệu thành công ra file:\n" + filePath);
}

void NhanVienManager::onExportToWord() {
    if (m_list.isEmpty()) {
        QMessageBox::warning(this, "Cảnh báo", "Không có dữ liệu để xuất!");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Lưu file RTF",
        "DanhSachNhanVien.rtf",
        "RTF Documents (*.rtf)"
        );

    if (filePath.isEmpty()) return;

    exportListToWord(filePath);
}

void NhanVienManager::onSave() {
    QFile file(QCoreApplication::applicationDirPath() + "/data/nhanvien.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Lỗi lưu file", "Không thể lưu dữ liệu!");
        return;
    }

    QTextStream out(&file);
    for (const NhanVien &nv : m_list) {
        out << nv.id() << ","
            << nv.hoTen() << ","
            << nv.gioiTinh() << ","
            << nv.ngaySinh() << ","
            << nv.chucVu() << ","
            << nv.luong() << ","
            << nv.soNgayNghi() << ","
            << nv.sdt() << "\n";
    }
    file.close();

    QMessageBox::information(this, "Thành công", "Đã lưu dữ liệu thành công!");
}

void NhanVienManager::onSearch() {
    QString query = m_searchEdit->text().trimmed();
    if (query.isEmpty()) {
        refreshTable();
        return;
    }

    bool found = false;
    for (const NhanVien &nv : m_list) {
        if (nv.id() == query || nv.sdt() == query) {  // Khớp chính xác 100% theo ID hoặc SDT
            m_table->setRowCount(0);  // Xóa bảng hiện tại
            QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
            addRowToTable(nv, locale);  // Thêm chỉ duy nhất một kết quả
            found = true;
            break;  // Chỉ hiển thị một kết quả đầu tiên tìm thấy
        }
    }

    if (!found) {
        QMessageBox::information(this, "Kết quả tìm kiếm", "Không tìm thấy nhân viên với ID hoặc SDT: " + query);
        refreshTable();  // Quay về hiển thị tất cả nếu không tìm thấy
    }
}

void NhanVienManager::onClearSearch() {
    m_searchEdit->clear();
    refreshTable();
}

void NhanVienManager::toggleInfoPanel(bool visible) {
    m_infoPanel->setVisible(visible);
    m_panelVisible = visible;
}
