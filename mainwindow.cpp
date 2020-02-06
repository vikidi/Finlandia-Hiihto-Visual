#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "APIs/finlandiaapi.h"
#include "APIs/localapi.h"

#include <iostream>
#include <QDebug>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QNetworkProxy>
#include <QFile>
#include <string>
#include <sstream>
#include <vector>
#include <QXmlStreamReader>
#include <ctime>
#include <ratio>
#include <chrono>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadData()
{
    std::thread t(&MainWindow::loadInThread, this);
    t.detach();
}

void MainWindow::loadInThread()
{
    // CLOCKING
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    FinlandiaAPI api = FinlandiaAPI();
    //std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > data = api.loadAllData();

    // CLOCKING
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Data fetch done in " << time_span.count() << " s" << std::endl;

    LocalAPI localAPI = LocalAPI();
    //localAPI.saveData(data);

    std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > data = localAPI.loadData();

    // CLOCKING
    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    duration<double> time_span2 = duration_cast<duration<double>>(t3 - t2);
    std::cout << "Data save done in " << time_span2.count() << " s" << std::endl;
}
