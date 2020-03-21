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
    curr_series_title = "";

    ui->comboBoxVuosi->setCurrentIndex(0);
    ui->vuosivaliBox->setCurrentIndex(0);
    ui->textEditUrheilija->setText("");
    ui->comboBoxMatka->setCurrentIndex(0);
    ui->textEditUrheilija->setText("");
    ui->timeEditLower->setTime(QTime(0,0,0));
    ui->timeEditUpper->setTime(QTime(0,0,0));
    ui->ComboBoxSijoitus->setCurrentIndex(0);
    ui->sukupuoliCB->setCurrentIndex(0);
    ui->textEditHome->setText("");
}

std::map<InternetExplorers::InterfaceFilter::ValueFilters, QString> Finlandia::makefilter(){
    std::map<InternetExplorers::InterfaceFilter::ValueFilters, QString> filter;
    QString title;

    //Only one year is selected
    if(ui->comboBoxVuosi->currentText() != "Kaikki Vuodet" and
            ui->vuosivaliBox->currentText() == "Vuosiväli"){
        if (title.length()>0){
            title = title + ":" + ui->comboBoxVuosi->currentText();
        }
        else{
            title = ui->comboBoxVuosi->currentText();
        }
        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> year_pair(
                    InternetExplorers::InterfaceFilter::YEAR, ui->comboBoxVuosi->currentText());
        filter.insert(year_pair);
    }
    else if(ui->comboBoxVuosi->currentText() != "Kaikki Vuodet" and
            ui->vuosivaliBox->currentText() != "Vuosiväli"){

        if (title.length()>0){
            title = title + ":" + ui->comboBoxVuosi->currentText() +
                    "-" +ui->vuosivaliBox->currentText();
        }
        else{
            title = ui->comboBoxVuosi->currentText() + "-" +ui->vuosivaliBox->currentText();
        }

        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> yearRange_pair(
                    InternetExplorers::InterfaceFilter::ValueFilters::YEAR_RANGE,
                    ui->comboBoxVuosi->currentText() + ";" + ui->vuosivaliBox->currentText());
        filter.insert(yearRange_pair);
    }

    if(ui->textEditUrheilija->toPlainText() != ""){
        if (title.length()>0){
            title = ui->textEditUrheilija->toPlainText();
        }
        else{
            title = title + ":" + ui->textEditUrheilija->toPlainText();
        }
        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> name_pair(
                    InternetExplorers::InterfaceFilter::NAME, ui->textEditUrheilija->toPlainText());
        filter.insert(name_pair);
    }

    if(ui->comboBoxMatka->currentText() != "Kaikki Matkat"){

        if(title.length() > 0){
            title = title + ":" + ui->comboBoxMatka->currentText();
        }
        else{
            title = ui->comboBoxMatka->currentText();
        }

        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> distance_pair(
                    InternetExplorers::InterfaceFilter::DISTANCE, ui->comboBoxMatka->currentText());
        filter.insert(distance_pair);
    }

    if(ui->timeEditLower->time().toString() != "00:00:00" or
            ui->timeEditUpper->time().toString() != "00:00:00"){

        if(title.length() > 0){
            title = title + ":" + ui->timeEditLower->time().toString()
                    + "-" + ui->timeEditUpper->time().toString();
        }
        else{
            title = ui->timeEditLower->time().toString()
                    + "-" + ui->timeEditUpper->time().toString();
        }

        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> timeRange_pair(
                    InternetExplorers::InterfaceFilter::ValueFilters::TIME_RANGE,
                    ui->timeEditLower->time().toString() + ";" +
                    ui->timeEditUpper->time().toString());
        filter.insert(timeRange_pair);
    }

    if(ui->ComboBoxSijoitus->currentText() != "" and
            ui->sukupuoliCB->currentText() == ""){

        if (title.length() > 0 ){
            title = title + ":" + ui->ComboBoxSijoitus->currentText();
        }
        else{
            title = ui->ComboBoxSijoitus->currentText();
        }

        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> place_pair(
                    InternetExplorers::InterfaceFilter::PLACE, ui->ComboBoxSijoitus->currentText());
        filter.insert(place_pair);
    }else if(ui->sukupuoliCB->currentText() == "M"){
        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> place_pair(
                    InternetExplorers::InterfaceFilter::PLACE_MEN, ui->ComboBoxSijoitus->currentText());
        filter.insert(place_pair);
    }else if(ui->sukupuoliCB->currentText() == "N"){
        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> place_pair(
                    InternetExplorers::InterfaceFilter::PLACE_WOMEN, ui->ComboBoxSijoitus->currentText());
        filter.insert(place_pair);
    }

    if(ui->sukupuoliCB->currentText() != ""){

        if(title.length()>0){
            title = title + ":" + ui->sukupuoliCB->currentText();
        }
        else{
            title = ui->sukupuoliCB->currentText();
        }

        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> sex_pair(
                    InternetExplorers::InterfaceFilter::SEX, ui->sukupuoliCB->currentText());
        filter.insert(sex_pair);
    }

    if(ui->textEditHome->toPlainText() != ""){

        if(title.length() > 0){
            title = title + ":" + ui->textEditHome->toPlainText();
        }
        else{
            title = ui->textEditHome->toPlainText();
        }

        std::pair<InternetExplorers::InterfaceFilter::ValueFilters, QString> national_pair(
                    InternetExplorers::InterfaceFilter::NATIONALITY, ui->textEditHome->toPlainText());
        filter.insert(national_pair);
    }

    /*

        case InternetExplorers::InterfaceFilter::ValueFilters::CITY:
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::BIRTH_YEAR:
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::TEAM:
            break;
        };*/

    curr_series_title = title;
    return filter;
}

void Finlandia::make_listview()
{
    //Lineseries for test purposes
    QLineSeries *lineseries = new QLineSeries();
    lineseries->setName(curr_series_title);

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

void Finlandia::on_pushButtoLisaaHaku_clicked()
{
    curr_series_title = "";
    std::map<InternetExplorers::InterfaceFilter::ValueFilters, QString> filter;

    filter = makefilter();

    ui->listWidgetTehtHaut->addItem(curr_series_title);

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
    make_listview();
}

