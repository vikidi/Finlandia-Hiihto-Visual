#include <QWidget>
#include <QtCharts>
#include <QPlainTextEdit>
#include <iostream>
#include <QDebug>
#include "finlandia.h"
#include "UI/finlandia.h"
#include "ui_finlandia.h"
#include <QtCharts>
#include <cctype>

Finlandia::Finlandia(InternetExplorers::DataHandler* dh,
                     QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Finlandia),
    m_DataHandler(dh),
    allSearches{},
    m_chart(new QChart())
{
    ui->setupUi(this);

    connect(ui->add_graphButton, &QPushButton::clicked, this,
            &Finlandia::make_chart);

    connect(ui->del_all_graphs, &QPushButton::clicked, this,
            &Finlandia::remove_cart);

    connect(ui->save_graph_pb, &QPushButton::clicked, this,
            &Finlandia::save_chart);

    // Menubar does not take ownership of created menu, so it is stored in a vector
    m_menus.push_back(new QMenu("File"));
    QAction* settings = m_menus.back()->addAction("Encryption settings");
    connect(settings, &QAction::triggered, this, &Finlandia::encryptionSettingsOpened);
    menuBar()->addMenu(m_menus.back());
    QAction* close = m_menus.back()->addAction("Close");
    connect(close, &QAction::triggered, [&](){QMainWindow::close();});
}

Finlandia::~Finlandia()
{
    // Delete created menus
    for(auto menu : m_menus)
    {
        for(auto action : menu->actions())
        { // Delete all actions inside menus
            menu->removeAction(action);
            delete action;
        }
        delete menu;
    }

    delete ui;
    delete m_chart;
}

void Finlandia::on_pushButtonNollaKaikki_clicked()
{
    m_chart -> removeAllSeries();
    ui->listWidgetTehtHaut->clear();
    curr_series_title = "";
    ui->listWidgetResult->clear();

    allSearches.clear();
    previousSrc.clear();

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
    ui->haeHitainRP->setChecked(false);
    ui->haeKaikkiRP->setChecked(true);
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

    //The title for the search is made at the same time
    QString title;

    //Only one year is selected
    if(ui->comboBoxVuosi->currentIndex() != 0 &&
            ui->vuosivaliBox->currentIndex() == 0){
        if (title.length()>0){
            title = title + ", Vuosi: " + ui->comboBoxVuosi->currentText();
        }
        else{
            title = "Vuosi: " + ui->comboBoxVuosi->currentText();
        }
        std::pair<Filter_NS, QString> year_pair(
                    InternetExplorers::Constants::Filter::YEAR,
                    ui->comboBoxVuosi->currentText());

        filter.insert(year_pair);
    }
    else if(ui->comboBoxVuosi->currentIndex() != 0 &&
            ui->vuosivaliBox->currentIndex() != 0){

        if (title.length()>0){
            title = title + ", Vuosiväli: " + ui->comboBoxVuosi->currentText() +
                    "-" +ui->vuosivaliBox->currentText();
        }
        else{
            title = "Vuosiväli: " + ui->comboBoxVuosi->currentText() + "-" +
                    ui->vuosivaliBox->currentText();
        }

        std::pair<Filter_NS, QString> yearRange_pair(
                    InternetExplorers::Constants::Filter::ValueFilters::YEAR_RANGE,
                    ui->comboBoxVuosi->currentText() + ";" +
                    ui->vuosivaliBox->currentText());

        filter.insert(yearRange_pair);
    }

    if(ui->textEditUrheilija->toPlainText() != ""){
        if (title.length()>0){
            title = title + ", Nimi: " + ui->textEditUrheilija->toPlainText();
        }
        else{
            title = "Nimi: " + ui->textEditUrheilija->toPlainText();
        }
        std::pair<Filter_NS, QString> name_pair(
                    InternetExplorers::Constants::Filter::NAME,
                    ui->textEditUrheilija->toPlainText());

        filter.insert(name_pair);
    }

    if(ui->comboBoxMatka->currentIndex() != 0){

        if(title.length() > 0){
            title = title + ", Matka: " + ui->comboBoxMatka->currentText();
        }
        else{
            title = "Matka: " + ui->comboBoxMatka->currentText();
        }

        std::pair<Filter_NS, QString> distance_pair(
                    InternetExplorers::Constants::Filter::DISTANCE,
                    ui->comboBoxMatka->currentText());

        filter.insert(distance_pair);
    }

    if(ui->timeEditLower->time().toString() != "00:00:00" ||
            ui->timeEditUpper->time().toString() != "00:00:00"){

        if(title.length() > 0){
            title = title + ", Suoritusaikaväli: " + ui->timeEditLower->time().toString()
                    + "-" + ui->timeEditUpper->time().toString();
        }
        else{
            title = "Suoritusaikaväli: " + ui->timeEditLower->time().toString()
                    + "-" + ui->timeEditUpper->time().toString();
        }

        std::pair<Filter_NS, QString> timeRange_pair(
                    InternetExplorers::Constants::Filter::ValueFilters::TIME_RANGE,
                    ui->timeEditLower->time().toString() + ";" +
                    ui->timeEditUpper->time().toString());

        filter.insert(timeRange_pair);
    }

    if(ui->comboBoxSijoitusAla->currentIndex() != 0 &&
            ui->sukupuoliCB->currentIndex() == 0){

        if(ui->ComboBoxSijoitusYla->currentIndex() == 0){

            if (title.length() > 0 ){
                title = title + ", Sijoitus: " + ui->comboBoxSijoitusAla->currentText();
            }
            else{
                title = "Sijoitus: " + ui->comboBoxSijoitusAla->currentText();
            }

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE,
                        ui->comboBoxSijoitusAla->currentText());

            filter.insert(place_pair);
        }
        else{
            if (title.length() > 0 ){


                title = title + ", Sijoitusväli: " + ui->comboBoxSijoitusAla->
                        currentText() + "-" + ui->ComboBoxSijoitusYla->currentText();
            }
            else{
                title = "Sijoitusväli: " + ui->comboBoxSijoitusAla->currentText()+
                        "-" + ui->ComboBoxSijoitusYla->currentText();
            }

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_RANGE,
                        ui->comboBoxSijoitusAla->currentText() +
                        ";" + ui->ComboBoxSijoitusYla->currentText());

            filter.insert(place_pair);
        }

    }

    if(ui->comboBoxSijoitusAla->currentIndex() != 0 &&
            ui->sukupuoliCB->currentIndex() == 1){
        std::pair<Filter_NS, QString> place_pair(
                    InternetExplorers::Constants::Filter::PLACE_MEN,
                    ui->comboBoxSijoitusAla->currentText());

        filter.insert(place_pair);

    }
    if(ui->comboBoxSijoitusAla->currentIndex() != 0 &&
            ui->sukupuoliCB->currentText() == "N"){
        std::pair<Filter_NS, QString> place_pair(
                    InternetExplorers::Constants::Filter::PLACE_WOMEN,
                    ui->comboBoxSijoitusAla->currentText());

        filter.insert(place_pair);
    }

    if(ui->sukupuoliCB->currentIndex() != 0){

        if(title.length()>0){
            title = title + ", Sukupuoli:" + ui->sukupuoliCB->currentText();
        }
        else{
            title = "Sukupuoli:" + ui->sukupuoliCB->currentText();
        }

        std::pair<Filter_NS, QString> sex_pair(
                    InternetExplorers::Constants::Filter::SEX,
                    ui->sukupuoliCB->currentText());

        filter.insert(sex_pair);
    }

    if(ui->textEditHome->toPlainText() != ""){

        if(title.length() > 0){
            title = title + ", Kotimaa: " + ui->textEditHome->toPlainText();
        }
        else{
            title = "Kotimaa: " + ui->textEditHome->toPlainText();
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

    QString header = "";

    //Make small header for listview
    for(int k : attr_vect){
        header += QString::fromStdString(attribute_enum.at(k)) + " : ";
    }
    header = header.mid(0, header.length()-2);
    ui->listWidgetResult->addItem(header);

    // Going through individual results in a search:
    for (unsigned int j= 0; j < data.size(); j++){
        std::vector<std::string> result = data.at(j);
        QString disp = "";
        for(int k : attr_vect){
            // Showcasing a result:

            disp += QString::fromStdString(result.at(k)) + " ";
            //            qDebug() << QString::number(k) + ", "  +
            //                        QString::fromStdString(result.at(k));

        }
        ui->listWidgetResult->addItem(disp);
    }
    ui->listWidgetResult->addItem(+ "\n");
    ui->hakuLabelTulokset->setText("Haku: " + curr_series_title);

}

void Finlandia::make_chart()
{
    int x = ui->x_akseliCB->currentIndex();
    ui->x_axisTitle->setText(ui->x_akseliCB->currentText());

    int y = ui->y_akseliCB->currentIndex();
    ui->y_axisTitle->setText(ui->y_akseliCB->currentText());

    if(ui->kuvaajatyyppiCB->currentIndex() == 1){
        QLineSeries *series = new QLineSeries();
        series->setName(curr_series_title);

        //adding the data from only last search: TODO add later searches too
        std::vector<std::vector<std::string>> data = allSearches.at(allSearches.size()-1);

        // Going through individual results in a search:
        for(std::vector<std::string> result : data){
            if(std::isdigit(*result.at(x).c_str()) && std::isdigit(*result.at(y).c_str()))
                series->append(QPoint(stoi(result.at(x)),stoi(result.at(y))));
        }
        //Adding the series to m_chart
        m_chart->addSeries(series);

    }else if(ui->kuvaajatyyppiCB->currentIndex() == 0){

        QBarSeries* series = new QBarSeries();
        series->setName(curr_series_title);

        //adding the data from only last search: TODO add later searches too
        std::vector<std::vector<std::string>> data = allSearches.at(allSearches.size()-1);

        // Going through individual results in a search:
        for(std::vector<std::string> result : data){
            if(!std::isdigit(*result.at(y).c_str()))
                continue;
            QBarSet *set = new QBarSet(QString::fromStdString(result.at(x)));

            *set << stoi(result.at(y));

            series->append(set);

        }
        //Adding the series to m_chart
        m_chart->addSeries(series);

    }
    m_chart->createDefaultAxes();
    ui->graafiWiev->setChart(m_chart);

}

void Finlandia::remove_cart()
{
    m_chart->removeAllSeries();
}

void Finlandia::save_chart()
{
    QPixmap p = ui->graafiWiev->grab();

    QString fileName = QFileDialog::
            getSaveFileName(this,tr("Save Chart as Image"),
                            QString(), tr("Images (*.png)"));

    if (!fileName.isEmpty()){
        p.save(fileName);
    }
}

void Finlandia::make_listviweLabel()
{
    QString label = "Esitetään:";

    if(ui->haeHitainRP->isChecked()){
        label = label + " Hitain,";
    }
    if(ui->haeKaikkiRP->isChecked()){
        label = "Esitetään: Kaikki tulokset,";
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

    //Remove the last comma
    ui->ListViewesityslabel->setText(label.mid(0, label.length()-1));

}

std::vector<int> Finlandia::select_attributes()
{
    std::vector<int>atr_vec;

    enum Atributes { year, distance, time, place, place_men, place_wm,
                     sex, name, town, nationality, birth_yr, team};

    if(ui->haeKaikkiRP->isChecked()){
        atr_vec = {year, distance, time, place, place_men, place_wm,
                   sex, name, town, nationality, birth_yr, team};
    }
    if(ui->KotimaaRP->isChecked()){
        atr_vec.push_back(nationality);
    }
    if(ui->haeJoukkueRP->isChecked()){
        atr_vec.push_back(team);
    }
    if(ui->aikaRP->isChecked()){
        atr_vec.push_back(time);
    }
    if(ui->hiihtajanNimiRP->isChecked()){
        atr_vec.push_back(name);
    }

    return atr_vec;
}

void Finlandia::on_pushButtoLisaaHaku_clicked()
{
    curr_series_title = "";
    std::map<Filter_NS, QString> filter;

    filter = makefilter();

    std::vector<std::vector<std::string>> newData;
    try {
        newData = m_DataHandler->getDataWithFilter(filter);
    } catch (InternetExplorers::FilterException &e) {
        std::cout << e.what() << std::endl;
    }

    unsigned long int size = newData.size();
    std::cout << "Koko " << size << std::endl;

    if(size > 0){
        allSearches.push_back(newData);
        ui->listWidgetTehtHaut->addItem(curr_series_title);
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Valitsemiasi kriteereitä vastaavia tuloksia "
                       "ei löytynyt.");
        msgBox.exec();
    }
}

void Finlandia::on_pushButton_clicked()
{
    make_listview();
    make_listviweLabel();
}

void Finlandia::encryptionSettingsOpened()
{
    m_encryptionSettings = std::make_unique<EncryptionSettingsWindow>();
    m_encryptionSettings->setWindowModality(Qt::WindowModality::ApplicationModal);
    m_encryptionSettings->show();
    connect(m_encryptionSettings.get(), &EncryptionSettingsWindow::closeProgram, [&](){this->close();});
}
