#include <QWidget>
#include <QtCharts>
#include <QPlainTextEdit>
#include <iostream>
#include <QDebug>
#include "finlandia.h"
#include "UI/finlandia.h"
#include "ui_finlandia.h"
#include <QtCharts>

Finlandia::Finlandia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Finlandia),
    m_DataHandler(new InternetExplorers::DataHandler),
    allSearches{},
    m_chart(new QChart())
{
    ui->setupUi(this);
    //m_DataHandler->Initialize();

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

    std::map<InternetExplorers::InterfaceFilter::ValueFilters, QString> filter = {
        {InternetExplorers::InterfaceFilter::NAME, "Mursu Esa"}
    };

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
