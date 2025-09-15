QT       += core gui network
#DESTDIR = ./bin
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttons/statebtn.cpp \
    buttons/timebutton.cpp \
    core/HttpMgr.cpp \
    core/TcpMgr.cpp \
    core/userdata.cpp \
    global/global.cpp \
    core/usermgr.cpp \
    widgets/adduseritem.cpp \
    widgets/applyfriend.cpp \
    widgets/applyfrienditem.cpp \
    widgets/applyfriendlist.cpp \
    widgets/applyfriendpage.cpp \
    widgets/bubbleframebase.cpp \
    widgets/chatdialog.cpp \
    widgets/chatmsgbaseitem.cpp \
    widgets/chatpage.cpp \
    widgets/chatview.cpp \
    widgets/contactlist.cpp \
    widgets/imagebubble.cpp \
    widgets/listitembase.cpp \
    widgets/loadingdlg.cpp \
    widgets/logindialog.cpp \
    main.cpp \
    widgets/mainwindow.cpp \
    widgets/registerdialog.cpp \
    widgets/resetdialog.cpp \
    widgets/searchlineedit.cpp \
    widgets/searchlist.cpp \
    widgets/statelabel.cpp \
    widgets/statewidget.cpp \
    widgets/textbubble.cpp \
    widgets/tiplabel.cpp \
    widgets/userchatitem.cpp \
    widgets/userchatlist.cpp \
    widgets/usercontactitem.cpp \
    widgets/userfounddlg.cpp

HEADERS += \
    buttons/statebtn.h \
    buttons/timebutton.h \
    core/HttpMgr.h \
    core/Singleton.h \
    core/TcpMgr.h \
    core/userdata.h \
    global/global.h \
    core/usermgr.h \
    widgets/adduseritem.h \
    widgets/applyfriend.h \
    widgets/applyfrienditem.h \
    widgets/applyfriendlist.h \
    widgets/applyfriendpage.h \
    widgets/bubbleframebase.h \
    widgets/chatdialog.h \
    widgets/chatmsgbaseitem.h \
    widgets/chatpage.h \
    widgets/chatview.h \
    widgets/contactlist.h \
    widgets/imagebubble.h \
    widgets/listitembase.h \
    widgets/loadingdlg.h \
    widgets/logindialog.h \
    widgets/mainwindow.h \
    widgets/registerdialog.h \
    widgets/resetdialog.h \
    widgets/searchlineedit.h \
    widgets/searchlist.h \
    widgets/statelabel.h \
    widgets/statewidget.h \
    widgets/textbubble.h \
    widgets/tiplabel.h \
    widgets/userchatitem.h \
    widgets/userchatlist.h \
    widgets/usercontactitem.h \
    widgets/userfounddlg.h

FORMS += \
    ui/logindialog.ui \
    ui/mainwindow.ui \
    ui/registerdialog.ui \
    ui/resetdialog.ui \
    ui/chatdialog.ui \
    ui/userchatitem.ui \
    ui/chatpage.ui \
    ui/loadingdlg.ui \
    ui/adduseritem.ui \
    ui/userfounddlg.ui \
    ui/applyfriend.ui \
    ui/usercontactitem.ui \
    ui/applyfriendpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/resource.qrc

DISTFILES += \
    config.ini
	
CONFIG(debug, debug|release) {
	DESTDIR = debug
} else {
	DESTDIR = release
}
	
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = ui
RCC_DIR = qr

#copy ini
myfiles.files = $$PWD/config.ini
myfiles.path  = $$DESTDIR
COPIES += myfiles

