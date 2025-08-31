#include "DiscountBill.h"
#include <QPainter>
#include <QLocale>

DiscountBill::DiscountBill(const QString& dataLine, int discountPercent)
    : Bill(dataLine), discountPercent(discountPercent) {
}

qint64 DiscountBill::getTotal() const {
    qint64 total = Bill::getTotal();
    return total - (total * discountPercent / 100);
}

QString DiscountBill::getCustomerName() const {
    return customerName + " (VIP)";
}

bool DiscountBill::generateImage(const QString& outputJpgPath) {
    // Draw as Bill, but show VIP and discounted total
    const int W = 800, H = 1200;
    QImage img(W, H, QImage::Format_ARGB32);
    img.fill(Qt::white);

    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);

    QColor headerBg(40, 167, 69);
    p.fillRect(0, 0, W, 140, headerBg);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 24, QFont::Bold));
    p.drawText(QRect(0, 0, W, 140), Qt::AlignCenter, "HÓA ĐƠN THANH TOÁN DỊCH VỤ");

    p.setPen(Qt::black);
    QFont lblF("Arial", 10, QFont::Bold), valF("Arial", 10);
    int xL = 40, xV = 180, y = 160, lh = 25;
    auto draw = [&](const QString& L, const QString& V) {
        p.setFont(lblF); p.drawText(xL, y, L);
        p.setFont(valF); p.drawText(xV, y, V);
        y += lh;
    };

    draw("Khách hàng:", getCustomerName());
    draw("Điện thoại:", customerPhone);
    draw("Dịch vụ:", serviceName);
    draw("Sân:", courtName);
    draw("Ngày:", date);
    draw("Giờ từ:", timeFrom + " đến: " + timeTo);

    QTime tFrom = QTime::fromString(timeFrom, "hh:mm");
    QTime tTo = QTime::fromString(timeTo, "hh:mm");
    double qtyField = tFrom.isValid() && tTo.isValid() ? tFrom.secsTo(tTo) / 3600.0 : 1.0;
    qint64 costField = qint64(unitPrice * qtyField);

    y += 20;
    int tx = 30, tW = W - 60, rowH = 30;
    p.fillRect(tx, y, tW, rowH, QColor(233, 245, 233));
    p.setFont(lblF);
    p.drawText(tx + 10, y + 20, "STT");
    p.drawText(tx + 60, y + 20, "Tên dịch vụ");
    p.drawText(tx + 300, y + 20, "Đơn giá");
    p.drawText(tx + 450, y + 20, "Số lượng");
    p.drawText(tx + 600, y + 20, "Thành tiền");
    y += rowH;

    p.setFont(valF);
    int stt = 1;
    QString cleanCourt = courtName;
    if (cleanCourt.startsWith("Sân "))
        cleanCourt = cleanCourt.mid(4);

    QString svcLabel = QString("Môn %1 - Sân %2").arg(serviceName).arg(cleanCourt);
    p.drawText(tx + 10, y + 20, QString::number(stt));
    p.drawText(tx + 60, y + 20, svcLabel);
    p.drawText(tx + 300, y + 20, QLocale("vi_VN").toString(unitPrice) + " đ");
    p.drawText(tx + 450, y + 20, QString::number(qtyField, 'f', 1) + " giờ");
    p.drawText(tx + 600, y + 20, QLocale("vi_VN").toString(costField) + " đ");
    y += rowH;

    qint64 sum = costField;
    for (const auto& row : extraServices) {
        QString name = row[0];
        qint64 price = row[1].toLongLong();
        int q = row[2].toInt();
        qint64 tot = price * q;
        sum += tot;

        p.drawText(tx + 10, y + 20, QString::number(++stt));
        p.drawText(tx + 60, y + 20, name);
        p.drawText(tx + 300, y + 20, QLocale("vi_VN").toString(price) + " đ");
        p.drawText(tx + 450, y + 20, QString::number(q));
        p.drawText(tx + 600, y + 20, QLocale("vi_VN").toString(tot) + " đ");
        y += rowH;
    }

    p.setPen(QPen(Qt::black, 1));
    p.drawRect(tx, y - rowH * (1 + extraServices.size()), tW, rowH * (2 + extraServices.size()));

    y += 20;
    p.setFont(lblF);
    p.drawText(tx + 450, y, "Tổng tiền (giảm 10%):");
    p.setFont(valF);
    p.drawText(tx + 600, y, QLocale("vi_VN").toString(getTotal()) + " đ");

    QPixmap logo(QCoreApplication::applicationDirPath() + "/images/logo.jpg");
    if (!logo.isNull()) {
        QRect logoRect(20, 40, 100, 100);
        p.drawPixmap(logoRect, logo);
    }
    // ⬇️ Thêm biểu tượng VIP ở góc phải
    QPixmap vip(QCoreApplication::applicationDirPath() + "/images/VIP.png");
    if (!vip.isNull()) {
        QRect vipRect(W - 120, 20, 100, 100);  // Góc phải trên
        p.drawPixmap(vipRect, vip);
    }
    return img.save(outputJpgPath, "JPG", 95);
}
