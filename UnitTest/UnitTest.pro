QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TARGET = UnitTest

CONFIG += c++17

TEMPLATE = app

SOURCES += \
    tst_interfacefilter.cpp \
    ../App/interfacefilter.cpp \

HEADERS += \
    ../App/interfacefilter.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../App \
DEPENDPATH  += ../App \
