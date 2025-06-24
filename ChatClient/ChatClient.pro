QT       += core gui network
#DESTDIR = ./bin
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/HttpMgr.cpp \
    global/global.cpp \
    widgets/logindialog.cpp \
    main.cpp \
    widgets/mainwindow.cpp \
    widgets/registerdialog.cpp

HEADERS += \
    core/HttpMgr.h \
    core/Singleton.h \
    global/global.h \
    widgets/logindialog.h \
    widgets/mainwindow.h \
    widgets/registerdialog.h

FORMS += \
    ui/logindialog.ui \
    ui/mainwindow.ui \
    ui/registerdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/resource.qrc

DISTFILES += \
    config.ini
