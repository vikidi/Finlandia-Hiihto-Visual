#include "mainwindow.h"
#include "finlandiamodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qRegisterMetaType<QPair<QByteArray,QByteArray>>();

    qmlRegisterType<Finlandiamodel>("cppbackend", 1, 0, "BackEnd");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    return app.exec();
}
