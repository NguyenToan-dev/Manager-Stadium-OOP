#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QVector>
#include <QPair>
#include <QString>
#include <QScrollArea>

#include "CalendarWindow.h"

class FieldSelectionWindow : public QWidget {
    Q_OBJECT

public:
    FieldSelectionWindow(const QString& sport, QWidget* parent = nullptr);
    QString removeAccents(const QString& input);

private:
    QGridLayout* gridLayout;
    QPushButton* prevButton;
    QPushButton* nextButton;
    QPushButton* backButton;
    QScrollArea* scrollArea;
    QList<QString> existingFields;
    QString selectedSport;

    CalendarWindow* calendarWindow;

    QVector<QPair<QString, int>> fields;
    int currentPage;

    void loadFieldsFromFile();
    void addField(const QString& name, int price);
    void refreshPage();
    void createFieldCard(const QString& name, int price, int row, int col);
    void updateField(const QString& oldName, const QString& newName, int newPrice);
    void renameFieldInBookingFile(const QString& oldName, const QString& newName);
    void renameFieldImage(const QString& oldName, const QString& newName);
    void reloadFields();
    void refreshFieldGrid();
    void deleteField(const QString& name);
    void removeFieldFromBookingFile(const QString& fieldName);
    void deleteFieldImage(const QString& fieldName);
};
