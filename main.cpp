#include "mainwindow.h"
#include "finlandiamodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "UI/finlandia.h"
#include "datahandler.h"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qRegisterMetaType<QPair<QByteArray,QByteArray>>();


    qmlRegisterType<Finlandiamodel>("cppbackend", 1, 0, "BackEnd");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    InternetExplorers::DataHandler dh;
    Finlandia f(&dh);
    MainWindow w(&f, &dh);
    w.show();


    return app.exec();
}
