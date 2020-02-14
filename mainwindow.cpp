#include "mainwindow.h"
#include "ui_mainwindow.h"

// DEBUG
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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_dataHandler;
}

void MainWindow::dataReady()
{
    std::map<InterfaceFilter::Filters, QString> filter = {
        {InterfaceFilter::YEAR, "2017"}
    };

    std::vector<std::vector<std::string>> test = m_dataHandler->getDataWithFilter(filter);
}

void MainWindow::progressChanged(const int progress)
{
    m_progress->setValue(progress);
}
