#pragma once
#include "Bill.h"

class NormalBill : public Bill {
public:
    NormalBill(const QString& dataLine) : Bill(dataLine) {}
    ~NormalBill() override = default;
    bool generateImage(const QString& outputJpgPath) override;
};
