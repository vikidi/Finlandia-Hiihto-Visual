#include <QWidget>
#include "finlandia.h"
#include "UI/finlandia.h"
#include "ui_finlandia.h"

Finlandia::Finlandia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Finlandia)
{
    ui->setupUi(this);    
}

Finlandia::~Finlandia()
{
    delete ui;
}
