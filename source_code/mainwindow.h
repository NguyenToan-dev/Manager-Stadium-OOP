#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setCentralWidgetContent(QWidget *widget);
    ~MainWindow();

private slots:
    void loadHomePage();
    void loadManageSportPage();
    void loadManageBookedFieldsPage();
    void loadBCTKPage();
    void loadCTKPage();
    void loadQLKHPage();
    void loadQLNSPage();
    void on_btnManage_clicked();
    void logOut();

private:
    Ui::MainWindow *ui;
    bool quanLyExpanded = false;
};

#endif // MAINWINDOW_H
