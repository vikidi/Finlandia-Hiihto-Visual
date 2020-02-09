#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_dataHandler(new DataHandler),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(m_dataHandler, &DataHandler::loadingReady, this, &MainWindow::dataReady);
    m_dataHandler->Initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_dataHandler;
}

void MainWindow::dataReady()
{
    std::map<QString, int> test = m_dataHandler->amountOfSkiers();
}
