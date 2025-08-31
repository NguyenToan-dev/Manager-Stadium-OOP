#ifndef ADDSPORT_H
#define ADDSPORT_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class AddSport;
}

class AddSport : public QDialog
{
    Q_OBJECT

public:
    explicit AddSport(QWidget *parent = nullptr);
    ~AddSport();

    QString getSportName() const;
    QPixmap getSportImage() const;

private slots:
    void on_btnChooseImage_clicked();
    void on_btnAdd_clicked();

    void on_btnCancel_clicked();

private:
    Ui::AddSport *ui;
    QPixmap selectedImage;
    QString selectedImagePath;
};

#endif // ADDSPORT_H
