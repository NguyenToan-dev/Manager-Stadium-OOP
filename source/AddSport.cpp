#include "AddSport.h"
#include "ui_AddSport.h"
#include <QFileDialog>
#include <QMessageBox>


AddSport::AddSport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSport)
{
    ui->setupUi(this);
}

AddSport::~AddSport()
{
    delete ui;
}

void AddSport::on_btnChooseImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Chọn ảnh", "", "Images (*.png *.jpg *.jpeg)");
    if (!fileName.isEmpty()) {
        selectedImagePath = fileName;
        selectedImage.load(fileName);
        ui->labelImage->setPixmap(selectedImage.scaled(100, 100, Qt::KeepAspectRatio));
    }
}

void AddSport::on_btnAdd_clicked()
{

    QString name = getSportName();
    QString imagePath = selectedImagePath;

    if (name.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập tên môn thể thao.");
        return;
    }

    QDir dir;
    if (!dir.exists("data")) {
        dir.mkpath("data");
    }

    QFile file("data/Sports.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << name << "," << imagePath << "\n";
        file.close();
    } else {
        qDebug() << "Không thể mở file data/Sports.txt để ghi";
    }

    accept(); // Đóng dialog và trả kết quả
}

QString AddSport::getSportName() const
{
    return ui->lineEditName->text();
}

QPixmap AddSport::getSportImage() const
{
    return selectedImage;
}

void AddSport::on_btnCancel_clicked()
{
    reject();
}

