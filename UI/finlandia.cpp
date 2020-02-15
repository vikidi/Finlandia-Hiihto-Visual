#include <QWidget>
 #include <QPlainTextEdit>
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

void Finlandia::on_pushButtonNollaKaikki_clicked()
{

}

void Finlandia::on_pushButtoLisaaHaku_clicked()
{
    std::map<InterfaceFilter::Filters, QString> filter = {
        {InterfaceFilter::YEAR, ui->comboBoxVuosi->currentText()},
        {InterfaceFilter::DISTANCE, ui->comboBoxMatka->currentText()},
        {InterfaceFilter::NAME, ui->textEditUrheilija->toPlainText()}
    };

    ui->listWidgetTehtHaut->addItem(ui->comboBoxVuosi->currentText() + " "
                                    + ui->comboBoxMatka->currentText() + " " +
                                    ui->textEditUrheilija->toPlainText() + "\n");


}
