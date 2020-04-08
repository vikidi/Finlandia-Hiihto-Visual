#include "predicterwindow.h"
#include "ui_predicterwindow.h"

#include <QDesktopServices>
#include <QUrl>
#include <QPushButton>

PredicterWindow::PredicterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PredicterWindow)
{
    ui->setupUi(this);

    view = new QWebEngineView(this);
    view->setGeometry(10,10,1280,780);
    view->load(QUrl("http://finlandia-hiihto-predicter.herokuapp.com/"));

    connect(ui->btn_browser, &QPushButton::clicked, this,
            &PredicterWindow::openInBrowser);
}

PredicterWindow::~PredicterWindow()
{
    delete view;
    delete ui;
}

void PredicterWindow::openInBrowser()
{
    QDesktopServices::openUrl(QUrl("http://finlandia-hiihto-predicter.herokuapp.com/"));
}
