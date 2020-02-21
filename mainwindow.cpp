#include "mainwindow.h"
#include "ui_mainwindow.h"

// DEBUG
#include "interfacefilter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_dataHandler(new InternetExplorers::DataHandler),
    m_progress(new QProgressBar(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(m_dataHandler, &InternetExplorers::DataHandler::loadingReady, this, &MainWindow::dataReady);
    m_dataHandler->Initialize();

    // For progress
    connect(m_dataHandler, &InternetExplorers::DataHandler::progressChanged, this, &MainWindow::progressChanged);

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

    // OK
    std::map<InternetExplorers::InterfaceFilter::Filters, QString> filter = {
        {InternetExplorers::InterfaceFilter::YEAR, "2017"}
    };

    std::vector<std::vector<std::string>> test = m_dataHandler->getDataWithFilter(filter);

    // EMPTY, not passing filter validation
    filter = {
        {InternetExplorers::InterfaceFilter::YEAR, "2030"}
    };

    test = m_dataHandler->getDataWithFilter(filter);

    // OK
    filter = {
        {InternetExplorers::InterfaceFilter::YEAR_RANGE, "2015;2017"}
    };

    test = m_dataHandler->getDataWithFilter(filter);

    // EMPTY, not passing filter validation
    filter = {
        {InternetExplorers::InterfaceFilter::YEAR_RANGE, "2015;2013"}
    };

    test = m_dataHandler->getDataWithFilter(filter);

    // OK
    filter = {
        {InternetExplorers::InterfaceFilter::DISTANCE, "V20jun"}
    };

    test = m_dataHandler->getDataWithFilter(filter);

    // EMPTY, not passing filter validation
    filter = {
        {InternetExplorers::InterfaceFilter::DISTANCE, "P200"}
    };

    test = m_dataHandler->getDataWithFilter(filter);

    // OK
    filter = {
        {InternetExplorers::InterfaceFilter::NAME, "Mursu Esa"}
    };

    test = m_dataHandler->getDataWithFilter(filter);

    // EMPTY, not passing filter validation
    filter = {
        {InternetExplorers::InterfaceFilter::NAME, "O?."}
    };

    test = m_dataHandler->getDataWithFilter(filter);

    // OK, Mursu vaan painaa
    filter = {
        {InternetExplorers::InterfaceFilter::YEAR, "2017"},
        {InternetExplorers::InterfaceFilter::DISTANCE, "P100"},
        {InternetExplorers::InterfaceFilter::NAME, "Mursu Esa"}
    };

    test = m_dataHandler->getDataWithFilter(filter);

    // EMPTY, not passing filter validation
    filter = {
        {InternetExplorers::InterfaceFilter::YEAR, "2017"},
        {InternetExplorers::InterfaceFilter::YEAR_RANGE, "2015;2017"}
    };

    test = m_dataHandler->getDataWithFilter(filter);
}

void MainWindow::progressChanged(const int progress)
{
    m_progress->setValue(progress);
}

void MainWindow::on_haunAloitusNappi_clicked()
{
    this->close();
}
