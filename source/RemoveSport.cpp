#include "RemoveSport.h"
#include "ui_RemoveSport.h"
#include <QFile>
#include <QTextStream>
#include <QCheckBox>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>

RemoveSport::RemoveSport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoveSport)
{
    ui->setupUi(this);

    layoutSportList = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layoutSportList->setAlignment(Qt::AlignTop);
    layoutSportList->setSpacing(10);

    loadSportList();

    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->btnDelete, &QPushButton::clicked, this, &RemoveSport::on_btnDelete_clicked);
}

RemoveSport::~RemoveSport()
{
    delete ui;
}

void RemoveSport::loadSportList()
{
    QFile file("data/Sports.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Không thể mở file data/Sports.txt";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(",");
        QString name = parts.value(0).trimmed();

        if (name.isEmpty()) continue;

        QCheckBox *check = new QCheckBox(name);
        layoutSportList->addWidget(check);
        checkboxes.append(check);
    }

    file.close();
}

void RemoveSport::on_btnDelete_clicked()
{
    QFile file("data/Sports.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Lỗi", "Không thể mở file để đọc.");
        return;
    }

    QTextStream in(&file);
    QStringList allLines;
    while (!in.atEnd()) {
        allLines << in.readLine();
    }
    file.close();

    // Lấy danh sách tên cần xóa
    QStringList namesToRemove;
    for (QCheckBox *check : checkboxes) {
        if (check->isChecked()) {
            namesToRemove << check->text();
        }
    }

    if (namesToRemove.isEmpty()) {
        QMessageBox::information(this, "Thông báo", "Bạn chưa chọn môn nào để xóa.");
        return;
    }

    // Lọc ra các dòng còn lại
    QStringList remainingLines;
    for (const QString &line : allLines) {
        QStringList parts = line.split(",");
        QString name = parts.value(0).trimmed();
        if (!namesToRemove.contains(name)) {
            remainingLines << line;
        }
    }

    saveRemainingSports(remainingLines);

    QMessageBox::information(this, "Thành công", "Đã xóa các môn thể thao đã chọn.");
    accept(); // Đóng dialog sau khi xóa xong
}

void RemoveSport::saveRemainingSports(const QStringList &remainingLines)
{
    QFile file("data/Sports.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Lỗi", "Không thể mở file để ghi.");
        return;
    }

    QTextStream out(&file);
    for (const QString &line : remainingLines) {
        out << line << "\n";
    }

    file.close();
}
