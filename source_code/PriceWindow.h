#ifndef PRICEWINDOW_H
#define PRICEWINDOW_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QTime>

class PriceWindow : public QWidget {
    Q_OBJECT

public:
    PriceWindow(const QString& sport, const QString& field, QWidget* parent = nullptr);
    QString removeAccents(const QString& input);

private:
    QString sportName, fieldName;
    QVector<QPair<QTime, QTime>> timeRanges;
    QVBoxLayout* entryLayout;
    void loadExisting();
    void saveToFile();
    void addTimeEntry(const QTime& from = QTime(), const QTime& to = QTime(), int price = 0);
};


#endif // PRICEWINDOW_H
