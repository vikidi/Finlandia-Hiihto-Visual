#include "mainwindow.h"
#include "finlandiamodel.hh"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQMLContext>
#include <QQmlComponent>

#include <QQuickView>

#include <QApplication>
#include <QMetaType>
#include <iostream>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qRegisterMetaType<QPair<QByteArray,QByteArray>>();

    qmlRegisterType<Finlandiamodel>("cppbackend", 1, 0, "BackEnd");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    return app.exec();
}
