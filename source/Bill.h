#pragma once
#include <QString>
#include <QStringList>
#include <QImage>

class Bill {
public:
    Bill(const QString& dataLine);
    virtual ~Bill() = default;

    virtual bool generateImage(const QString& outputJpgPath) = 0;  // <-- PURE VIRTUAL
    virtual qint64 getTotal() const;
    virtual QString getCustomerName() const;

protected:
    void parseLine(const QString& line);

    QString serviceName, courtName, date, timeFrom, timeTo, customerName, customerPhone;
    qint64 unitPrice;
    QList<QStringList> extraServices;
};
