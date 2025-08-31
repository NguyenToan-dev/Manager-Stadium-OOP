// ManageBookedFields.h
#ifndef MANAGEBOOKEDFIELDS_H
#define MANAGEBOOKEDFIELDS_H

#include "Thanh_Toan.h"
#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QString>
#include <QCoreApplication>
#include <QSizePolicy>
#include <QPushButton>
#include <QMessageBox>
#include <QDir>

class ManageBookedFields : public QWidget
{
    Q_OBJECT

public:
    explicit ManageBookedFields(QWidget *parent = nullptr);
    void loadBookings(const QString& filePath);

private:
    QTableWidget* table;
    bool showingHistory;
    void handleDeleteRow(const QString& lineToDelete);
};

#endif // MANAGEBOOKEDFIELDS_H
