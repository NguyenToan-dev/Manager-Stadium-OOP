#pragma once
#include "Bill.h"
#include <QTime>
#include <QCoreApplication>

class DiscountBill : public Bill {
public:
    DiscountBill(const QString& dataLine, int discountPercent = 10);
    ~DiscountBill() override = default;

    qint64 getTotal() const override;
    QString getCustomerName() const override;
    bool generateImage(const QString& outputJpgPath) override;

private:
    int discountPercent;
};
