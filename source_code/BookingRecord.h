#pragma once

#include <QString>
#include <QDate>
#include <QTime>

class BookingRecord {
public:
    BookingRecord();

    QString sport;
    QString fieldCode;
    QDate date;
    QTime timeStart;
    QTime timeEnd;
    QString customerName;
    QString phone;
    double money;
};
