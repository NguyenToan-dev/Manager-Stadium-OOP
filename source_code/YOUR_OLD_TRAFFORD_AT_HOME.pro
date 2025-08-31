QT += core gui widgets uitools charts quick printsupport
CONFIG += c++17
CONFIG += lrelease
CONFIG += embed_translations
RC_FILE = app_icon.rc

SOURCES += \
    AddSportWindow.cpp \
    HomePage.cpp \
    ManageBookedFields.cpp \
    ManageSport.cpp \
    PriceWindow.cpp \
    RemoveSport.cpp \
    discountbill.cpp \
    main.cpp \
    mainwindow.cpp \
    AddFieldWindow.cpp \
    BookingInfoWindow.cpp \
    CalendarWindow.cpp \
    FieldSelectionWindow.cpp \
    TimeWindow.cpp \
    AuthManager.cpp \
    Bao_Cao_Thong_Ke.cpp \
    BookingRecord.cpp \
    LoginWindow.cpp \
    SportVenueManager.cpp \
    Bill.cpp \
    Cap_Tai_Khoan.cpp \
    Thanh_Toan.cpp \
    Quan_Ly_Khach_Hang.cpp \
    NhanVien.cpp \
    nhanvienmanager.cpp \
    normalbill.cpp

HEADERS += \
    AddSportWindow.h \
    HomePage.h \
    ManageBookedFields.h \
    ManageSport.h \
    PriceWindow.h \
    RemoveSport.h \
    discountbill.h \
    mainwindow.h \
    AddFieldWindow.h \
    BookingInfoWindow.h \
    CalendarWindow.h \
    FieldSelectionWindow.h \
    TimeWindow.h \
    AuthManager.h \
    Bao_Cao_Thong_Ke.h \
    BookingRecord.h \
    LoginWindow.h \
    SportVenueManager.h \
    Bill.h \
    Cap_Tai_Khoan.h \
    Thanh_Toan.h \
    Quan_Ly_Khach_Hang.h \
    NhanVien.h \
    nhanvienmanager.h \
    normalbill.h

FORMS += \
    HomePage.ui \
    RemoveSport.ui \
    mainwindow.ui \
    SportVenueManager.ui \
    NoiQuyDialog.ui

RESOURCES += \
    resources.qrc

INCLUDEPATH += $$PWD/libsodium-stable/include
LIBS += -L$$PWD/libsodium-stable/lib -lsodium

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
