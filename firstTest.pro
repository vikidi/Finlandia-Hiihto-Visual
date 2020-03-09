#-------------------------------------------------
#
# Project created by QtCreator 2020-01-23T19:16:10
#
#-------------------------------------------------

QT += core gui network charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = firstTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

CONFIG += openssl-linked

SOURCES += \
    UI/gamescene.cpp \
        main.cpp \
        mainwindow.cpp \
    APIs/finlandiaapi.cpp \
    APIs/localapi.cpp \
    datahandler.cpp \
    APIs/ilmatiedeapi.cpp \
    APIs/finlandiacaller.cpp \
    UI/finlandia.cpp \
    interfacefilter.cpp \
    libraries/simplecrypt.cpp \
    APIs/localdataloader.cpp \
    logger.cpp

HEADERS += \
    UI/gamescene.h \
        mainwindow.h \
    APIs/finlandiaapi.h \
    APIs/localapi.h \
    datahandler.h \
    APIs/ilmatiedeapi.h \
    APIs/finlandiacaller.h \
    UI/finlandia.h \
    interfacefilter.h \
    libraries/simplecrypt.h \
    APIs/localdataloader.h \
    logger.h

FORMS += \
        mainwindow.ui \
    UI/finlandia.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
