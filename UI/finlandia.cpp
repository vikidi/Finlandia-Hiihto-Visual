#include <QWidget>
#include <QPlainTextEdit>
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
    std::map<InterfaceFilter::Filters, QString> filter = {
        {InterfaceFilter::YEAR, ui->comboBoxVuosi->currentText()},
        {InterfaceFilter::DISTANCE, ui->comboBoxMatka->currentText()},
        {InterfaceFilter::NAME, ui->textEditUrheilija->toPlainText()}
    };

    ui->listWidgetTehtHaut->addItem(ui->comboBoxVuosi->currentText() + " "
                                    + ui->comboBoxMatka->currentText() + " " +
                                    ui->textEditUrheilija->toPlainText() + "\n");

    std::vector<std::vector<std::string>> newData = m_DataHandler->getDataWithFilter(filter);

    allSearches.push_back(newData);
}

void Finlandia::on_pushButton_clicked()
{
    // Going through all of the added search data:
    for (int i = 0; i < allSearches.size(); i++){
        // Data added per search:
        std::vector<std::vector<std::string>> data = allSearches.at(i);
/*

        // Going through individual results in a search:
        for (auto result; j < allSearches.at(i); j++){
            std::vector<std::string> result = data.at(j);
*/

            // Showcasing a result:
/*            ui->listWidgetResults->addItem(
                                           );
*/
      //  }
     }

   // ui->listWidgetResults->
}
