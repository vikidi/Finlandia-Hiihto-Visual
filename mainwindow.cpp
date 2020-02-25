#include "mainwindow.h"
#include "ui_mainwindow.h"

// DEBUG
#include <QDebug>

#include "interfacefilter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_dataHandler(new DataHandler),
    m_progress(new QProgressBar(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(m_dataHandler, &DataHandler::loadingReady, this, &MainWindow::dataReady);
    m_dataHandler->Initialize();

    // For progress
    connect(m_dataHandler, &DataHandler::progressChanged, this, &MainWindow::progressChanged);

    // Remove toolbar
    QList<QToolBar *> allToolBars = this->findChildren<QToolBar *>();
    foreach(QToolBar *tb, allToolBars) {
        this->removeToolBar(tb);
    }

    ui->haunAloitusNappi->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_dataHandler;
}

void MainWindow::dataReady()
{
 //    std::map<QString, int> test = m_dataHandler->amountOfSkiers();
    ui->haunAloitusNappi->setDisabled(false);

    std::vector<std::vector<std::string>> test;

    // OK
    std::map<InterfaceFilter::ValueFilters, QString> filter = {
        {InterfaceFilter::ValueFilters::YEAR, "2017"}
    };

    try {
        m_dataHandler->getDataWithFilter(filter);
    } catch (FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // EMPTY, not passing filter validation
    filter = {
        {InterfaceFilter::ValueFilters::YEAR, "2030"}
    };

    try {
        test = m_dataHandler->getDataWithFilter(filter);
    } catch (FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // OK
    filter = {
        {InterfaceFilter::ValueFilters::YEAR_RANGE, "2015;2017"}
    };

    try {
        m_dataHandler->getDataWithFilter(filter);
    } catch (FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // EMPTY, not passing filter validation
    filter = {
        {InterfaceFilter::ValueFilters::YEAR_RANGE, "2015;2013"}
    };

    try {
        m_dataHandler->getDataWithFilter(filter);
    } catch (FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // OK
    filter = {
        {InterfaceFilter::ValueFilters::DISTANCE, "V20jun"}
    };

    try {
        m_dataHandler->getDataWithFilter(filter);
    } catch (FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // EMPTY, not passing filter validation
    filter = {
        {InterfaceFilter::ValueFilters::DISTANCE, "P200"}
    };

    try {
        m_dataHandler->getDataWithFilter(filter);
    } catch (FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // OK
    filter = {
        {InterfaceFilter::ValueFilters::NAME, "Mursu Esa"}
    };

    try {
        m_dataHandler->getDataWithFilter(filter);
    } catch (FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // EMPTY, not passing filter validation
    filter = {
        {InterfaceFilter::ValueFilters::NAME, "O?."}
    };

    try {
        m_dataHandler->getDataWithFilter(filter);
    } catch (FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // OK, Mursu vaan painaa
    filter = {
        {InterfaceFilter::ValueFilters::YEAR, "2017"},
        {InterfaceFilter::ValueFilters::DISTANCE, "P100"},
        {InterfaceFilter::ValueFilters::NAME, "Mursu Esa"}
    };

    try {
        m_dataHandler->getDataWithFilter(filter);
    } catch (FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // EMPTY, not passing filter validation
    filter = {
        {InterfaceFilter::ValueFilters::YEAR, "2017"},
        {InterfaceFilter::ValueFilters::YEAR_RANGE, "2015;2017"}
    };

    try {
        m_dataHandler->getDataWithFilter(filter);
    } catch (FilterException &e) {
        std::cout << e.what() << std::endl;
    }
}

void MainWindow::progressChanged(const int progress)
{
    m_progress->setValue(progress);
}

void MainWindow::on_haunAloitusNappi_clicked()
{
    this->close();
}
