#include <QApplication>
#include <QSplashScreen>
#include <QStyleFactory>
#include <QCoreApplication>
#include <QLocale>
#include <QIcon>
#include "LoginWindow.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication::setStyle("Fusion");
    QApplication a(argc, argv);
    QLocale::setDefault(QLocale::Vietnamese);

    // Set logo cho app (hiện trên taskbar + titlebar)
    a.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/images/logo.jpg"));

    // Splash screen với logo
    QPixmap logoPixmap(QCoreApplication::applicationDirPath() + "/images/logo.jpg");
    QSplashScreen splash(logoPixmap.scaled(400, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    splash.showMessage("Đang khởi động...", Qt::AlignBottom | Qt::AlignCenter, Qt::black);
    splash.show();

    // Tạo login window
    LoginWindow loginWindow;

    // Splash sẽ tự động đóng khi loginWindow show
    splash.finish(&loginWindow);
    loginWindow.show();

    // Xử lý đăng nhập thành công
    QObject::connect(&loginWindow, &LoginWindow::loginSuccess, [&]() {
        loginWindow.close();
        MainWindow* mainWindow = new MainWindow();
        mainWindow->show();
    });

    return a.exec();
}
