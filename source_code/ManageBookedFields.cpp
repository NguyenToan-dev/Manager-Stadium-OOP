#include "ManageBookedFields.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>
#include <QStyle>
#include <QLineEdit>
#include <QLabel>
#include <QDir>
#include <QByteArray>

#include "Thanh_Toan.h"

ManageBookedFields::ManageBookedFields(QWidget* parent)
    : QWidget(parent)
{
    this->setObjectName("ManageBookedFields");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 15, 30, 15);
    mainLayout->setSpacing(15);

    // --- Search Bar ---
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel* searchLabel = new QLabel("Tìm kiếm:");
    QLineEdit* searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Tìm kiếm theo thông tin sân đã đặt...");
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchInput);
    mainLayout->addLayout(searchLayout);

    // --- Table Widget ---
    table = new QTableWidget(this);
    table->setColumnCount(10);
    table->setHorizontalHeaderLabels({
        "Môn", "Sân", "Ngày", "Giờ bắt đầu", "Giờ kết thúc", "Người đặt", "SĐT", "Giá", "Hủy", "Thanh toán"
    });
    table->setSortingEnabled(true);
    table->verticalHeader()->setVisible(false);

    // --- Column Sizing ---
    QHeaderView* header = table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(9, QHeaderView::ResizeToContents);

    this->setStyleSheet(R"(
        #ManageBookedFields {
            background-color: #3e4a59;
            font-family: "Segoe UI", "Arial";
            color: #e0e0e0;
        }
        QLabel {
            font-size: 13px;
            color: #e0e0e0;
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
            padding: 8px;
            border: none;
            border-bottom: 1px solid #4a637d;
        }
        QTableWidget::item {
            padding: 8px;
            color: #e0e0e0;
            border-bottom: 1px solid #4a637d;
        }
        QTableWidget::item:selected {
            background-color: #0d6efd;
            color: #ffffff;
        }
        QTableWidget:focus {
            outline: 0;
        }
        QScrollBar:vertical {
            border: none;
            background: #2c3e50;
            width: 10px;
            margin: 0px 0px 0px 0px;
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
            margin: 0px 0px 0px 0px;
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

    connect(searchInput, &QLineEdit::textChanged, this, [this](const QString& text){
        for (int i = 0; i < table->rowCount(); ++i) {
            bool match = false;
            for (int j = 0; j < table->columnCount() - 2; ++j) {
                QTableWidgetItem* item = table->item(i, j);
                if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                    match = true;
                    break;
                }
            }
            table->setRowHidden(i, !match);
        }
    });

    QPushButton* historyButton = new QPushButton("Lịch sử đặt sân", this);
    historyButton->setCursor(Qt::PointingHandCursor);
    historyButton->setStyleSheet(R"(
        QPushButton {
            background-color: #0d6efd;
            color: white;
            font-weight: bold;
            border-radius: 4px;
            padding: 6px;
        }
        QPushButton:hover {
            background-color: #0b5ed7;
        }
    )");
    searchLayout->addWidget(historyButton);

    showingHistory = false;

    connect(historyButton, &QPushButton::clicked, this, [this, historyButton]() mutable {
        table->sortItems(9, Qt::AscendingOrder);
        table->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
        this->showingHistory = !this->showingHistory;
        if (this->showingHistory) {
            historyButton->setText("Sân đang đặt");
            historyButton->setStyleSheet(R"(
                QPushButton {
                    background-color: #38ff4c;
                    color: #3e4a59;
                    font-weight: bold;
                    border-radius: 4px;
                    padding: 6px;
                }
                QPushButton:hover {
                    background-color: #79fc86;
                }
            )");
            loadBookings(QCoreApplication::applicationDirPath() + "/data/data.txt");
        } else {
            historyButton->setText("Lịch sử đặt sân");
            historyButton->setStyleSheet(R"(
            QPushButton {
                background-color: #ff7e42;
                color: white;
                font-weight: bold;
                border-radius: 4px;
                padding: 6px;
            }
            QPushButton:hover {
                background-color: #ffb896;
            }
        )");
            loadBookings(QCoreApplication::applicationDirPath() + "/data/booking.txt");
        }
    });

    loadBookings(QCoreApplication::applicationDirPath() + "/data/booking.txt");
}

// Subclass để sort theo ngày
class DateItem : public QTableWidgetItem {
public:
    DateItem(const QString& text) : QTableWidgetItem(text) {}
    bool operator<(const QTableWidgetItem &other) const override {
        QDate d1 = QDate::fromString(text(), "dd/MM/yyyy");
        QDate d2 = QDate::fromString(other.text(), "dd/MM/yyyy");
        return d1 < d2;
    }
};

void ManageBookedFields::loadBookings(const QString& filePath)
{
    int sortedColumn = table->horizontalHeader()->sortIndicatorSection();
    Qt::SortOrder sortOrder = table->horizontalHeader()->sortIndicatorOrder();

    table->clear();
    table->setRowCount(0);

    if (filePath.endsWith("booking.txt")) {
        table->setColumnCount(10);
        table->setHorizontalHeaderLabels({
            "Môn", "Sân", "Ngày", "Giờ bắt đầu", "Giờ kết thúc", "Người đặt", "SĐT", "Giá", "Hủy", "Thanh toán"
        });
    } else {
        table->setColumnCount(8);
        table->setHorizontalHeaderLabels({
            "Môn", "Sân", "Ngày", "Giờ bắt đầu", "Giờ kết thúc", "Người đặt", "SĐT", "Giá"
        });
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    int row = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split("-");
        if (parts.size() != 8) continue;

        if (!filePath.endsWith("booking.txt")) { // lịch sử
            QString dateStr = parts[2];
            QString endTimeStr = parts[4];
            QDateTime endDateTime = QDateTime::fromString(dateStr + " " + endTimeStr, "dd/MM/yyyy HH:mm");
            if (!endDateTime.isValid() || endDateTime > QDateTime::currentDateTime())
                continue;
        }

        table->insertRow(row);
        for (int i = 0; i < 8; ++i) {
            QTableWidgetItem* item = nullptr;
            if (i == 2)
                item = new DateItem(parts[i]);
            else
                item = new QTableWidgetItem(parts[i]);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, i, item);
        }

        if (filePath.endsWith("booking.txt")) {
            // --- Hủy ---
            QPushButton* deleteButton = new QPushButton("Hủy");
            deleteButton->setCursor(Qt::PointingHandCursor);
            deleteButton->setStyleSheet(R"(
                QPushButton {
                    background-color: transparent;
                    color: #e74c3c;
                    font-weight: bold;
                    border-radius: 4px;
                }
                QPushButton:hover {
                    color: white;
                }
            )");
            QWidget* deleteContainer = new QWidget();
            QHBoxLayout* deleteLayout = new QHBoxLayout(deleteContainer);
            deleteLayout->addWidget(deleteButton);
            deleteLayout->setAlignment(Qt::AlignCenter);
            deleteLayout->setContentsMargins(0,0,0,0);
            table->setCellWidget(row, 8, deleteContainer);

            connect(deleteButton, &QPushButton::clicked, this, [=]() { handleDeleteRow(line); });

            // --- Thanh toán ---
            QPushButton* payButton = new QPushButton("Thanh toán");
            payButton->setCursor(Qt::PointingHandCursor);
            payButton->setStyleSheet(R"(
                QPushButton {
                    background-color: transparent;
                    color: #0d6efd;
                    font-weight: bold;
                    border-radius: 4px;
                }
                QPushButton:hover {
                    color: white;
                }
            )");
            QWidget* payContainer = new QWidget();
            QHBoxLayout* payLayout = new QHBoxLayout(payContainer);
            payLayout->addWidget(payButton);
            payLayout->setAlignment(Qt::AlignCenter);
            payLayout->setContentsMargins(0,0,0,0);
            table->setCellWidget(row, 9, payContainer);

            connect(payButton, &QPushButton::clicked, this, [=]() {
                QString rowData;
                for (int i = 0; i < 8; ++i)
                    rowData += (table->item(row,i) ? table->item(row,i)->text().trimmed() : "") + (i<7 ? "-" : "");
                Thanh_Toan* paymentWindow = new Thanh_Toan(rowData, nullptr);

                QObject::connect(paymentWindow, &Thanh_Toan::paymentCompleted, this, [this, filePath]() {
                    QMessageBox successBox(this);
                    successBox.setWindowTitle("Thành công");
                    successBox.setText("Thanh toán thành công!");
                    successBox.setIcon(QMessageBox::Information);
                    successBox.setStyleSheet(R"(
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
                    successBox.exec();
                    loadBookings(filePath);
                });

                paymentWindow->setAttribute(Qt::WA_DeleteOnClose);
                paymentWindow->show();
            });
        }

        ++row;
    }

    file.close();
    table->sortItems(sortedColumn, sortOrder);
}

void ManageBookedFields::handleDeleteRow(const QString& lineToDelete)
{
    if (lineToDelete.isEmpty()) return;

    // --- Xác nhận hủy ---
    QMessageBox confirmBox(this);
    confirmBox.setWindowTitle("Xác nhận hủy");
    confirmBox.setText("Bạn có chắc chắn muốn hủy sân này không?");
    confirmBox.setIcon(QMessageBox::Question);
    confirmBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmBox.setDefaultButton(QMessageBox::No);
    confirmBox.setStyleSheet(R"(
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
    int reply = confirmBox.exec();
    if (reply != QMessageBox::Yes) return;

    QFile file(QCoreApplication::applicationDirPath() + "/data/booking.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox errorBox(this);
        errorBox.setWindowTitle("Lỗi");
        errorBox.setText("Không thể đọc file booking.txt để xóa.");
        errorBox.setIcon(QMessageBox::Warning);
        errorBox.setStyleSheet(R"(
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
        errorBox.exec();
        return;
    }

    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString currentLine = in.readLine();
        if (currentLine.trimmed() != lineToDelete.trimmed()) {
            lines << currentLine;
        }
    }
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox errorBox(this);
        errorBox.setWindowTitle("Lỗi");
        errorBox.setText("Không thể ghi vào file booking.txt để cập nhật.");
        errorBox.setIcon(QMessageBox::Warning);
        errorBox.setStyleSheet(R"(
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
        errorBox.exec();
        return;
    }

    QTextStream out(&file);
    for (const QString& l : lines) {
        out << l << "\n";
    }
    file.close();

    QMessageBox successBox(this);
    successBox.setWindowTitle("Thành công");
    successBox.setText("Đã hủy sân thành công.");
    successBox.setIcon(QMessageBox::Information);
    successBox.setStyleSheet(R"(
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
    successBox.exec();

    loadBookings(QCoreApplication::applicationDirPath() + "/data/booking.txt");
}
