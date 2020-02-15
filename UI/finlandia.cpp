#include <QWidget>
#include <QPlainTextEdit>
#include <iostream>
#include "finlandia.h"
#include "UI/finlandia.h"
#include "ui_finlandia.h"

Finlandia::Finlandia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Finlandia),
    m_DataHandler(new DataHandler),
    allSearches{}
{
    ui->setupUi(this);    
    m_DataHandler->Initialize();
}

Finlandia::~Finlandia()
{
    delete ui;
    delete m_DataHandler;
}

void Finlandia::on_pushButtonNollaKaikki_clicked()
{

}

void Finlandia::on_pushButtoLisaaHaku_clicked()
{
    /*
    std::map<InterfaceFilter::Filters, QString> filter = {
        {InterfaceFilter::YEAR, ui->comboBoxVuosi->currentText()},
        {InterfaceFilter::DISTANCE, ui->comboBoxMatka->currentText()},
        {InterfaceFilter::NAME, ui->textEditUrheilija->toPlainText()}
    };
    */

    ui->listWidgetTehtHaut->addItem(ui->comboBoxVuosi->currentText() + " "
                                    + ui->comboBoxMatka->currentText() + " " +
                                    ui->textEditUrheilija->toPlainText() + "\n");

    std::map<InterfaceFilter::Filters, QString> filter = {
        {InterfaceFilter::NAME, "Mursu Esa"}
    };

    std::vector<std::vector<std::string>> newData = m_DataHandler->getDataWithFilter(filter);

    unsigned long int size = newData.size();
    std::cout << size << std::endl;

    allSearches.push_back(newData);
}

void Finlandia::on_pushButton_clicked()
{
    // Going through all of the added search data:
    for (unsigned int i = 0; i < allSearches.size(); i++){
        // Data added per search:
        std::vector<std::vector<std::string>> data = allSearches.at(i);

        // Going through individual results in a search:
        for (unsigned int j= 0; j < data.size(); j++){
            std::vector<std::string> result = data.at(j);
            QString disp = "";
            for (unsigned int k = 0; k < result.size(); k++){
                // Showcasing a result:
                disp += QString::fromStdString(result.at(k)) + " ";

            }
            ui->listWidgetResult->addItem(disp);
        }
    ui->listWidgetResult->addItem(+ "\n");

    }

   // ui->listWidgetResults->
}
