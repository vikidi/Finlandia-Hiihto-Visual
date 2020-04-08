#include "predicterwindow.h"
#include "ui_predicterwindow.h"

PredicterWindow::PredicterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PredicterWindow)
{
    ui->setupUi(this);

    view = new QWebEngineView(this);
    view->setGeometry(10,10,1280,780);
    view->load(QUrl("http://finlandia-hiihto-predicter.herokuapp.com/"));
}

PredicterWindow::~PredicterWindow()
{
    delete view;
    delete ui;
}
