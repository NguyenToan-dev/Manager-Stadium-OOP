#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include "LoginWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QPixmap splashImg("D:/LABOOP/SportVenueManager/image/splash.png");
    QSplashScreen splash(splashImg);
    splash.show();
    splash.showMessage("Đang khởi động...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);

    LoginWindow loginWindow;

    QTimer::singleShot(2000, [&]() {
        splash.close();
        loginWindow.show();
        });

    return a.exec();
}
