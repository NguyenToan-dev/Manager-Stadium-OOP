#pragma once

#include <QWidget>
#include <QCalendarWidget>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <QList>
#include <QString>
#include "TimeWindow.h"

class TimeWindow;

class CalendarWindow : public QWidget {
    Q_OBJECT

public:
    CalendarWindow(const QString& sport, const QString& field, QWidget *parent = nullptr);

signals:
    void backToFieldSelection();
    void calendarConfirmed();

private slots:
    void onDateClicked(const QDate &date);
    void onConfirm();

private:
    QCalendarWidget *calendar;
    QString selectedField;
    QString selectedSport;
    QLabel *selectedLabel;
    QLabel *warningLabel;
    QPushButton *confirmButton;
    QPushButton *backButton;
    QDate selectedDate;
    QList<QDate> fullyBookedDates;
    TimeWindow *timeWindow = nullptr;

    void highlightFullyBookedDates();
};
