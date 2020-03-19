#include <QWidget>
#include <QtCharts>
#include <QPlainTextEdit>
#include <iostream>
#include <QDebug>
#include "finlandia.h"
#include "UI/finlandia.h"
#include "ui_finlandia.h"
#include <QtCharts>

Finlandia::Finlandia(InternetExplorers::DataHandler* dh,
                     QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Finlandia),
    m_DataHandler(dh),
    allSearches{},
    m_chart(new QChart())
{
    ui->setupUi(this);
    //m_DataHandler->Initialize();

}

Finlandia::~Finlandia()
{
    delete ui;
}

void Finlandia::on_pushButtonNollaKaikki_clicked()
{
    m_chart -> removeAllSeries();
    ui->listWidgetTehtHaut->clear();
}

void Finlandia::on_pushButtoLisaaHaku_clicked()
{
    std::map<InternetExplorers::InterfaceFilter::ValueFilters, QString> filter;

    if(ui->comboBoxVuosi->currentText() != "Kaikki Vuodet"){
        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> year_pair(
                InternetExplorers::InterfaceFilter::YEAR, ui->comboBoxVuosi->currentText());
        filter.insert(year_pair);
    }

    if(ui->textEditUrheilija->toPlainText() != ""){
        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> name_pair(
                InternetExplorers::InterfaceFilter::NAME, ui->textEditUrheilija->toPlainText());
        filter.insert(name_pair);
    }

    if(ui->comboBoxMatka->currentText() != "Kaikki Matkat"){
        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> distance_pair(
                InternetExplorers::InterfaceFilter::DISTANCE, ui->comboBoxMatka->currentText());
        filter.insert(distance_pair);
    }
    /*std::map<InternetExplorers::InterfaceFilter::ValueFilters, QString> filter = {
        {InternetExplorers::InterfaceFilter::YEAR, ui->comboBoxVuosi->currentText()},
        {InternetExplorers::InterfaceFilter::DISTANCE, ui->comboBoxMatka->currentText()},
        {InternetExplorers::InterfaceFilter::NAME, ui->textEditUrheilija->toPlainText()}
    };*/


    ui->listWidgetTehtHaut->addItem(ui->comboBoxVuosi->currentText() + " "
                                    + ui->comboBoxMatka->currentText() + " " +
                                    ui->textEditUrheilija->toPlainText() + "\n");

    /*
    std::map<InternetExplorers::InterfaceFilter::ValueFilters, QString> filter = {
        {InternetExplorers::InterfaceFilter::NAME, "Mursu Esa"}
    };
    */

    std::vector<std::vector<std::string>> newData;
    try {
        newData = m_DataHandler->getDataWithFilter(filter);
    } catch (InternetExplorers::FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    unsigned long int size = newData.size();
    std::cout << size << std::endl;

    allSearches.push_back(newData);
}

void Finlandia::on_pushButton_clicked()
{
    //Lineseries for test purposes
    QLineSeries *lineseries = new QLineSeries();
    lineseries->setName("Esan urotyöt (Kiitos Esa)");

    // Going through all of the added search data:
    for (unsigned int i = 0; i < allSearches.size(); i++){
        // Data added per search:
        std::vector<std::vector<std::string>> data = allSearches.at(i);

        // Going through individual results in a search:
        for (unsigned int j= 0; j < data.size(); j++){
            std::vector<std::string> result = data.at(j);
            QString disp = "";


            lineseries->append(QPoint(stoi(result.at(0)),stoi(result.at(2))));
            for (unsigned int k = 0; k < result.size(); k++){
                // Showcasing a result:

                disp += QString::fromStdString(result.at(k)) + " ";

            }
            ui->listWidgetResult->addItem(disp);
        }
        ui->listWidgetResult->addItem(+ "\n");
        //Adding the series to m_chart
        m_chart->addSeries(lineseries);
        m_chart->createDefaultAxes();
        ui->graafiWiev->setChart(m_chart);
    }

    // ui->listWidgetResults->
}
