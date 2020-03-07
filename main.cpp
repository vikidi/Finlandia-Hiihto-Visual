#include "mainwindow.h"
#include "UI/finlandia.h"

#include <QApplication>
#include <QMetaType>
#include <iostream>

#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QPair<QByteArray,QByteArray>>();

    Finlandia f;    
    f.show();
    MainWindow w;
    w.show();

    return a.exec();
}
