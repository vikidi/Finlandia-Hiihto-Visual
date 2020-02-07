#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_dataHandler(new DataHandler),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_dataHandler->Initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_dataHandler;
}
