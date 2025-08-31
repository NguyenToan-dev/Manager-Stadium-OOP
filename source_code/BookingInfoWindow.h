#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class BookingInfoWindow : public QWidget {
    Q_OBJECT

public:
    BookingInfoWindow(QWidget* parent = nullptr);

signals:
    void backToTimeSelection();
    void infoConfirmed(const QString& name, const QString& phone);

private slots:
    void onConfirmClicked();

private:
    QLineEdit* nameEdit;
    QLineEdit* phoneEdit;
    QPushButton* confirmButton;
    QPushButton* backButton;
};
