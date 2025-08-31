#include "Thanh_Toan.h"
#include "NormalBill.h"
#include "DiscountBill.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QSpinBox>
#include <QLocale>
#include <QFile>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QRectF>
#include <QStandardPaths>

Thanh_Toan::Thanh_Toan(const QString& dataLine, QWidget* parent)
    : QWidget(parent), bill(nullptr)
{
    // Parse dữ liệu ban đầu
    QStringList fields = dataLine.split('-', Qt::SkipEmptyParts);
    if (fields.size() >= 8) {
        serviceName = fields.at(0).trimmed();
        courtName = fields.at(1).trimmed();
        date = fields.at(2).trimmed();
        timeFrom = fields.at(3).trimmed();
        timeTo = fields.at(4).trimmed();
        customerName = fields.at(5).trimmed();
        customerPhone = fields.at(6).trimmed();
        unitPrice = fields.at(7).trimmed().toLongLong();
    }

    setStyleSheet(R"(
        /* === Main Widget Background === */
        QWidget {
            background-color: #3e4a59;
            font-family: 'Segoe UI', sans-serif;
            font-size: 14px;
        }

        /* === Labels === */
        QLabel {
            background-color: transparent;
            color: #bdc3c7;
            font-size: 13px;
            font-weight: 500;
        }

        /* === Tree Widget === */
        QTreeWidget {
            background-color: #2c3e50;
            border: 1px solid #4a637d;
            border-radius: 4px;
            color: #e0e0e0;
        }
        QTreeWidget::item {
            padding: 4px;
        }
        QTreeWidget::item:selected {
            background-color: #0d6efd;
            color: #ffffff;
        }

        /* === Table Widget === */
        QTableWidget {
            background-color: #2c3e50;
            border: 1px solid #4a637d;
            border-radius: 4px;
            color: #e0e0e0;
        }
        QTableWidget::item {
            padding: 4px;
        }
        QTableWidget::item:selected {
            background-color: #0d6efd;
            color: #ffffff;
        }

        /* === Header View === */
        QHeaderView::section {
            background-color: #34495e;
            padding: 4px;
            color: #e0e0e0;
            font-weight: bold;
            border: 1px solid #4a637d;
        }
        QTableWidget QTableCornerButton::section {
            background-color: #34495e;
            border: 1px solid #4a637d;
        }

        /* === Spin Box === */
        QSpinBox {
            background-color: #3e4a59;
            border: 1px solid #4a637d;
            border-radius: 4px;
            padding: 0 12px;
            font-size: 14px;
            color: #e0e0e0;
            min-height: 30px;
        }
        QSpinBox:focus {
            border-color: #0d6efd;
        }
        QSpinBox::up-button, QSpinBox::down-button {
            background-color: #34495e;
            border: none;
            width: 20px;
        }
        QSpinBox::up-button:hover, QSpinBox::down-button:hover {
            background-color: #4a637d;
        }

        /* === Button Base Style === */
        QPushButton {
            padding: 10px 20px;
            font-size: 14px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
        }

        /* === Primary Action Buttons (Add Service, Pay) === */
        #btnThemDichVu, #btnThanhToan {
            background-color: #0d6efd;
            color: white;
        }
        #btnThemDichVu:hover, #btnThanhToan:hover {
            background-color: #0b5ed7;
        }
        QMessageBox {
            background-color: #3e4a59;
            border: 1px solid #4a637d;
        }
        QMessageBox QLabel {
            color: #e0e0e0;
            font-size: 14px;
        }

        QMessageBox QDialogButtonBox QPushButton, QMessageBox QPushButton {
            background-color: #0d6efd;
            color: white;
            padding: 8px 18px;
            border-radius: 5px;
            font-weight: bold;
            border: none;
            min-width: 88px;
        }
        QMessageBox QDialogButtonBox QPushButton:hover, QMessageBox QPushButton:hover {
            background-color: #0b5ed7;
        }
        QMessageBox QDialogButtonBox QPushButton:pressed, QMessageBox QPushButton:pressed {
            background-color: #0a58ca;
        }
        QMessageBox QDialogButtonBox QPushButton:focus, QMessageBox QPushButton:focus {
            border: 1px solid #0d6efd;
        }
    )");

    setupUI();
    setFixedSize(500, 500);
}

Thanh_Toan::~Thanh_Toan() {
    if (bill) delete bill;
}

void Thanh_Toan::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* lblExtra = new QLabel("Danh sách dịch vụ thêm:");
    mainLayout->addWidget(lblExtra);

    extraTree = new QTreeWidget();
    extraTree->setColumnCount(3);
    extraTree->setHeaderLabels({ "Tên dịch vụ", "Giá (VNĐ)", "Số lượng" });
    extraTree->setColumnWidth(0, 200);
    mainLayout->addWidget(extraTree);

    danhMucDichVu = {
        {"Dịch vụ", {{"Trọng tài", 100000}, {"Bảng tỉ số", 50000}}},
        {"Giày thi đấu", {{"Giày đá bóng", 30000}, {"Giày bóng chuyền", 25000}, {"Giày bóng rổ", 25000}, {"Tất thể thao", 5000}, {"Băng cổ chân", 10000}}},
        {"Đồ uống", {{"Sting", 15000}, {"Revive chanh muối", 15000}, {"Revive chanh", 15000}, {"Nước suối", 5000}, {"Aquafina", 7000}, {"Coca Cola", 12000}, {"Trà chanh C2", 10000}, {"Nước cam", 15000}, {"Sữa đậu nành", 10000}}},
        {"Y tế", {{"Sơ cứu cơ bản", 20000}, {"Dán salonpas", 10000}, {"Thuốc giảm đau", 3000}, {"Chườm đá", 5000}}}
    };

    for (auto& group : danhMucDichVu) {
        QTreeWidgetItem* parentItem = new QTreeWidgetItem(extraTree);
        parentItem->setText(0, group.first);
        parentItem->setFirstColumnSpanned(true);
        parentItem->setExpanded(false);

        for (auto& dv : group.second) {
            QTreeWidgetItem* child = new QTreeWidgetItem();
            child->setText(0, dv.first);
            child->setText(1, QString::number(dv.second));
            QSpinBox* sp = new QSpinBox();
            if (dv.first == "Trọng tài" || dv.first == "Bảng tỉ số") {
                sp->setMaximum(1);
            }
            else {
                sp->setMaximum(100);
            }
            sp->setValue(0);

            parentItem->addChild(child);
            extraTree->setItemWidget(child, 2, sp);
        }
    }

    btnThemDichVu = new QPushButton("Thêm dịch vụ vào bảng");
    btnThemDichVu->setObjectName("btnThemDichVu");
    connect(btnThemDichVu, &QPushButton::clicked, this, &Thanh_Toan::onThemDichVuClicked);
    mainLayout->addWidget(btnThemDichVu);

    btnThanhToan = new QPushButton("Thanh toán và in hoá đơn");
    btnThanhToan->setObjectName("btnThanhToan");
    connect(btnThanhToan, &QPushButton::clicked, this, &Thanh_Toan::onThanhToanClicked);
    mainLayout->addWidget(btnThanhToan);

    summaryTable = new QTableWidget(0, 4);
    summaryTable->setHorizontalHeaderLabels({ "Dịch vụ", "Đơn giá", "Số lượng", "Thành tiền" });
    summaryTable->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(summaryTable);
}

void Thanh_Toan::collectExtraServices() {
    extraServiceLines.clear();
    for (int i = 0; i < extraTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem* parent = extraTree->topLevelItem(i);
        for (int j = 0; j < parent->childCount(); ++j) {
            QTreeWidgetItem* item = parent->child(j);
            QString name = item->text(0);
            int price = item->text(1).toInt();
            QSpinBox* sp = qobject_cast<QSpinBox*>(extraTree->itemWidget(item, 2));
            int qty = sp ? sp->value() : 0;
            if (qty > 0) {
                extraServiceLines.append(QString("%1,%2,%3").arg(name).arg(price).arg(qty));
            }
        }
    }
}

void Thanh_Toan::updateSummaryTable() {
    collectExtraServices();

    QString dataLine = QString("%1-%2-%3-%4-%5-%6-%7-%8")
                           .arg(serviceName)
                           .arg(courtName)
                           .arg(date)
                           .arg(timeFrom)
                           .arg(timeTo)
                           .arg(customerName)
                           .arg(customerPhone)
                           .arg(unitPrice);

    if (!extraServiceLines.isEmpty())
        dataLine += "|" + extraServiceLines.join(";");

    currentDataLine = dataLine;

    QTime start = QTime::fromString(timeFrom, "hh:mm");
    QTime end = QTime::fromString(timeTo, "hh:mm");
    double hours = start.secsTo(end) / 3600.0;
    if (hours <= 0) hours = 1;
    qint64 fieldCost = unitPrice * hours;

    qint64 extraTotal = 0;
    QList<QStringList> extraRows;
    if (dataLine.contains('|')) {
        QStringList mainAndExtra = dataLine.split('|');
        if (mainAndExtra.size() > 1) {
            QStringList extraList = mainAndExtra[1].split(';', Qt::SkipEmptyParts);
            for (const QString& row : extraList) {
                QStringList cols = row.split(',');
                if (cols.size() == 3) {
                    extraRows.append(cols);
                    extraTotal += cols[1].toLongLong() * cols[2].toInt();
                }
            }
        }
    }

    summaryTable->setRowCount(0);
    int row = 0;
    summaryTable->insertRow(row);
    summaryTable->setItem(row, 0, new QTableWidgetItem(courtName));
    summaryTable->setItem(row, 1, new QTableWidgetItem(QString::number(unitPrice)));
    summaryTable->setItem(row, 2, new QTableWidgetItem(QString::number(hours, 'f', 1)));
    summaryTable->setItem(row, 3, new QTableWidgetItem(QString::number(fieldCost)));
    ++row;

    for (const QStringList& cols : extraRows) {
        summaryTable->insertRow(row);
        summaryTable->setItem(row, 0, new QTableWidgetItem(cols[0]));
        summaryTable->setItem(row, 1, new QTableWidgetItem(cols[1]));
        summaryTable->setItem(row, 2, new QTableWidgetItem(cols[2]));
        summaryTable->setItem(row, 3, new QTableWidgetItem(QString::number(cols[1].toInt() * cols[2].toInt())));
        ++row;
    }
    summaryTable->resizeRowsToContents();
}

void Thanh_Toan::onThemDichVuClicked() {
    updateSummaryTable();
}

void Thanh_Toan::onThanhToanClicked() {
    updateSummaryTable();

    if (bill) delete bill;

    // ==== 1. Tạo Bill ====
    int vipBookings = getBookingsFromVip(customerName, customerPhone);
    if (vipBookings == 0) {
        bill = new NormalBill(currentDataLine);
    } else {
        int discountsApplied = getDiscountsApplied(customerName, customerPhone);
        QFile discountFile(QCoreApplication::applicationDirPath() + "/data/bill_discount.txt");
        bool isEmpty = true;
        if (discountFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&discountFile);
            if (!in.atEnd()) isEmpty = false;
            discountFile.close();
        }

        if (isEmpty || discountsApplied == 0) {
            bill = new DiscountBill(currentDataLine, 10);
            updateDiscountsApplied(customerName, customerPhone, 1);
        } else {
            if (vipBookings >= 5 * discountsApplied) {
                bill = new DiscountBill(currentDataLine, 10);
                updateDiscountsApplied(customerName, customerPhone, discountsApplied + 1);
            } else {
                bill = new NormalBill(currentDataLine);
            }
        }
    }

    qint64 grandTotal = bill->getTotal();
    bool isDiscount = dynamic_cast<DiscountBill*>(bill) != nullptr;

    // ==== 2. Ghi thống kê dịch vụ ====
    QMap<QString, QPair<int, qint64>> thongKeMap;
    QString thongKePath = QCoreApplication::applicationDirPath() + "/data/DichVu_ThongKe.txt";

    QFile fileIn(thongKePath);
    if (fileIn.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fileIn);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QStringList parts = line.split(',');
            if (parts.size() == 4) {
                QString key = parts[0] + "," + parts[1];
                int luot = parts[2].toInt();
                qint64 doanhThu = parts[3].remove('.').remove(',').toLongLong();
                thongKeMap[key] = qMakePair(luot, doanhThu);
            }
        }
        fileIn.close();
    }

    for (const QString& row : extraServiceLines) {
        QStringList cols = row.split(',');
        if (cols.size() == 3) {
            QString tenDV = cols[0];
            qint64 gia = cols[1].toLongLong();
            int sl = cols[2].toInt();
            qint64 doanhThu = gia * sl;

            QString loaiDV;
            for (auto& group : danhMucDichVu) {
                for (auto& dv : group.second) {
                    if (dv.first == tenDV) {
                        loaiDV = group.first;
                        break;
                    }
                }
                if (!loaiDV.isEmpty()) break;
            }

            QString key = loaiDV + "," + tenDV;
            if (thongKeMap.contains(key)) {
                thongKeMap[key].first += sl;
                thongKeMap[key].second += doanhThu;
            } else {
                thongKeMap[key] = qMakePair(sl, doanhThu);
            }
        }
    }

    QFile fileOut(thongKePath);
    if (fileOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&fileOut);
        for (auto it = thongKeMap.begin(); it != thongKeMap.end(); ++it) {
            QString key = it.key();
            int luot = it.value().first;
            qint64 doanhThu = it.value().second;
            out << key << "," << luot << "," << QLocale("vi_VN").toString(doanhThu) << "\n";
        }
        fileOut.close();
    }

    // ==== 3. Hiện tổng tiền ====
    QString totalMsg = isDiscount ?
                           QString("Tổng tiền (giảm 10%): %1 đ").arg(QLocale("vi_VN").toString(grandTotal)) :
                           QString("Tổng tiền: %1 đ").arg(QLocale("vi_VN").toString(grandTotal));

    auto reply = QMessageBox::information(
        this, "Tổng tiền", totalMsg,
        QMessageBox::Ok | QMessageBox::Cancel
        );

    if (reply != QMessageBox::Ok) {
        this->close();
        return;
    }

    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_bill.jpg";
    if (!tempPath.isEmpty()) {
        bill->generateImage(tempPath);
    }

    // ==== 5. In ====
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize::A4);
    printer.setFullPage(true);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QImage img(tempPath);
        if (!img.isNull() && painter.isActive()) {
            QRect viewport = painter.viewport();
            QSize imgSize = img.size();
            imgSize.scale(viewport.size(), Qt::KeepAspectRatio);
            painter.setViewport(viewport.x(), viewport.y(), imgSize.width(), imgSize.height());
            painter.setWindow(img.rect());
            painter.drawImage(0, 0, img);
        }

        // ==== 6. Ghi data.txt và booking.txt ====
        QString dataFilePath = QCoreApplication::applicationDirPath() + "/data/data.txt";
        QFile dataFile(dataFilePath);
        if (dataFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&dataFile);
            out << currentDataLine << "\n";
            dataFile.close();
        }

        QString bookingPath = QCoreApplication::applicationDirPath() + "/data/booking.txt";
        QFile bookingFile(bookingPath);
        QStringList allLines;
        if (bookingFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&bookingFile);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty() && line != currentDataLine.split('|')[0]) {
                    allLines << line;
                }
            }
            bookingFile.close();
        }
        if (bookingFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream out(&bookingFile);
            for (const QString& l : allLines) {
                out << l << "\n";
            }
            bookingFile.close();
        }
        emit paymentCompleted();
        QFile::remove(tempPath);  // Xóa file tạm
    }

    this->close();
}

int Thanh_Toan::getBookingsFromVip(const QString& name, const QString& phone) {
    QFile vipFile(QCoreApplication::applicationDirPath() + "/data/vip.txt");
    if (vipFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&vipFile);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) {
                int num = parts.last().toInt();
                QString phoneFromFile = parts[parts.size() - 2];
                QString nameFromFile = parts.mid(0, parts.size() - 2).join(" ");
                if (nameFromFile == name && phoneFromFile == phone) {
                    return num;
                }
            }
        }
        vipFile.close();
    }
    return 0; // Không tìm thấy
}

int Thanh_Toan::getDiscountsApplied(const QString& name, const QString& phone) {
    QFile discountFile(QCoreApplication::applicationDirPath() + "/data/bill_discount.txt");
    if (discountFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&discountFile);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) {
                int discounts = parts.last().toInt();
                QString phoneFromFile = parts[parts.size() - 2];
                QString nameFromFile = parts.mid(0, parts.size() - 2).join(" ");
                if (nameFromFile == name && phoneFromFile == phone) {
                    return discounts;
                }
            }
        }
        discountFile.close();
    }
    return 0; // Không tìm thấy
}

void Thanh_Toan::updateDiscountsApplied(const QString& name, const QString& phone, int newDiscounts) {
    QFile discountFile(QCoreApplication::applicationDirPath() + "/data/bill_discount.txt");
    QStringList lines;
    bool found = false;
    if (discountFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&discountFile);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) {
                QString phoneFromFile = parts[parts.size() - 2];
                QString nameFromFile = parts.mid(0, parts.size() - 2).join(" ");
                if (nameFromFile == name && phoneFromFile == phone) {
                    line = name + " " + phone + " " + QString::number(newDiscounts);
                    found = true;
                }
            }
            lines << line;
        }
        discountFile.close();
    }
    if (!found) {
        lines << name + " " + phone + " " + QString::number(newDiscounts);
    }
    if (discountFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&discountFile);
        for (const QString& l : lines) {
            out << l << "\n";
        }
        discountFile.close();
    }
}
