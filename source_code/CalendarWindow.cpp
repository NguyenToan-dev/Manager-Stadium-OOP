// CalendarWindow.cpp
#include "CalendarWindow.h"
#include "TimeWindow.h"
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QDebug>
#include <fstream>
#include <QCoreApplication>
#include <QMessageBox>

using namespace std;

CalendarWindow::CalendarWindow(const QString& sport, const QString& field, QWidget *parent) :
    selectedField(field), QWidget(parent), selectedSport(sport) {
    setWindowTitle("Chọn ngày đặt");
    setFixedSize(400, 460);

    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(10);

    selectedLabel = new QLabel("Ngày đang chọn: ");
    warningLabel = new QLabel;
    warningLabel->setStyleSheet("color: #d32f2f; font-style: italic; font-size: 14px;");
    calendar = new QCalendarWidget;
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    selectedDate = QDate::currentDate();
    calendar->setSelectedDate(selectedDate);
    selectedLabel->setText("Ngày đang chọn: " + selectedDate.toString("dd/MM/yyyy"));

    highlightFullyBookedDates();

    bool fullyBooked = false;
    for (const QDate& d : fullyBookedDates)
        if (d == selectedDate)
            fullyBooked = true;
    if (fullyBooked)
        warningLabel->setText("Ngày đang chọn hiện đang kín giờ!");

    confirmButton = new QPushButton("Xác nhận");
    confirmButton->setObjectName("confirmButton"); // Added to match stylesheet ID
    backButton = new QPushButton("← Quay lại");
    backButton->setObjectName("backButton"); // Added to match stylesheet ID

    layout->addWidget(selectedLabel);
    layout->addWidget(calendar);

    auto* warningLayout = new QHBoxLayout;
    warningLayout->addWidget(warningLabel, 0, Qt::AlignLeft);

    auto* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(backButton, 0, Qt::AlignLeft);
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton, 0, Qt::AlignRight);

    layout->addLayout(warningLayout);
    layout->addLayout(buttonLayout);

    setStyleSheet(R"(
        /* === Main Widget Background === */
        QWidget {
            background-color: #3e4a59;
        }

        /* === Labels === */
        QLabel {
            background-color: transparent;
            color: #bdc3c7;
            font-size: 13px;
            font-weight: 500;
        }

        /* === Calendar Widget === */
        QCalendarWidget {
            background-color: #2c3e50;
            border: 1px solid #4a637d;
            border-radius: 10px;
        }

        QCalendarWidget QAbstractItemView {
            selection-background-color: #0d6efd;
            selection-color: white;
            background-color: #34495e;
            font-size: 14px;
            color: #c5d4e3;
            gridline-color: #4a637d;
        }

        QCalendarWidget QComboBox {
            background-color: #34495e;
            color: #c5d4e3;
            border: 1px solid #4a637d;
            border-radius: 4px;
            padding: 2px 6px;
            font-size: 14px;
            font-weight: 600;
        }
        QCalendarWidget QComboBox::drop-down {
            border: none;
            width: 18px;
        }
        QCalendarWidget QComboBox QAbstractItemView {
            background-color: #2c3e50;
            color: #ecf0f1;
            selection-background-color: #0d6efd;
            selection-color: white;
            border: 1px solid #4a637d;
        }

        QCalendarWidget QToolButton {
            color: #c5d4e3;
            font-size: 16px;
            font-weight: bold;
        }

        QCalendarWidget QSpinBox {
            color: #e0e0e0;
            font-size: 14px;
            background-color: #34495e;
            border: 1px solid #4a637d;
            border-radius: 4px;
        }

        /* === Warning Label Specific Styling (Override) === */
        QLabel[styleSheet~="color: #d32f2f"] {
            color: #e74c3c; /* Adjusted to fit dark theme */
            font-style: italic;
            font-size: 14px;
        }

        /* === Button Base Style === */
        QPushButton {
            padding: 10px 20px;
            font-size: 14px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
        }

        /* === Primary Action Button (Confirm) === */
        #confirmButton {
            background-color: #0d6efd;
            color: white;
        }
        #confirmButton:hover {
            background-color: #0b5ed7;
        }

        /* === Secondary Button (Back) === */
        #backButton {
            background-color: #34495e;
            color: #e0e0e0;
            border: 1px solid #4a637d;
        }
        #backButton:hover {
            background-color: #4a637d;
        }
    )");

    connect(calendar, &QCalendarWidget::clicked, this, &CalendarWindow::onDateClicked);
    connect(confirmButton, &QPushButton::clicked, this, &CalendarWindow::onConfirm);
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit backToFieldSelection();
        this->close();
    });

    QDate today = QDate::currentDate();
    QTextCharFormat pastFormat;
    pastFormat.setForeground(QBrush(Qt::gray));
    pastFormat.setFontItalic(true);

    QDate date = QDate(today.year(), today.month() - 1, 21);
    while (date < today) {
        calendar->setDateTextFormat(date, pastFormat);
        date = date.addDays(1);
    }

    calendar->setMinimumDate(today);
}

void CalendarWindow::onDateClicked(const QDate &date) {
    selectedDate = date;
    selectedLabel->setText("Ngày đang chọn: " + date.toString("dd/MM/yyyy"));
    bool fullyBooked = false;
    for (const QDate& d : fullyBookedDates)
        if (d == selectedDate)
            fullyBooked = true;
    if (fullyBooked)
        warningLabel->setText("Ngày đang chọn hiện đang kín giờ!");
    else warningLabel->setText("");
}

void CalendarWindow::highlightFullyBookedDates(){
    string filePath = QCoreApplication::applicationDirPath().toStdString() + "/data/booking.txt";
    ifstream file(filePath);
    bool skip = false;
    QDate lastDate;
    QTime lastEnd;
    string line;
    while (getline(file, line)) {
        QStringList parts = QString::fromStdString(line).split('-');
        if (parts.size() < 5) continue;

        QString sportName = parts[0].trimmed();
        QString fieldName = parts[1].trimmed();
        if (fieldName != selectedField || sportName != selectedSport) continue;

        QDate date = QDate::fromString(parts[2], "dd/MM/yyyy");
        QTime start = QTime::fromString(parts[3], "hh:mm");
        QTime end = QTime::fromString(parts[4], "hh:mm");

        if (date != lastDate){
            lastDate = date;
            if ((start >= QTime(6, 0) && lastDate != QDate::currentDate()) || (start >= QTime::currentTime().addSecs(3600) && lastDate == QDate::currentDate()))
                skip = true;
            else {
                skip = false;
                lastDate = date;
                lastEnd = end;
            }
        }
        if (!skip){
            if (lastEnd.addSecs(1800) >= start){
                lastEnd = end;
            }
            else
                skip = true;
            if (lastEnd >= QTime(21, 30))
                fullyBookedDates.append(lastDate);
        }
    }
    file.close();

    if (QTime::currentTime() > QTime(19, 0))
        fullyBookedDates.append(QDate::currentDate());

    QTextCharFormat fullyBookedFormat;
    fullyBookedFormat.setForeground(QBrush(Qt::gray));
    fullyBookedFormat.setFontItalic(true);

    for (const QDate& date : fullyBookedDates){
        calendar->setDateTextFormat(date, fullyBookedFormat);
    }
}

void CalendarWindow::onConfirm() {
    bool fullyBooked = false;
    for (const QDate& date : fullyBookedDates)
        if (date == selectedDate)
            fullyBooked = true;
    if (selectedDate.isValid() && selectedDate >= QDate::currentDate() && !fullyBooked) {
        this->hide();

        timeWindow = new TimeWindow(selectedSport, selectedField, selectedDate);
        timeWindow->setAttribute(Qt::WA_DeleteOnClose);

        connect(timeWindow, &TimeWindow::timeConfirmed, this, [this]() {
            emit calendarConfirmed();
            this->close();
        });

        connect(timeWindow, &TimeWindow::backToDateSelection, this, [this]() {
            this->show();
        });

        timeWindow->show();
    }
}
