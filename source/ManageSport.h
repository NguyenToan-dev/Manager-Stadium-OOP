#ifndef MANAGESPORT_H
#define MANAGESPORT_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QStringList>

class ManageSport : public QWidget
{
    Q_OBJECT

public:
    explicit ManageSport(QWidget *parent = nullptr);

private:
    QGridLayout* gridLayout;
    QScrollArea* scrollArea;
    QStringList existingSports;

    void loadSportsFromFile();
    void createSportCard(const QString& name, int row, int col);
    void reloadSports();
    void clearGridLayout();

    void renameSportImage(const QString& oldName, const QString& newName);
    void updateSport(const QString& oldName, const QString& newName);
};

#endif // MANAGESPORT_H
