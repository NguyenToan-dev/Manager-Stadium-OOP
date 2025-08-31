#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLineEdit>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QWidget>
#include <QTextStream>
#include <QFileDialog>
#include <QHeaderView>
#include <QDate>
#include <QLabel>

class Quan_Ly_Khach_Hang : public QMainWindow
{
    Q_OBJECT

public:
    Quan_Ly_Khach_Hang(QWidget *parent = nullptr);
    ~Quan_Ly_Khach_Hang();

private slots:
    void onSearchCustomer();
    void onBackButtonClicked();

private:
    // UI components
    QTableWidget *tableWidget;
    QPushButton *btnSearch;
    QPushButton *btnExport;
    QPushButton *btnBack;
    QLineEdit *lineSearch;
    QStatusBar *statusBar;
    QWidget *detailWidget;
    QLabel *lblName;
    QLabel *lblPhone;
    QLabel *lblCount;
    QLabel *lblStatus;
    QLabel *titleLabel; // Thêm label tiêu đề
    QTableWidget *historyTable; // Bảng lịch sử đặt sân
    QWidget* searchContainer = nullptr;

    // Data
    QStandardItemModel *m_model;

    // Functions
    void applyDarkTheme();
    void setupUI();
    void loadVIPData();
    void showMessage(const QString &message);
    void displayVIPCustomers(const QList<QPair<QString, QPair<QString, int>>> &vipCustomers);
    void exportCustomerListToCSV();
};
