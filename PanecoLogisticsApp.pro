QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



CONFIG += c++17

RC_ICONS = app_icon.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminpanel.cpp \
    databasehandler.cpp \
    fineloguser.cpp \
    inputmanager.cpp \
    listitem.cpp \
    main.cpp \
    mainwindow.cpp \
    overlaywidget.cpp \
    protocolform.cpp \
    reportheadline.cpp \
    settingspanel.cpp \
    stylesheetmanipulator.cpp \
    useritem.cpp \
    userpanel.cpp \
    userreport.cpp

HEADERS += \
    adminpanel.h \
    databasehandler.h \
    fineloguser.h \
    inputmanager.h \
    listitem.h \
    mainwindow.h \
    overlaywidget.h \
    protocolform.h \
    reportheadline.h \
    settingspanel.h \
    stylesheetmanipulator.h \
    useritem.h \
    userpanel.h \
    userreport.h

FORMS += \
    adminpanel.ui \
    listitem.ui \
    mainwindow.ui \
    protocolform.ui \
    settingspanel.ui \
    useritem.ui \
    userpanel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    content/paneco-logo.webp

RESOURCES += \
    Images.qrc

android: include(C:/Users/I9/AppData/Local/Android/Sdk/android_openssl/openssl.pri)
android: include(C:/Users/dalac/AppData/Local/Android/Sdk/android_openssl/openssl.pri)
