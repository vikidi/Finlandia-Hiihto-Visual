#include "mainwindow.h"
#include "UI/finlandia.h"
#include "datahandler.h"

#include <QApplication>
#include <QMetaType>
#include <iostream>

/*!
 * \brief
 * Our project groups root namespace
 */
namespace InternetExplorers {
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QPair<QByteArray,QByteArray>>();

    InternetExplorers::DataHandler dh;
    Finlandia f(&dh);
    MainWindow w(&f, &dh);
    w.show();

    return a.exec();
}
