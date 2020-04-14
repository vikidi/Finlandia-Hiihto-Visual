#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "interfacefilter.h"
#include "constants.h"

MainWindow::MainWindow(Finlandia* finlandiaUI, InternetExplorers::DataHandler* dh, QWidget *parent) :
    QMainWindow(parent),
    m_finlandiaUI(finlandiaUI),
    m_dataHandler(dh),
    m_view(new QGraphicsView),
    m_scene(new InternetExplorers::GameScene(true)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(m_dataHandler, &InternetExplorers::DataHandler::loadingReady, this, &MainWindow::dataReady);
    m_dataHandler->Initialize();

    // Connect for progress
    connect(m_dataHandler, &InternetExplorers::DataHandler::progressChanged,
            m_scene, &InternetExplorers::GameScene::updateProgress);

    // Connect for info box
    connect(m_dataHandler, &InternetExplorers::DataHandler::appendInfo,
            this, &MainWindow::appendInfo);

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
}

void MainWindow::appendInfo(const QString text)
{
    std::string now = QDateTime::currentDateTime().toString("hh:mm:ss.zzz").toStdString();
    QString time = QString::fromStdString(now.substr(0, now.size() - 2));
    QString msg = time + "  -  " + text;
    ui->txt_info->append(msg);
}

void MainWindow::on_haunAloitusNappi_clicked()
{
    m_finlandiaUI->show();
    this->close();
}
