#include "Bill.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTime>
#include <QPainter>
#include <QFont>
#include <QPixmap>
#include <QLocale>

Bill::Bill(const QString& dataLine) {
    parseLine(dataLine);
}

void Bill::parseLine(const QString& line) {
    serviceName = courtName = date = timeFrom = timeTo = customerName = customerPhone = "";
    unitPrice = 0;
    extraServices.clear();

    QStringList parts = line.split('|');
    QString mainPart = parts[0].trimmed();
    QStringList fields = mainPart.split('-', Qt::SkipEmptyParts);

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

    if (parts.size() > 1) {
        QString extrasStr = parts[1];
        QStringList items = extrasStr.split(';', Qt::SkipEmptyParts);
        for (const QString& item : items) {
            QStringList cols = item.split(',', Qt::SkipEmptyParts);
            if (cols.size() == 3) {
                QStringList row;
                row << cols[0].trimmed() << cols[1].trimmed() << cols[2].trimmed();
                extraServices.append(row);
            }
        }
    }
}

qint64 Bill::getTotal() const {
    QTime tFrom = QTime::fromString(timeFrom, "hh:mm");
    QTime tTo = QTime::fromString(timeTo, "hh:mm");
    double qtyField = tFrom.isValid() && tTo.isValid() ? tFrom.secsTo(tTo) / 3600.0 : 1.0;
    qint64 costField = qint64(unitPrice * qtyField);

    qint64 sum = costField;
    for (const auto& row : extraServices) {
        qint64 price = row[1].toLongLong();
        int q = row[2].toInt();
        sum += price * q;
    }
    return sum;
}

QString Bill::getCustomerName() const {
    return customerName;
}

