#ifndef ADDSPORTWINDOW_H
#define ADDSPORTWINDOW_H

#include <QWidget>
#include <QList>

class QLineEdit;
class QPushButton;
class QLabel;

class AddSportWindow : public QWidget {
    Q_OBJECT

public:
    explicit AddSportWindow(QWidget* parent = nullptr);
    void setExistingSportsNames(const QList<QString>& existing);
    void setEditMode(const QString& sportName, QList<QString>& existing);
    QString removeAccents(const QString& input);

signals:
    void sportConfirmed(const QString& name);
    void backToSportList();

private:
    QLineEdit* nameEdit;
    QLabel* imagePathLabel;
    QLabel* imagePreviewLabel;
    QPushButton* selectImageButton;
    QPushButton* confirmButton;
    QPushButton* backButton;

    QString imagePath;
    QList<QString> sportList;
    QString originalName;
    bool editing;

    void onConfirmClicked();
};

#endif // ADDSPORTWINDOW_H
