#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_dataHandler = new DataHandler();
    m_dataHandler->Initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_dataHandler;
}
