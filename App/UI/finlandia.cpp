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
    ui->ComboBoxSijoitusYla->setCurrentIndex(0);
    ui->comboBoxSijoitusAla->setCurrentIndex(0);
    ui->sukupuoliCB->setCurrentIndex(0);
    ui->textEditHome->setText("");
    ui->esitysListanaRP->setChecked(false);
    ui->esitysGraafinenRP->setChecked(false);
    ui->haeHitainRP->setChecked(false);
    ui->haeKaikkiRP->setChecked(false);
    ui->haeNopeinRP->setChecked(false);
    ui->haeOsalMaarRP->setChecked(false);
    ui->KotimaaRP->setChecked(false);
    ui->haeJoukkueRP->setChecked(false);
    ui->aikaRP->setChecked(false);
    ui->keskinopeusRP->setChecked(false);
    ui->hiihtajanNimiRP->setChecked(false);

}

std::map<Filter_NS, QString> Finlandia::makefilter(){
    std::map<Filter_NS, QString> filter;
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
        std::pair<Filter_NS, QString> year_pair(
                    InternetExplorers::Constants::Filter::YEAR,
                    ui->comboBoxVuosi->currentText());

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

        std::pair<Filter_NS, QString> yearRange_pair(
                    InternetExplorers::Constants::Filter::ValueFilters::YEAR_RANGE,
                    ui->comboBoxVuosi->currentText() + ";" + ui->vuosivaliBox->currentText());
        filter.insert(yearRange_pair);
    }

    if(ui->textEditUrheilija->toPlainText() != ""){
        if (title.length()>0){
            title = title + ":" + ui->textEditUrheilija->toPlainText();
        }
        else{
            title = ui->textEditUrheilija->toPlainText();
        }
        std::pair<Filter_NS, QString> name_pair(
                    InternetExplorers::Constants::Filter::NAME, ui->textEditUrheilija->toPlainText());
        filter.insert(name_pair);
    }

    if(ui->comboBoxMatka->currentText() != "Kaikki Matkat"){

        if(title.length() > 0){
            title = title + ":" + ui->comboBoxMatka->currentText();
        }
        else{
            title = ui->comboBoxMatka->currentText();
        }

        std::pair<Filter_NS, QString> distance_pair(
                    InternetExplorers::Constants::Filter::DISTANCE, ui->comboBoxMatka->currentText());
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

        std::pair<Filter_NS, QString> timeRange_pair(
                    InternetExplorers::Constants::Filter::ValueFilters::TIME_RANGE,
                    ui->timeEditLower->time().toString() + ";" +
                    ui->timeEditUpper->time().toString());

        filter.insert(timeRange_pair);
    }

    if(ui->comboBoxSijoitusAla->currentIndex() != 0 and
            ui->sukupuoliCB->currentIndex() == 0){

        if(ui->ComboBoxSijoitusYla->currentIndex() == 0){

            if (title.length() > 0 ){
                title = title + ":" + ui->comboBoxSijoitusAla->currentText();
            }
            else{
                title = ui->comboBoxSijoitusAla->currentText();
            }

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE,
                        ui->comboBoxSijoitusAla->currentText());

            filter.insert(place_pair);
        }
        else{
            if (title.length() > 0 ){
                title = title + ":" + ui->comboBoxSijoitusAla->currentText() +
                        "-" + ui->ComboBoxSijoitusYla->currentText();
            }
            else{
                title = ui->comboBoxSijoitusAla->currentText()+
                        "-" + ui->ComboBoxSijoitusYla->currentText();
            }

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_RANGE,
                        ui->comboBoxSijoitusAla->currentText() +
                        ";" + ui->ComboBoxSijoitusYla->currentText());

            filter.insert(place_pair);
        }

    }else if(ui->sukupuoliCB->currentText() == "M"){
        std::pair<Filter_NS, QString> place_pair(
                    InternetExplorers::Constants::Filter::PLACE_MEN,
                    ui->comboBoxSijoitusAla->currentText());

        filter.insert(place_pair);

    }else if(ui->sukupuoliCB->currentText() == "N"){
        std::pair<Filter_NS, QString> place_pair(
                    InternetExplorers::Constants::Filter::PLACE_WOMEN,
                    ui->comboBoxSijoitusAla->currentText());

        filter.insert(place_pair);
    }

    if(ui->sukupuoliCB->currentText() != ""){

        if(title.length()>0){
            title = title + ":" + ui->sukupuoliCB->currentText();
        }
        else{
            title = ui->sukupuoliCB->currentText();
        }

        std::pair<Filter_NS, QString> sex_pair(
                    InternetExplorers::Constants::Filter::SEX,
                    ui->sukupuoliCB->currentText());

        filter.insert(sex_pair);
    }

    if(ui->textEditHome->toPlainText() != ""){

        if(title.length() > 0){
            title = title + ":" + ui->textEditHome->toPlainText();
        }
        else{
            title = ui->textEditHome->toPlainText();
        }

        std::pair<Filter_NS, QString> national_pair(
                    InternetExplorers::Constants::Filter::NATIONALITY,
                    ui->textEditHome->toPlainText());

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

    std::vector<int>attr_vect = select_attributes();

    //Adding only latest search to list
    std::vector<std::vector<std::string>> data =
            allSearches.at(allSearches.size() -1);


    // Going through individual results in a search:
    for (unsigned int j= 0; j < data.size(); j++){
        std::vector<std::string> result = data.at(j);
        QString disp = "";

        //for (unsigned int k = 0; k < result.size(); k++)
        for(int k : attr_vect){
            // Showcasing a result:

            disp += QString::fromStdString(result.at(k)) + " ";
            qDebug() << QString::number(k) + ", "  +
                        QString::fromStdString(result.at(k));

        }
        ui->listWidgetResult->addItem(disp);
    }
    ui->listWidgetResult->addItem(+ "\n");

}

void Finlandia::make_chart()
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
            lineseries->append(QPoint(stoi(result.at(0)),stoi(result.at(2))));
        }
        //Adding the series to m_chart
        m_chart->addSeries(lineseries);
        m_chart->createDefaultAxes();
        ui->graafiWiev->setChart(m_chart);
    }
}

void Finlandia::make_listviweLabel()
{
    QString label = "Esitetään:";

    if(ui->haeHitainRP->isChecked()){
        label = label + " Hitain,";
    }
    if(ui->haeKaikkiRP->isChecked()){
        label = label + " Kaikki tulokset,";
    }
    if(ui->haeNopeinRP->isChecked()){
        label = label + " Nopein,";
    }
    if(ui->haeOsalMaarRP->isChecked()){
        label = label + " Osallistujien määrä,";
    }
    if(ui->KotimaaRP->isChecked()){
        label = label + " Kotimaa,";
    }
    if(ui->haeJoukkueRP->isChecked()){
        label = label + " Joukkue,";
    }
    if(ui->aikaRP->isChecked()){
        label = label + " Aika,";
    }
    if(ui->keskinopeusRP->isChecked()){
        label = label + " Keskinopeus,";
    }
    if(ui->hiihtajanNimiRP->isChecked()){
        label = label + " Nimi,";
    }
    if(ui->haeKaikkiRP->isChecked()){
        label = label + " Kaikki tiedot,";
    }

    //Remove the last comma
    ui->ListViewesityslabel->setText(label.mid(0, label.length()-1));

}

std::vector<int> Finlandia::select_attributes()
{
    std::vector<int>atr_vec;

    enum Atributes { year, distance, time, place, place_men, place_wm,
                     sex, name, town, nationality, birth_yr, team};

    if(ui->haeHitainRP->isChecked()){

    }
    if(ui->haeKaikkiRP->isChecked()){
        atr_vec = {year, distance, time, place, place_men, place_wm,
                   sex, name, town, nationality, birth_yr, team};
    }
    if(ui->haeNopeinRP->isChecked()){

    }
    if(ui->haeOsalMaarRP->isChecked()){

    }
    if(ui->KotimaaRP->isChecked()){
        atr_vec.push_back(nationality);
    }
    if(ui->haeJoukkueRP->isChecked()){
        atr_vec.push_back(team);
    }

    return atr_vec;
}

void Finlandia::on_pushButtoLisaaHaku_clicked()
{
    curr_series_title = "";
    std::map<Filter_NS, QString> filter;

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
    if(ui->esitysListanaRP->isChecked()){
        make_listview();
        make_listviweLabel();
        if(ui->esitysGraafinenRP->isChecked()){
            make_chart();
        }
    }else if(ui->esitysGraafinenRP->isChecked()){
        make_chart();
    }else{
        QMessageBox msgBox;
        msgBox.setText("Et ole valinnut esitystapaa.");
        msgBox.exec();
    }
}

