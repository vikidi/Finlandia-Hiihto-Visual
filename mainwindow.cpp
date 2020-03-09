#include "mainwindow.h"
#include "ui_mainwindow.h"

// DEBUG
#include <QDebug>

#include "interfacefilter.h"

MainWindow::MainWindow(Finlandia* finlandiaUI, InternetExplorers::DataHandler* dh, QWidget *parent) :
    QMainWindow(parent),
    m_finlandiaUI(finlandiaUI),
    m_dataHandler(dh),
    m_progress(new QProgressBar(this)),
    m_view(new QGraphicsView),
    m_scene(new InternetExplorers::GameScene),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(m_dataHandler, &InternetExplorers::DataHandler::loadingReady, this, &MainWindow::dataReady);
    m_dataHandler->Initialize();

    // For progress
    connect(m_dataHandler, &InternetExplorers::DataHandler::progressChanged, this, &MainWindow::progressChanged);
    connect(m_dataHandler, &InternetExplorers::DataHandler::progressChanged,
            m_scene, &InternetExplorers::GameScene::updateProgress);

    // Remove toolbar
    QList<QToolBar *> allToolBars = this->findChildren<QToolBar *>();
    foreach(QToolBar *tb, allToolBars) {
        this->removeToolBar(tb);
    }

    ui->haunAloitusNappi->setDisabled(true);

    m_view->setScene(m_scene);
    ui->gridLayout->addWidget(m_view);
    m_view->verticalScrollBar()->setVisible(false);
    m_view->horizontalScrollBar()->setVisible(false);
    m_view->setFixedSize(m_scene->width()+30,m_scene->height()+30);
}

MainWindow::~MainWindow()
{
    delete m_scene;
    delete m_view;
    delete ui;
}

void MainWindow::dataReady()
{
    ui->haunAloitusNappi->setDisabled(false);

    std::vector<std::vector<std::string>> test;

    // OK
    std::map<InternetExplorers::InterfaceFilter::ValueFilters, QString> filter = {
        {InternetExplorers::InterfaceFilter::YEAR, "2017"}
    };

    // CLOCKING
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    try {
       test = m_dataHandler->getDataWithFilter(filter);
    } catch (InternetExplorers::FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // CLOCKING
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Success, YEAR " << time_span.count() << " s" << std::endl;

    // OK, Mursu vaan painaa
    filter = {
        {InternetExplorers::InterfaceFilter::ValueFilters::YEAR, "2017"},
        {InternetExplorers::InterfaceFilter::ValueFilters::DISTANCE, "P100"},
        {InternetExplorers::InterfaceFilter::ValueFilters::NAME, "Mursu Esa"}
    };

    // CLOCKING
    t1 = high_resolution_clock::now();

    try {
        test = m_dataHandler->getDataWithFilter(filter);
    } catch (InternetExplorers::FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    // CLOCKING
    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Success, YEAR, DISTANCE, NAME " << time_span.count() << " s" << std::endl;
}

void MainWindow::progressChanged(const int progress)
{
    m_progress->setValue(progress);
}

void MainWindow::on_haunAloitusNappi_clicked()
{
    m_finlandiaUI->show();
    this->close();
}
