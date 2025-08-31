#ifndef REMOVESPORT_H
#define REMOVESPORT_H

#include <QDialog>
#include <QVBoxLayout>
#include <QList>
#include <QCheckBox>

namespace Ui {
class RemoveSport;
}

class RemoveSport : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveSport(QWidget *parent = nullptr);
    ~RemoveSport();

private slots:
    void on_btnDelete_clicked();

private:
    Ui::RemoveSport *ui;
    QVBoxLayout *layoutSportList;
    QList<QCheckBox*> checkboxes; // để dễ truy xuất trạng thái
    void loadSportList();
    void saveRemainingSports(const QStringList &remainingLines);
};

#endif // REMOVESPORT_H
