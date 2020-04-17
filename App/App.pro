#-------------------------------------------------
#
# Project created by QtCreator 2020-01-23T19:16:10
#
#-------------------------------------------------

QT += core gui network charts webenginewidgets multimedia
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
    UI/encryptionsettingswindow.cpp \
    UI/gamescene.cpp \
    crypter.cpp \
        main.cpp \
        mainwindow.cpp \
    APIs/finlandiaapi.cpp \
    APIs/localapi.cpp \
    datahandler.cpp \
    APIs/finlandiacaller.cpp \
    UI/finlandia.cpp \
    interfacefilter.cpp \
    libraries/simplecrypt.cpp \
    APIs/localdataloader.cpp \
    logger.cpp \
    dataorderer.cpp \
    UI/predicterwindow.cpp \
    helper.cpp

HEADERS += \
    UI/encryptionsettingswindow.h \
    UI/gamescene.h \
    crypter.h \
        mainwindow.h \
    APIs/finlandiaapi.h \
    APIs/localapi.h \
    datahandler.h \
    APIs/finlandiacaller.h \
    UI/finlandia.h \
    interfacefilter.h \
    libraries/simplecrypt.h \
    APIs/localdataloader.h \
    logger.h \
    constants.h \
    dataorderer.h \
    UI/predicterwindow.h \
    helper.h

FORMS += \
    UI/encryptionsettingswindow.ui \
        mainwindow.ui \
    UI/finlandia.ui \
    UI/predicterwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

RCC_BINARY_SOURCES += music.qrc

asset_builder.commands = $$[QT_HOST_BINS]/rcc -binary ${QMAKE_FILE_IN} -o ${QMAKE_FILE_OUT} -no-compress
asset_builder.depend_command = $$[QT_HOST_BINS]/rcc -list $$QMAKE_RESOURCE_FLAGS ${QMAKE_FILE_IN}
asset_builder.input = RCC_BINARY_SOURCES
asset_builder.output = $$OUT_PWD/$$DESTDIR/${QMAKE_FILE_IN_BASE}.qrb
asset_builder.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += asset_builder

OTHER_FILES += $$RCC_BINARY_SOURCES
