#pragma once
#include "BookingRecord.h"
#include <QWidget>
#include <QDate>
#include <QTime>
#include <QList>
#include <QPair>
#include <QTableWidget>
#include <QChartView>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>

class Bao_Cao_Thong_Ke : public QWidget {
    Q_OBJECT
public:
    explicit Bao_Cao_Thong_Ke(QWidget* parent = nullptr);

    void onChartTypeChanged(int index);
    void onDateRangeChanged();
    void parseLine(const QString& line, BookingRecord& rec);
    QString sportToName(const QString& sport) const;
    void loadData();
    void exportToCSV();

protected:
    void setupUI();
    void updateChart();

    QString currentSport;
    int currentChartType;
    QWidget* leftWidget;
    QWidget* rightWrapper;
    QDate minDate, maxDate;
    QPushButton* btnFootball;
    QPushButton* btnBasketball;
    QPushButton* btnVolleyball;
    QComboBox* chartTypeCombo;
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
    QChartView* chartView;
    QTableWidget* serviceTable;
    QComboBox* filterCombo;
    QLabel* totalLabel;
    QLabel* titleLabel;

    QList<BookingRecord> records;
    QMap<QString, QPair<int, qint64>> readServiceStatsFromFile();
    // Dịch vụ đi kèm
    QList<QPair<QString, QList<QPair<QString, int>>>> danhMucDichVu = {
        {"Dịch vụ", {
                        {"Trọng tài", 100000},
                        {"Bảng tỉ số", 50000}
                    }},
        {"Giày thi đấu", {
                             {"Giày đá bóng", 30000},
                             {"Giày bóng chuyền", 25000},
                             {"Giày bóng rổ", 25000},
                             {"Tất thể thao", 5000},
                             {"Băng cổ chân", 10000}
                         }},
        {"Đồ uống", {
                        {"Sting", 15000}, {"Revive chanh muối", 15000}, {"Revive chanh", 15000},
                        {"Nước suối", 5000}, {"Aquafina", 7000}, {"Coca Cola", 12000},
                        {"Trà chanh C2", 10000}, {"Nước cam", 15000}, {"Sữa đậu nành", 10000}
                    }},
        {"Y tế", {
                     {"Sơ cứu cơ bản", 20000}, {"Dán salonpas", 10000},
                     {"Thuốc giảm đau", 3000}, {"Chườm đá", 5000}
                 }}
    };
};
