#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QString>
#include <QList>

class HomePage : public QWidget {
    Q_OBJECT
public:
    explicit HomePage(QWidget* parent = nullptr);

private:
    QTableWidget* table;

    void loadBookings();
    void readBookingsFromFile(const QString &filePath, const QString &currentDateStr,
                              QList<QPair<QStringList, QString>> &bookings);
    void sortAndDisplayBookings(const QList<QPair<QStringList, QString>>& bookings);
    void openNoiQuyDialog();
};

#endif // HOMEPAGE_H
