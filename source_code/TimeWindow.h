#pragma once

#include <QWidget>
#include <QDate>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QStringList>

class TimeWindow : public QWidget {
    Q_OBJECT

public:
    explicit TimeWindow(const QString& sport, const QString& field, const QDate& date, QWidget* parent = nullptr);
    static QString removeAccents(const QString& input);

signals:
    void backToDateSelection();
    void timeConfirmed();

private slots:
    void onConfirm();

private:
    QDate selectedDate;
    QString selectedField;
    QString selectedSport;
    QComboBox* startCombo;
    QComboBox* endCombo;
    QPushButton* confirmButton;
    QPushButton* backButton;
    QList<QPair<QTime, QTime>> bookedSlots;

    QStringList generateTimeList(const QTime& minTime, const QTime& maxTime);
    void loadBookedSlots();
    bool isBooked(const QTime& time) const;
    void updateStartCombo();
    void updateEndCombo(const QString& startTimeStr);
};
