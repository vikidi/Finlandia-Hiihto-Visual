#include <QWidget>
#include <QtCharts>
#include <QPlainTextEdit>
#include <QDebug>
#include "finlandia.h"
#include "UI/finlandia.h"
#include "ui_finlandia.h"
#include "gamescene.h"
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
    QAction* settings2 = m_menus.back()->addAction("Results predicter");
    connect(settings2, &QAction::triggered, this, &Finlandia::predicterOpened);
    QAction* game = m_menus.back()->addAction("Open a game");
    connect(game, &QAction::triggered, this, &Finlandia::openGame);
    QAction* close = m_menus.back()->addAction("Close");
    connect(close, &QAction::triggered, [&](){QMainWindow::close();});
    menuBar()->addMenu(m_menus.back());
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

    ui->comboBoxVuosi->setCurrentIndex(0);
    ui->vuosivaliBox->setCurrentIndex(0);
    ui->textEditUrheilija->setText("");
    ui->comboBoxMatka->setCurrentIndex(0);
    ui->timeEditLower->setTime(QTime(0,0,0));
    ui->timeEditUpper->setTime(QTime(0,0,0));
    ui->sukupuoliCB->setCurrentIndex(0);
    ui->spinBoxSijoitusYla->setValue(0);
    ui->spinBoxSijoitusAla->setValue(0);
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

    m_datalump.clear();
    all_titles.clear();

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

    if(ui->spinBoxSijoitusAla->value() != 0 &&
            ui->sukupuoliCB->currentIndex() == 0)
    { // PLACE has lower limit, no gender assigned

        if(ui->spinBoxSijoitusYla->value() == 0)
        { // PLACE does not have upper limit

            if (title.length() > 0 ){
                title = title + ", Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }
            else{
                title = "Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE,
                        QString::number(ui->spinBoxSijoitusAla->value()));

            filter.insert(place_pair);
        }
        else{ // PLACE has upper limit
            if (title.length() > 0 ){

                title = title + ", Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->
                                                                     value()) + "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }
            else{
                title = "Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->value())+
                        "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_RANGE,
                        QString::number(ui->spinBoxSijoitusAla->value())+
                        ";" + QString::number(ui->spinBoxSijoitusYla->value()));

            filter.insert(place_pair);
        }

    }

    if(ui->spinBoxSijoitusAla->value() != 0 &&
            ui->sukupuoliCB->currentText() == "N")
    { // PLACE has lower limit, gender is N

        if(ui->spinBoxSijoitusYla->value() == 0)
        { // PLACE does not have upper limit

            if (title.length() > 0 ){
                title = title + ", Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }
            else{
                title = "Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_WOMEN,
                        QString::number(ui->spinBoxSijoitusAla->value()));

            filter.insert(place_pair);
        }
        else{ // PLACE has upper limit
            if (title.length() > 0 ){


                title = title + ", Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->
                                                                     value()) + "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }
            else{
                title = "Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->value())+
                        "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_RANGE_WOMEN,
                        QString::number(ui->spinBoxSijoitusAla->value())+
                        ";" + QString::number(ui->spinBoxSijoitusYla->value()));

            filter.insert(place_pair);
        }
    }

    if(ui->spinBoxSijoitusAla->value() != 0 &&
            ui->sukupuoliCB->currentText() == "M")
    { // PLACE has lower limit, gender is M

        if(ui->spinBoxSijoitusYla->value() == 0)
        { // PLACE does not have upper limit

            if (title.length() > 0 ){
                title = title + ", Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }
            else{
                title = "Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_MEN,
                        QString::number(ui->spinBoxSijoitusAla->value()));

            filter.insert(place_pair);
        }
        else{ // PLACE has upper limit
            if (title.length() > 0 ){

                title = title + ", Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->
                                                                     value()) + "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }
            else{
                title = "Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->value())+
                        "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_RANGE_MEN,
                        QString::number(ui->spinBoxSijoitusAla->value())+
                        ";" + QString::number(ui->spinBoxSijoitusYla->value()));

            filter.insert(place_pair);
        }
    }

    if(ui->sukupuoliCB->currentIndex() != 0)
    { // No gender assigned

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

    for(auto value : filter)
    {
        qDebug() << value.first << value.second << "\n";
    }

    return filter;
}

void Finlandia::make_listview()
{

    std::vector<int>attr_vect = select_attributes();
    QString combined_title;

    int i = 0;

    //Adding all the searches in line
    for(auto data : allSearches){

        QString header = all_titles.at(i);
        ++i;
        ui->listWidgetResult->addItem(header);

        // Going through individual results in a search:
        for (unsigned int j= 0; j < data.size(); j++){
            std::vector<std::string> result = data.at(j);
            QString disp = "";

            //showing only attributes demanded in attr_vect
            for(int k : attr_vect){

                if(k <= Atributes::team){
                    // Showcasing a result:
                    disp += QString::fromStdString(result.at(k)) + " ";
                }
            }
            ui->listWidgetResult->addItem(disp);
        }
        ui->listWidgetResult->addItem(+ "\n");

        for (auto title : all_titles) {
            if (combined_title.length() > 0){
                combined_title = combined_title + "[" + title + "]";
            }
            else{
                combined_title = "[" + title + "]";
            }
        }

        ui->hakuLabelTulokset->setText("Haku: " + combined_title);
    }

    if(check_for_special_filters()){
        ui->listWidgetResult->addItem("#########" + combined_title +
                                      " LISÄTIEDOT:" + "#########");
        print_special_result(attr_vect);
    }
}

void Finlandia::print_special_result(std::vector<int> atr_vec)
{
    for (int k : atr_vec){
        if(k == Atributes::nmbr_of_parts)
        {
            ui->listWidgetResult->addItem("Osallistujien määrä vuosittain");

            for(auto thing : m_nmbr_of_parts){
                QString disp = "";
                disp += "Vuosi: ";
                disp += QString::fromStdString(thing.first) + " ";
                disp += "Osallistujien määrä: ";
                disp += QString::number(thing.second);

                ui->listWidgetResult->addItem(disp);
            }
            ui->listWidgetResult->addItem("\n");

        }

        if(k == Atributes::fastest)
        {
            ui->listWidgetResult->addItem("Joka vuoden nopein");
            for(auto thing : m_fastest)
            {
                QString disp = "";
                disp += "Vuosi: ";

                if(thing.second.size() < 1){
                    disp += QString::fromStdString(thing.first) + " ";
                    disp += "Ei tuloksia";
                }

                for(auto element : thing.second)
                {
                    // Showcasing a result:
                    disp += QString::fromStdString(element + " ");
                }
                ui->listWidgetResult->addItem(disp);
            }
            ui->listWidgetResult->addItem("\n");
        }
        if(k == Atributes::slowest){
            ui->listWidgetResult->addItem("Joka vuoden hitain");
            for(auto thing : m_slowest)
            {
                QString disp = "";
                disp += "Vuosi: ";

                if(thing.second.size() < 1){
                    disp += QString::fromStdString(thing.first) + " ";
                    disp += "Ei tuloksia";
                }

                for(auto element : thing.second)
                {
                    // Showcasing a result:
                    disp += QString::fromStdString(element + " ");
                }
                ui->listWidgetResult->addItem(disp);
            }
            ui->listWidgetResult->addItem("\n");
        }
        if(k == Atributes::avrg_speed)
        {
            ui->listWidgetResult->addItem("Aikojen keskiarvot");

            for(auto thing : m_avrg_time){
                QString disp = "";
                disp += "Vuosi: ";
                disp += QString::fromStdString(thing.first) + " ";
                disp += "Suoritusaikojen keskiarvo: ";
                disp += QString::fromStdString(thing.second);

                ui->listWidgetResult->addItem(disp);
            }
            ui->listWidgetResult->addItem("\n");
        }
        if(k == Atributes::nmbr_of_parts_nationvice){
            ui->listWidgetResult->addItem("Osallistujat maittain");

            for(auto thing : m_avrg_time){
                QString disp = "";
                disp += "Maa: ";
                disp += QString::fromStdString(thing.first) + " ";
                disp += "Urheilijoiden määrä : ";
                disp += QString::fromStdString(thing.second);

                ui->listWidgetResult->addItem(disp);
            }
            ui->listWidgetResult->addItem("\n");
        }
        if(k == Atributes::bestofx){
            ui->listWidgetResult->addItem("Vuoden parhaat joukkueet");

            for(auto thing : m_avrg_time){
                QString disp = "";
                disp += "Joukkue: ";
                disp += QString::fromStdString(thing.first) + " ";
                disp += "Suoritusaikojen keskiarvo: ";
                disp += QString::fromStdString(thing.second);

                ui->listWidgetResult->addItem(disp);
            }
            ui->listWidgetResult->addItem("\n");
        }
    }
}

void Finlandia::make_chart()
{
    int x = ui->x_akseliCB->currentIndex();
    ui->x_axisTitle->setText(ui->x_akseliCB->currentText());

    int y = ui->y_akseliCB->currentIndex();
    ui->y_axisTitle->setText(ui->y_akseliCB->currentText());


    if(ui->kuvaajatyyppiCB->currentIndex() == 1){

        if(std::isdigit(*m_datalump.at(1).at(y).c_str()) &&
                std::isdigit(*m_datalump.at(1).at(y).c_str())){
            make_line_chart(x, y);
        }

    }else if(ui->kuvaajatyyppiCB->currentIndex() == 0){
        if(std::isdigit(*m_datalump.at(1).at(y).c_str())){
            make_bar_chart(x, y);
        }
    }
}


void Finlandia::make_bar_chart(int x, int y)
{
    QBarSeries* series = new QBarSeries();
    series->setName(curr_series_title);

    //adding the data from only last search: TODO add later searches too
    std::vector<std::vector<std::string>> data = m_datalump;

    // Going through individual results in a search:
    for(std::vector<std::string> result : data){
        QBarSet *set = new QBarSet(QString::fromStdString(result.at(x)));

        if(y == 2){
            int secs = QTime(0, 0, 0).secsTo(QTime::fromString
                                             (QString::fromStdString(
                                                  result.at(y)), "h:mm:ss"));
            *set << secs;

        }
        series->append(set);

    }
    //Adding the series to m_chart
    m_chart->addSeries(series);

    m_chart->createDefaultAxes();
    ui->graafiWiev->setChart(m_chart);

}

void Finlandia::make_line_chart(int x, int y)
{
    QLineSeries *series = new QLineSeries();
    series->setName(curr_series_title);

    //adding the data from only last search: TODO add later searches too
    std::vector<std::vector<std::string>> data = m_datalump;

    // Going through individual results in a search:
    for(std::vector<std::string> result : data){

        series->append(QPoint(stoi(result.at(x)),stoi(result.at(y))));
    }
    //Adding the series to m_chart
    m_chart->addSeries(series);

}

void Finlandia::apply_special_filters(std::map<Filter_NS,
                                      QString> filters)
{


    //Näytetään kullekin vuodelle osallistujien määrä sekä voittajan
    //ja viimeiseksi maaliin tulleen ajat ja keskinopeudet jokaiselta matkalta.
    if(ui->haeOsalMaarRP->isChecked()){
        // < year, amount >
        m_nmbr_of_parts = m_DataHandler->getAmountOfParticipants(filters);

    }

    if(ui->haeHitainRP->isChecked()){
        // < year, row >
        m_slowest = m_DataHandler -> getSlowest(filters);

    }
    if( ui->haeNopeinRP->isChecked() ){
        // <year, row >
        m_fastest = m_DataHandler->getFastest(filters);

    }

    if(ui->keskinopeusRP->isChecked()){
        // Needs to have at least DISTANCE filter
        // < year, average time >
        m_avrg_time = m_DataHandler->getAverageTimes(filters);

    }

    if(ui->osall_lkm_maittainRP->isChecked()){
        //•Näytetään urheilijoiden jakauma maittain
        //hae-kohtaan "Urheilijoiden määrä maittain"
        m_nmbr_of_parts_nationvice =
                m_DataHandler->getParticipantsByCountry(filters);


    }

    if(ui->vuodenXparhaatRP->isChecked()){
        //Vuoden X parhaat joukkeet
        //Needs to have at least DISTANCE filter
        // < team, average time >
        m_best_of_year_X = m_DataHandler->getBestTenTeams(filters);
    }

}

void Finlandia::order_result(std::map<Filter_NS,
                             QString> filters)
{
    //Järjestetään tietyn vuoden tulokset seuran nimen mukaan aakkosjärjestykseen.
    if(ui->jarjestaAakkosRP->isChecked() && ui->jarjestaSeuranNimiRP->isChecked()){
        std::vector<std::vector<std::string>> ordered_data =
                m_DataHandler->getDataWithFilter(filters,
                                                 InternetExplorers::Constants::
                                                 Filter::OrderFilters::ALPH_TEAM);
    }

    /*
    enum OrderFilters {
        PLACEMENT = 0,      //< Sort by placement
    YEAR_ORDER,         //< Sort by year
            DISTANCE_ORDER,     //< Sort by distance
            AGE,                //< Sort by age of the skier
            ALPH_NATIONALITY,   //< Sort alphabetically by nationality
            ALPH_NAME,          //< Sort alphabetically by name
            ALPH_TEAM,          //< Sort alphabetically by team
            ALPH_CITY,          //< Sort alphabetically by city
};
*/

}

bool Finlandia::check_for_special_filters()
{
    if(ui->haeOsalMaarRP->isChecked()){
        return true;
    }
    if(ui->haeHitainRP->isChecked()){
        return true;
    }
    if( ui->haeNopeinRP->isChecked() ){
        return true;
    }
    if(ui->keskinopeusRP->isChecked()){
        return true;
    }
    if(ui->osall_lkm_maittainRP->isChecked()){
        return true;
    }
    if(ui->vuodenXparhaatRP->isChecked()){
        return true;
    }
    return false;
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
    QString jarjestys = "Järjestetään:";

    if(ui->haeHitainRP->isChecked()){
        label = label + " Hitain,";
    }
    if(ui->haeKaikkiRP->isChecked()){
        label = "Esitetään: Kaikki tulokset,";
    }
    if(ui->haeNopeinRP->isChecked()){
        label = label + " Nopein,";
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

    if(ui->vuodenXparhaatRP->isChecked()){
        label = label + " Vuoden " + ui->comboBoxVuosi->currentText() +
                " parhaat tulokset, ";
    }

    if(ui->haeOsalMaarRP->isChecked()){
        label = label + " Osallistujien määrä,";
    }

    if(ui->osall_lkm_maittainRP->isChecked()){
        label = label + " Osallistujien määrä maittain,";
    }

    //JÄRJESTYS-ASETUKET
    if(ui->jarjestaAakkosRP->isChecked()){
        jarjestys = jarjestys + " Aakkosjärjestys,";
    }


    if(ui->jarjestaSeuranNimiRP->isChecked()){
        jarjestys = jarjestys + " Seuran nimen mukaan,";
    }

    //Remove the last comma
    label = label.mid(0, label.length()-1);
    //Remove the last comma
    jarjestys = jarjestys.mid(0, jarjestys.length()-1);

    ui->ListViewesityslabel->setText(label);
    ui->jarjestyslabel->setText(jarjestys);

}

std::vector<int> Finlandia::select_attributes()
{
    std::vector<int>atr_vec;

    if(ui->haeKaikkiRP->isChecked()){
        atr_vec = {Atributes::year, Atributes::distance, Atributes::time,
                   Atributes::place, Atributes::place_men, Atributes::place_wm,
                   Atributes::sex, Atributes::name, Atributes::town,
                   Atributes::nationality, Atributes::birth_yr, Atributes::team};
        qDebug()<< "atr_vec";
        if(!check_for_special_filters()){
            return atr_vec;
        }
    }

    if(ui->KotimaaRP->isChecked()){
        atr_vec.push_back(Atributes::nationality);
    }
    if(ui->haeJoukkueRP->isChecked()){
        atr_vec.push_back(Atributes::team);
    }
    if(ui->aikaRP->isChecked()){
        atr_vec.push_back(Atributes::time);
    }
    if(ui->hiihtajanNimiRP->isChecked()){
        atr_vec.push_back(Atributes::name);
    }
    if(ui->haeOsalMaarRP->isChecked()){
        atr_vec.push_back(Atributes::nmbr_of_parts); //Number of participants
    }
    if( ui->haeNopeinRP->isChecked() ){
        atr_vec.push_back(Atributes::fastest); //fastest time
    }

    if(ui->haeHitainRP->isChecked()){
        atr_vec.push_back(Atributes::slowest); //the slowest time
    }

    if(ui->keskinopeusRP->isChecked()){
        atr_vec.push_back(Atributes::avrg_speed);
    }
    if(ui->osall_lkm_maittainRP->isChecked()){
        atr_vec.push_back(Atributes::nmbr_of_parts_nationvice);
    }

    if(ui->vuodenXparhaatRP->isChecked()){
        atr_vec.push_back(Atributes::bestofx);
    }
    return atr_vec;
}

void Finlandia::on_pushButtoLisaaHaku_clicked()
{

    std::vector<std::vector<std::string>> newData;

    //Also makes curr_series_title
    std::map<Filter_NS, QString> filter = makefilter();

    if(check_for_special_filters()){
        apply_special_filters(filter);
    }


    try {
        newData = m_DataHandler->getDataWithFilter(filter);
    }
    catch (InternetExplorers::FilterException &e) {
        qDebug() << e.what();
    }

    unsigned long int size = newData.size();
    qDebug() << "Koko" << size;

    if(size > 0){
        allSearches.push_back(newData);

        m_datalump.insert( m_datalump.end(), newData.begin(), newData.end());

        //m_all_searches.at(curr_series_title) = newData;
        ui->listWidgetTehtHaut->addItem(curr_series_title);
        all_titles.push_back(curr_series_title);
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Hakuehdot eivät tuottaneet yhtäkään tulosta");
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

void Finlandia::predicterOpened()
{
    m_predicter = std::make_unique<PredicterWindow>();
    m_predicter->setWindowModality(Qt::WindowModality::ApplicationModal);
    m_predicter->show();
}

void Finlandia::openGame()
{
    m_gameWindow = std::make_unique<QMainWindow>();
    m_gameWindow->setWindowTitle("Ski game");
    auto scene = new InternetExplorers::GameScene(true,m_gameWindow.get());
    auto sceneView = new QGraphicsView(scene,m_gameWindow.get());
    m_gameWindow->setCentralWidget(sceneView);
    m_gameWindow->resize(620,150);
    m_gameWindow->show();
}

void Finlandia::on_spinBoxSijoitusYla_valueChanged(int newValue)
{
    if(newValue == 0)
    { // No need to check anything
        return;
    }

    if(ui->spinBoxSijoitusAla->value() == 0)
    { // Lower limit is not set
        if(newValue != 0)
        { // Automatically set lower limit to 1 to enable filter check for upper limit
            ui->spinBoxSijoitusAla->setValue(1);
            return;
        }

    } else if(ui->spinBoxSijoitusAla->value() > newValue)
    { // Prevent lower limit from being bigger than upper limit
        ui->spinBoxSijoitusAla->setValue(newValue);
    }
}

void Finlandia::on_spinBoxSijoitusAla_valueChanged(int newValue)
{
    if(newValue == 0)
    { // If RANGE -filters are not used
        ui->spinBoxSijoitusYla->setValue(0);

    } else if(ui->spinBoxSijoitusYla->value() != 0)
    { // If PLACE_RANGE filter is going to be used

        if(ui->spinBoxSijoitusYla->value() < newValue)
        { // Prevent upper limit from being smaller than lower limit
            ui->spinBoxSijoitusYla->setValue(newValue);
        }
    }
}


