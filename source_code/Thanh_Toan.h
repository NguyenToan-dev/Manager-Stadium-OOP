#ifndef THANH_TOAN_H
#define THANH_TOAN_H

#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QPushButton>

class Bill;

class Thanh_Toan : public QWidget {
    Q_OBJECT
public:
    Thanh_Toan(const QString& dataLine, QWidget* parent = nullptr);
    ~Thanh_Toan();

signals:
    void paymentCompleted();

private slots:
    void onThemDichVuClicked();
    void onThanhToanClicked();

private:
    void setupUI();
    void collectExtraServices();
    void updateSummaryTable();
    int getBookingsFromVip(const QString& name, const QString& phone);
    int getDiscountsApplied(const QString& name, const QString& phone);
    void updateDiscountsApplied(const QString& name, const QString& phone, int newDiscounts);

    Bill* bill;
    QString serviceName, courtName, date, timeFrom, timeTo, customerName, customerPhone;
    qint64 unitPrice;
    QStringList extraServiceLines;
    QString currentDataLine;

    QTreeWidget* extraTree;
    QTableWidget* summaryTable;
    QPushButton* btnThemDichVu;
    QPushButton* btnThanhToan;

    QList<QPair<QString, QList<QPair<QString, int>>>> danhMucDichVu;
};

#endif // THANH_TOAN_H
