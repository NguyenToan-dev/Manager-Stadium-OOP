#pragma once

#include <QWidget>
#include <QLabel>

class QLineEdit;
class QPushButton;

class AddFieldWindow : public QWidget {
    Q_OBJECT

public:
    explicit AddFieldWindow(const QString& sport, QWidget* parent = nullptr);
    void setExistingFieldNames(const QList<QString>& existingFieldNames);
    void setEditMode(const QString& fieldName, int fieldPrice, QList<QString>& existingFields);

signals:
    void fieldConfirmed(const QString& name, int price);
    void backToFieldList();

private:
    bool editing;
    QString originalName;
    QString sportName;
    QLineEdit* nameEdit;
    QLineEdit* priceEdit;
    QPushButton* confirmButton;
    QPushButton* backButton;
    QString imagePath;
    QPushButton* selectImageButton;
    QLabel* imagePathLabel;
    QLabel* imagePreviewLabel;
    QList<QString> fieldList;

    void onConfirmClicked();
};
