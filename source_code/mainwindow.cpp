#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ManageSport.h"
#include "HomePage.h"
#include "ManageBookedFields.h"
#include "Bao_Cao_Thong_Ke.h"
#include "Cap_Tai_Khoan.h"
#include "LoginWindow.h"
#include "Quan_Ly_Khach_Hang.h"
#include "nhanvienmanager.h"
#include "AuthManager.h"

#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <string>
#include <vector>
#include <fstream>
#include <QCoreApplication>
#include <QPropertyAnimation>
#include <QButtonGroup>

using namespace std;

void removePastBookings() {
    ifstream inFile(QCoreApplication::applicationDirPath().toStdString() + "/data/booking.txt");
    if (!inFile.is_open()) return;

    vector<string> validLines;
    vector<string> invalidLines;
    string line;

    QDateTime now = QDateTime::currentDateTime();

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        QStringList parts = QString::fromStdString(line).split('-');
        if (parts.size() < 4) continue;

        QDate date = QDate::fromString(parts[2], "dd/MM/yyyy");
        QTime end = QTime::fromString(parts[4], "hh:mm");

        if (!date.isValid() || !end.isValid()) continue;

        QDateTime endDateTime(date, end);
        if (endDateTime >= now) {
            validLines.push_back(line);
        } else invalidLines.push_back(line);
    }

    inFile.close();

    ofstream outFile(QCoreApplication::applicationDirPath().toStdString() + "/data/booking.txt", ios::trunc);
    for (const auto& validLine : validLines) {
        outFile << validLine << '\n';
    }
    outFile.close();

    ofstream dataFile(QCoreApplication::applicationDirPath().toStdString() + "/data/data.txt", ios::app);
    for (const auto& invalidLine : invalidLines){
        dataFile << invalidLine << '\n';
    }
    dataFile.close();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1425, 600);
    ui->btnCTK->setVisible(false);
    ui->btnBCTK->setVisible(false);

    int role = AuthManager::instance().getCurrentUser().role;
    if (role == 0){
        ui->btnCTK->setVisible(true);
        ui->btnBCTK->setVisible(true);
    } else if (role == 1){
        ui->btnManage->setVisible(false);
        ui->btnBCTK->setVisible(true);
    } else if (role == 2){
        ui->btnManageSport->setVisible(false);
        ui->btnQLKH->setVisible(false);
        ui->btnBookedFields->setVisible(false);
    } else if (role == 3){
        ui->btnManageSport->setVisible(false);
        ui->btnQLNS->setVisible(false);
        ui->btnBookedFields->setVisible(false);
    } else if (role == 4){
        ui->btnBCTK->setVisible(true);
        ui->btnManageSport->setVisible(false);
        ui->btnManage->setVisible(false);
        ui->btnBookedFields->setVisible(false);
    }

    ui->widgetManage->setMaximumHeight(0);
    ui->widgetManage->setVisible(false);

    // Đặt cửa sổ ra giữa màn hình
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    int windowWidth = this->width();
    int windowHeight = this->height();

    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;
    this->move(x, y);

    connect(ui->btnHomePage, &QPushButton::clicked, this, &MainWindow::loadHomePage);
    connect(ui->btnManageSport, &QPushButton::clicked, this, &MainWindow::loadManageSportPage);
    connect(ui->btnBookedFields, &QPushButton::clicked, this, &MainWindow::loadManageBookedFieldsPage);
    connect(ui->btnBCTK, &QPushButton::clicked, this, &MainWindow::loadBCTKPage);
    connect(ui->btnCTK, &QPushButton::clicked, this, &MainWindow::loadCTKPage);
    connect(ui->btnLogOut, &QPushButton::clicked, this, &MainWindow::logOut);
    connect(ui->btnQLKH, &QPushButton::clicked, this, &MainWindow::loadQLKHPage);
    connect(ui->btnQLNS, &QPushButton::clicked, this, &MainWindow::loadQLNSPage);

    removePastBookings();
    loadHomePage(); // Trang mặc định

    ui->btnHomePage->setCheckable(true);
    ui->btnManage->setCheckable(true);
    ui->btnManageSport->setCheckable(true);
    ui->btnBookedFields->setCheckable(true);
    ui->btnQLKH->setCheckable(true);
    ui->btnQLNS->setCheckable(true);
    ui->btnBCTK->setCheckable(true);
    ui->btnCTK->setCheckable(true);
    QButtonGroup *menuGroup = new QButtonGroup(this);
    menuGroup->setExclusive(true);
    menuGroup->addButton(ui->btnHomePage);
    menuGroup->addButton(ui->btnManage);
    menuGroup->addButton(ui->btnManageSport);
    menuGroup->addButton(ui->btnBookedFields);
    menuGroup->addButton(ui->btnQLKH);
    menuGroup->addButton(ui->btnQLNS);
    menuGroup->addButton(ui->btnBCTK);
    menuGroup->addButton(ui->btnCTK);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadHomePage()
{
    QWidget *page = new HomePage(this);
    setCentralWidgetContent(page);
}

void MainWindow::loadManageSportPage()
{
    QWidget *page = new ManageSport(this);
    setCentralWidgetContent(page);
}

void MainWindow::loadManageBookedFieldsPage(){
    QWidget *page = new ManageBookedFields(this);
    setCentralWidgetContent(page);
}

void MainWindow::loadBCTKPage(){
    QWidget *page = new Bao_Cao_Thong_Ke(this);
    setCentralWidgetContent(page);
}

void MainWindow::loadCTKPage(){
    QWidget *page = new Cap_Tai_Khoan(this);
    setCentralWidgetContent(page);
}

void MainWindow::loadQLKHPage(){
    QWidget *page = new Quan_Ly_Khach_Hang(this);
    setCentralWidgetContent(page);
}

void MainWindow::loadQLNSPage(){
    QWidget *page = new NhanVienManager(this);
    setCentralWidgetContent(page);
}

void MainWindow::setCentralWidgetContent(QWidget *widget)
{
    // Xoá layout cũ nếu có
    QLayout *layout = ui->centralArea->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->centralArea);
        ui->centralArea->setLayout(layout);
    }

    while (QLayoutItem *item = layout->takeAt(0)) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    // Cố định kích thước
    QSize fixedSize(1200, 550);
    widget->setFixedSize(fixedSize);
    ui->centralArea->setFixedSize(fixedSize);

    layout->addWidget(widget);
}

void MainWindow::on_btnManage_clicked(){
    static bool quanLyExpanded = false;

    int startHeight, endHeight;

    if (!quanLyExpanded) {
        // Expand
        startHeight = 0;
        endHeight = 150; // đúng với chiều cao thật
        ui->widgetManage->setVisible(true);
    } else {
        // Collapse
        startHeight = ui->widgetManage->height();
        endHeight = 0;
    }

    QPropertyAnimation* anim = new QPropertyAnimation(ui->widgetManage, "maximumHeight");
    anim->setDuration(200);
    anim->setStartValue(startHeight);
    anim->setEndValue(endHeight);

    if (quanLyExpanded) {
        connect(anim, &QPropertyAnimation::finished, this, [=]() {
            ui->widgetManage->setVisible(false);
        });
    }

    anim->start();

    quanLyExpanded = !quanLyExpanded; // Đảo trạng thái ở cuối
}

void MainWindow::logOut(){
    this->close();
    LoginWindow* loginWindow = new LoginWindow();
    loginWindow->show();
    QObject::connect(loginWindow, &LoginWindow::loginSuccess, [=]() {
        loginWindow->close();
        MainWindow* mainWindow = new MainWindow();
        mainWindow->show();
    });
}




