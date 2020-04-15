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
#include <QSizePolicy>
#include "../constants.h"

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

    // Setup scroll area for tables
    ui->scrollArea->setWidget(ui->scrollWidget);
    m_scrollLayout = new QHBoxLayout(ui->scrollWidget);
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
    // Clear charts
    m_chart -> removeAllSeries();

    // Clear result tables
    QLayoutItem* item;
    while ( (item = m_scrollLayout->takeAt( 0 )) != nullptr ) {
        delete item->widget();
        delete item;
    }

    // Clear search listing
    ui->listWidgetTehtHaut->clear();

    // Clear search related containers
    allSearches.clear();
    m_titles.clear();
    m_headers.clear();

    // Clear UI options
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
    ui->osall_lkm_maittainRP->setChecked(false);
    ui->vuodenXparhaatRP->setChecked(false);
    ui->KotimaaRP->setChecked(false);
    ui->haeJoukkueRP->setChecked(false);
    ui->aikaRP->setChecked(false);
    ui->keskinopeusRP->setChecked(false);
    ui->hiihtajanNimiRP->setChecked(false);

    ui->jarjestaMatkaRP->setChecked(false);
    ui->jarjestaVuosiRP->setChecked(false);
    ui->jarjestaKotimaaRP->setChecked(false);
    ui->jarjestaSijoitusRP->setChecked(false);
    ui->jarjestaVainParasRP->setChecked(false);
    ui->jarjestaSeuranNimiRP->setChecked(false);
    ui->jarjestaAlkupRP->setChecked(true);

    // DEPR
    m_datalump.clear();
    all_titles.clear();
    // /DEPR
}

std::map<Filter_NS, QString> Finlandia::makefilter()
{
    std::map<Filter_NS, QString> filter;

    //Only one year is selected
    if (ui->comboBoxVuosi->currentIndex() != 0 &&
            ui->vuosivaliBox->currentIndex() == 0) {

        std::pair<Filter_NS, QString> year_pair(
                    InternetExplorers::Constants::Filter::YEAR,
                    ui->comboBoxVuosi->currentText());

        filter.insert(year_pair);
    }

    else if (ui->comboBoxVuosi->currentIndex() != 0 &&
            ui->vuosivaliBox->currentIndex() != 0) {

        std::pair<Filter_NS, QString> yearRange_pair(
                    InternetExplorers::Constants::Filter::ValueFilters::YEAR_RANGE,
                    ui->comboBoxVuosi->currentText() + ";" +
                    ui->vuosivaliBox->currentText());

        filter.insert(yearRange_pair);
    }

    if (ui->textEditUrheilija->toPlainText() != "") {

        std::pair<Filter_NS, QString> name_pair(
                    InternetExplorers::Constants::Filter::NAME,
                    ui->textEditUrheilija->toPlainText());

        filter.insert(name_pair);
    }

    if (ui->comboBoxMatka->currentIndex() != 0) {

        std::pair<Filter_NS, QString> distance_pair(
                    InternetExplorers::Constants::Filter::DISTANCE,
                    ui->comboBoxMatka->currentText());

        filter.insert(distance_pair);
    }

    if (ui->timeEditLower->time().toString() != "00:00:00" ||
            ui->timeEditUpper->time().toString() != "00:00:00") {

        std::pair<Filter_NS, QString> timeRange_pair(
                    InternetExplorers::Constants::Filter::ValueFilters::TIME_RANGE,
                    ui->timeEditLower->time().toString() + ";" +
                    ui->timeEditUpper->time().toString());

        filter.insert(timeRange_pair);
    }

    if (ui->spinBoxSijoitusAla->value() != 0 &&
            ui->sukupuoliCB->currentIndex() == 0) { // PLACE has lower limit, no gender assigned

        if (ui->spinBoxSijoitusYla->value() == 0) { // PLACE does not have upper limit

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE,
                        QString::number(ui->spinBoxSijoitusAla->value()));

            filter.insert(place_pair);
        }
        else { // PLACE has upper limit

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_RANGE,
                        QString::number(ui->spinBoxSijoitusAla->value())+
                        ";" + QString::number(ui->spinBoxSijoitusYla->value()));

            filter.insert(place_pair);
        }

    }

    if (ui->spinBoxSijoitusAla->value() != 0 &&
            ui->sukupuoliCB->currentText() == "N") { // PLACE has lower limit, gender is N

        if(ui->spinBoxSijoitusYla->value() == 0) { // PLACE does not have upper limit

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_WOMEN,
                        QString::number(ui->spinBoxSijoitusAla->value()));

            filter.insert(place_pair);
        }
        else { // PLACE has upper limit

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_RANGE_WOMEN,
                        QString::number(ui->spinBoxSijoitusAla->value())+
                        ";" + QString::number(ui->spinBoxSijoitusYla->value()));

            filter.insert(place_pair);
        }
    }

    if (ui->spinBoxSijoitusAla->value() != 0 &&
            ui->sukupuoliCB->currentText() == "M") { // PLACE has lower limit, gender is M

        if(ui->spinBoxSijoitusYla->value() == 0) { // PLACE does not have upper limit

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_MEN,
                        QString::number(ui->spinBoxSijoitusAla->value()));

            filter.insert(place_pair);
        }
        else { // PLACE has upper limit

            std::pair<Filter_NS, QString> place_pair(
                        InternetExplorers::Constants::Filter::PLACE_RANGE_MEN,
                        QString::number(ui->spinBoxSijoitusAla->value())+
                        ";" + QString::number(ui->spinBoxSijoitusYla->value()));

            filter.insert(place_pair);
        }
    }

    if (ui->sukupuoliCB->currentIndex() != 0) { // No gender assigned

        std::pair<Filter_NS, QString> sex_pair(
                    InternetExplorers::Constants::Filter::SEX,
                    ui->sukupuoliCB->currentText());

        filter.insert(sex_pair);
    }

    if (ui->textEditHome->toPlainText() != "") {

        std::pair<Filter_NS, QString> national_pair(
                    InternetExplorers::Constants::Filter::NATIONALITY,
                    ui->textEditHome->toPlainText());

        filter.insert(national_pair);
    }

    return filter;
}

void Finlandia::make_listview()
{
    //Adding all the searches
    unsigned long long i = 0;
    for(auto data : allSearches){
        addTableWidget(data, m_headers.at(i), m_titles.at(i).toStdString());
        ++i;
    }
}

void Finlandia::print_special_result(std::vector<int> atr_vec)
{
    /*for (int k : atr_vec){
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

            for(auto thing : m_nmbr_of_parts_nationvice){
                QString disp = "";
                disp += "Maa: ";
                disp += QString::fromStdString(thing.first) + " ";
                disp += "Urheilijoiden määrä : ";
                disp += QString::number(thing.second);

                ui->listWidgetResult->addItem(disp);
            }
            ui->listWidgetResult->addItem("\n");
        }
        if(k == Atributes::bestofx){
            ui->listWidgetResult->addItem("Vuoden TOP10");
            int i = 1;

            for(auto thing : m_best_of_year_X){
                QString disp = "";
                disp += QString::number(i) + ". " + "Joukkue: ";
                disp += QString::fromStdString(thing.first) + " ";
                disp += "Suoritusaikojen keskiarvo: ";
                disp += QString::fromStdString(thing.second);

                ui->listWidgetResult->addItem(disp);
                ++i;
            }
            ui->listWidgetResult->addItem("\n");
        }
    }*/
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
    //series->setName(curr_series_title);

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
    //series->setName(curr_series_title);

    //adding the data from only last search: TODO add later searches too
    std::vector<std::vector<std::string>> data = m_datalump;

    // Going through individual results in a search:
    for(std::vector<std::string> result : data){
        if(y == 2){
            int secs = QTime(0, 0, 0).secsTo(QTime::fromString
                                             (QString::fromStdString(
                                                  result.at(y)), "h:mm:ss"));
            series->append(QPoint(stoi(result.at(x)),secs));
        }
        else{

            series->append(QPoint(stoi(result.at(x)),stoi(result.at(y))));
        }
    }
    //Adding the series to m_chart
    m_chart->addSeries(series);

}

void Finlandia::apply_special_filters(std::map<Filter_NS,
                                      QString> filters)
{
    // Osallistujien määrä
    if(ui->haeOsalMaarRP->isChecked()){

        // Get data, < year, amount >
        std::map<std::string, int> data = m_DataHandler->getAmountOfParticipants(filters);

        if (data.size() > 0) {

            // Change data to row format
            std::vector<std::vector<std::string>> newData = {};
            for (const auto& row : data) {
                std::vector<std::string> newRow = {};
                newRow.emplace_back(row.first); // Add year
                newRow.emplace_back(std::to_string(row.second)); // Add amount

                newData.emplace_back(newRow);
            }

            // Add to searches
            allSearches.emplace_back(newData);

            // Create title
            QString title = "Osallistujien määrä; " + makeNormalTitle();
            ui->listWidgetTehtHaut->addItem(title);
            m_titles.emplace_back(title);

            // Create header
            std::vector<std::string> head({});
            head.emplace_back(" Vuosi ");
            head.emplace_back(" Osallisujamäärä ");
            m_headers.emplace_back(head);
        }
    }

    if(ui->haeHitainRP->isChecked()){
        // < year, row >
        std::map<std::string, std::vector<std::string>> data = m_DataHandler->getSlowest(filters);

        if (data.size() > 0) {

            // Change data to row format
            std::vector<std::vector<std::string>> newData = {};
            for (const auto& row : data) {
                std::vector<std::string> newRow = {};
                newRow.emplace_back(row.first); // Add year
                newRow.emplace_back(row.second[InternetExplorers::Constants::DataIndex::IndexInData::NAME]); // Add name
                newRow.emplace_back(row.second[InternetExplorers::Constants::DataIndex::IndexInData::TIME]); // Add time

                newData.emplace_back(newRow);
            }

            // Add to searches
            allSearches.emplace_back(newData);

            // Create title
            QString title = "Hitain aika; " + makeNormalTitle();
            ui->listWidgetTehtHaut->addItem(title);
            m_titles.emplace_back(title);

            // Create header
            std::vector<std::string> head({});
            head.emplace_back(" Vuosi ");
            head.emplace_back(" Nimi ");
            head.emplace_back(" Aika ");
            m_headers.emplace_back(head);
        }
    }
    if( ui->haeNopeinRP->isChecked() ){

        // <year, row >
        std::map<std::string, std::vector<std::string>> data = m_DataHandler->getFastest(filters);

        if (data.size() > 0) {


            std::vector<std::vector<std::string>> newData = {};
            for (const auto& row : data) {
                std::vector<std::string> newRow = {};
                newRow.emplace_back(row.first); // Add year
                newRow.emplace_back(row.second[InternetExplorers::Constants::DataIndex::IndexInData::NAME]); // Add name
                newRow.emplace_back(row.second[InternetExplorers::Constants::DataIndex::IndexInData::TIME]); // Add time

                newData.emplace_back(newRow);
            }

            // Add to searches
            allSearches.emplace_back(newData);

            // Create title
            QString title = "Nopein aika; " + makeNormalTitle();
            ui->listWidgetTehtHaut->addItem(title);
            m_titles.emplace_back(title);

            // Create header
            std::vector<std::string> head({});
            head.emplace_back(" Vuosi ");
            head.emplace_back(" Nimi ");
            head.emplace_back(" Aika ");
            m_headers.emplace_back(head);
        }
    }

    if(ui->keskinopeusRP->isChecked()){
        // Needs to have at least DISTANCE filter
        // < year, average time >
        std::map<std::string, std::string> data = m_DataHandler->getAverageTimes(filters);

        if (data.size() > 0) {


            std::vector<std::vector<std::string>> newData = {};
            for (const auto& row : data) {
                std::vector<std::string> newRow = {};
                newRow.emplace_back(row.first); // Add year
                newRow.emplace_back(row.second); // Add time

                newData.emplace_back(newRow);
            }

            // Add to searches
            allSearches.emplace_back(newData);

            // Create title
            QString title = "Keskiaika; " + makeNormalTitle();
            ui->listWidgetTehtHaut->addItem(title);
            m_titles.emplace_back(title);

            // Create header
            std::vector<std::string> head({});
            head.emplace_back(" Vuosi ");
            head.emplace_back(" Keskinopeus ");
            m_headers.emplace_back(head);
        }
    }

    if(ui->osall_lkm_maittainRP->isChecked()){

        std::map<std::string, int> data = m_DataHandler->getParticipantsByCountry(filters);

        if (data.size() > 0) {


            std::vector<std::vector<std::string>> newData = {};
            for (const auto& row : data) {
                std::vector<std::string> newRow = {};
                newRow.emplace_back(row.first); // Add year
                newRow.emplace_back(std::to_string(row.second)); // Add amount

                newData.emplace_back(newRow);
            }

            // Add to searches
            allSearches.emplace_back(newData);

            // Create title
            QString title = "Osallistujien määrä maittain; " + makeNormalTitle();
            ui->listWidgetTehtHaut->addItem(title);
            m_titles.emplace_back(title);

            // Create header
            std::vector<std::string> head({});
            head.emplace_back(" Maa ");
            head.emplace_back(" Osallistujamäärä ");
            m_headers.emplace_back(head);
        }
    }

    if(ui->vuodenXparhaatRP->isChecked()){
        //Vuoden X parhaat joukkeet
        //Needs to have at least DISTANCE filter
        // < team, average time >
        std::vector<std::pair<std::string, std::string>> data = m_DataHandler->getBestTenTeams(filters);

        if (data.size() > 0) {
            std::vector<std::vector<std::string>> newData = {};
            for (const auto& row : data) {
                std::vector<std::string> newRow = {};
                newRow.emplace_back(row.first); // Add year
                newRow.emplace_back(row.second); // Add time

                newData.emplace_back(newRow);
            }

            // Add to searches
            allSearches.emplace_back(newData);

            // Create title
            QString title = "Kymmenen parasta joukkuetta; " + makeNormalTitle();
            ui->listWidgetTehtHaut->addItem(title);
            m_titles.emplace_back(title);

            // Create header
            std::vector<std::string> head({});
            head.emplace_back(" Joukkue ");
            head.emplace_back(" Keskiaika ");
            m_headers.emplace_back(head);
        }
    }
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

bool Finlandia::check_for_order_filter()
{
    if(ui->jarjestaMatkaRP->isChecked()){

        return true;
    }

    if(ui->jarjestaVuosiRP->isChecked()){

        return true;
    }

    if(ui->jarjestaAlkupRP->isChecked()){
        return true;

    }

    if(ui->jarjestaSijoitusRP->isChecked()){
        return true;
    }

    return false;
}

std::vector<std::vector<std::string>> Finlandia::get_ordered_data(std::map<Filter_NS,
                                                                  QString> filter)
{
    if(ui->jarjestaSeuranNimiRP->isChecked()){
        std::vector<std::vector<std::string>> ordered_data =
                m_DataHandler->getDataWithFilter(filter,
                                                 InternetExplorers::Constants::
                                                 Filter::OrderFilters::ALPH_TEAM);
        return ordered_data;
    }

    if(ui->jarjestaKotimaaRP->isChecked()){
        std::vector<std::vector<std::string>> ordered_data =
                m_DataHandler->getDataWithFilter
                (filter, InternetExplorers::Constants::Filter::
                 OrderFilters::ALPH_NATIONALITY);

        return ordered_data;
    }

    if(ui->nimi_jarkkaRP->isChecked()){
        std::vector<std::vector<std::string>> ordered_data =
                m_DataHandler->getDataWithFilter
                (filter, InternetExplorers::Constants::Filter::
                 OrderFilters::ALPH_NAME);

        return ordered_data;
    }

    if(ui->jarjestaSeuranNimiRP->isChecked()){
        std::vector<std::vector<std::string>> ordered_data =
                m_DataHandler->getDataWithFilter
                (filter, InternetExplorers::Constants::Filter::
                 OrderFilters::ALPH_TEAM);

        return ordered_data;
    }

    if(ui->jarjestaMatkaRP->isChecked()){
        std::vector<std::vector<std::string>> ordered_data =
                m_DataHandler->getDataWithFilter
                (filter, InternetExplorers::Constants::Filter::
                 OrderFilters::DISTANCE_ORDER);

        return ordered_data;
    }

    if(ui->jarjestaVuosiRP->isChecked()){
        std::vector<std::vector<std::string>> ordered_data =
                m_DataHandler->getDataWithFilter
                (filter, InternetExplorers::Constants::Filter::
                 OrderFilters::YEAR_ORDER);

        return ordered_data;
    }

    if(ui->jarjestaSijoitusRP->isChecked()){
        std::vector<std::vector<std::string>> ordered_data =
                m_DataHandler->getDataWithFilter
                (filter, InternetExplorers::Constants::Filter::
                 OrderFilters::PLACEMENT);

        return ordered_data;
    }

    if(ui->jarjestaVainParasRP->isChecked()){

    }

    return {};
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

std::vector<int> Finlandia::select_attributes()
{
    std::vector<int>atr_vec;

    if(ui->haeKaikkiRP->isChecked()){
        atr_vec = {Atributes::year, Atributes::distance, Atributes::time,
                   Atributes::place, Atributes::place_men, Atributes::place_wm,
                   Atributes::sex, Atributes::name, Atributes::town,
                   Atributes::nationality, Atributes::birth_yr, Atributes::team};

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
    //Also makes curr_series_title
    std::map<Filter_NS, QString> filter = makefilter();

    // Special search
    if(check_for_special_filters()){

        // Handle data fetch, headers and titles
        apply_special_filters(filter);
    }

    // Normal search
    else {
        std::vector<std::vector<std::string>> newData = {};

        try {
            if(check_for_order_filter())
            {
                newData = get_ordered_data(filter);
            }
            else
            {
                newData = m_DataHandler->getDataWithFilter(filter);
            }
        }
        catch (InternetExplorers::FilterException &e) {
            qDebug() << e.what();
        }

        unsigned long long size = newData.size();

        if(size > 0){
            // Add the new data
            allSearches.push_back(newData);

            // Create the header
            createNormalHeader();

            // Create the title
            m_titles.emplace_back(makeNormalTitle());

            // Add the title to the list
            ui->listWidgetTehtHaut->addItem(m_titles.back());
        }
        else{
            QMessageBox msgBox;
            msgBox.setText("Hakuehdot eivät tuottaneet yhtäkään tulosta");
            msgBox.exec();
        }
    }
}



void Finlandia::on_pushButton_clicked()
{
    make_listview();
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

void Finlandia::createNormalHeader()
{
    std::vector<std::string> head({});

    if(ui->haeKaikkiRP->isChecked()){
        head.emplace_back(" Vuosi ");
        head.emplace_back(" Matka ");
        head.emplace_back(" Aika ");
        head.emplace_back(" Sija ");
        head.emplace_back(" Sija (miehet) ");
        head.emplace_back(" Sija (naiset) ");
        head.emplace_back(" Sukupuoli ");
        head.emplace_back(" Nimi ");
        head.emplace_back(" Kaupunki ");
        head.emplace_back(" Maa ");
        head.emplace_back(" Syntymävuosi ");
        head.emplace_back(" Joukkue ");
    }
    else {
        if(ui->hiihtajanNimiRP->isChecked()){
            head.emplace_back(" Nimi ");
        }
        if(ui->aikaRP->isChecked()){
            head.emplace_back(" Aika ");
        }
        if(ui->KotimaaRP->isChecked()){
            head.emplace_back(" Maa ");
        }
        if(ui->haeJoukkueRP->isChecked()){
            head.emplace_back(" Joukkue ");
        }
    }

    m_headers.emplace_back(head);
}

void Finlandia::addTableWidget(const std::vector<std::vector<std::string> > &rows,
                               const std::vector<std::string> &header,
                               const std::string& title)
{
    QTableWidget *w = new QTableWidget(ui->scrollWidget);

    w->setEditTriggers(QAbstractItemView::EditTriggers(nullptr));

    w->setRowCount(static_cast<int>(rows.size()) + 1);
    w->setColumnCount(static_cast<int>(header.size()));

    w->setSpan(0, 0, 1 , static_cast<int>(header.size()));

    // Add header
    int i = 0;
    for (const auto& col : header) {
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(col));
        w->setHorizontalHeaderItem(i, newItem);
        ++i;
    }

    // Add rows
    int r = 1;
    for (const auto& row : rows) {
        int c = 0;
        for (const auto& col : row) {
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(col));
            w->setItem(r, c, newItem);
            ++c;
        }
        ++r;
    }

    w->resizeColumnsToContents();

    // Add title, needs to be after column resize!
    QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(title));
    w->setItem(0, 0, newItem);

    w->resizeRowToContents(0);

    int sum = 100;
    for (int i = 0; i < w->columnCount(); ++i) {
        sum += w->columnWidth(i);
    }

    w->setMinimumWidth((sum > 500) ? 500 : sum);

    m_scrollLayout->addWidget(w);
}

QString Finlandia::makeNormalTitle()
{
    //The title for the search is made at the same time
    QString title = "";

    //Only one year is selected
    if (ui->comboBoxVuosi->currentIndex() != 0 &&
            ui->vuosivaliBox->currentIndex() == 0) {

        if (title.length() > 0) {
            title = title + ", Vuosi: " + ui->comboBoxVuosi->currentText();
        }
        else {
            title += "Vuosi: " + ui->comboBoxVuosi->currentText();
        }
    }

    else if (ui->comboBoxVuosi->currentIndex() != 0 &&
            ui->vuosivaliBox->currentIndex() != 0) {

        if (title.length() > 0) {
            title = title + ", Vuosiväli: " + ui->comboBoxVuosi->currentText() +
                    "-" +ui->vuosivaliBox->currentText();
        }
        else {
            title = "Vuosiväli: " + ui->comboBoxVuosi->currentText() + "-" +
                    ui->vuosivaliBox->currentText();
        }
    }

    if (ui->textEditUrheilija->toPlainText() != "") {
        if (title.length() > 0) {
            title = title + ", Nimi: " + ui->textEditUrheilija->toPlainText();
        }
        else {
            title = "Nimi: " + ui->textEditUrheilija->toPlainText();
        }
    }

    if (ui->comboBoxMatka->currentIndex() != 0) {

        if (title.length() > 0) {
            title = title + ", Matka: " + ui->comboBoxMatka->currentText();
        }
        else {
            title = "Matka: " + ui->comboBoxMatka->currentText();
        }
    }

    if (ui->timeEditLower->time().toString() != "00:00:00" ||
            ui->timeEditUpper->time().toString() != "00:00:00") {

        if (title.length() > 0) {
            title = title + ", Suoritusaikaväli: " + ui->timeEditLower->time().toString()
                    + "-" + ui->timeEditUpper->time().toString();
        }
        else {
            title = "Suoritusaikaväli: " + ui->timeEditLower->time().toString()
                    + "-" + ui->timeEditUpper->time().toString();
        }
    }

    if (ui->spinBoxSijoitusAla->value() != 0 &&
            ui->sukupuoliCB->currentIndex() == 0) { // PLACE has lower limit, no gender assigned

        if(ui->spinBoxSijoitusYla->value() == 0) { // PLACE does not have upper limit

            if (title.length() > 0  ) {
                title = title + ", Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }
            else {
                title = "Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }
        }
        else { // PLACE has upper limit
            if (title.length() > 0 ) {

                title = title + ", Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->
                                                                     value()) + "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }
            else {
                title = "Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->value())+
                        "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }
        }

    }

    if (ui->spinBoxSijoitusAla->value() != 0 &&
            ui->sukupuoliCB->currentText() == "N") { // PLACE has lower limit, gender is N

        if (ui->spinBoxSijoitusYla->value() == 0) { // PLACE does not have upper limit

            if (title.length() > 0 ) {
                title = title + ", Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }
            else {
                title = "Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }
        }
        else { // PLACE has upper limit
            if (title.length() > 0 ) {

                title = title + ", Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->
                                                                     value()) + "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }
            else {
                title = "Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->value())+
                        "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }
        }
    }

    if(ui->spinBoxSijoitusAla->value() != 0 &&
            ui->sukupuoliCB->currentText() == "M") { // PLACE has lower limit, gender is M

        if(ui->spinBoxSijoitusYla->value() == 0) { // PLACE does not have upper limit

            if (title.length() > 0 ) {
                title = title + ", Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }
            else {
                title = "Sijoitus: " + QString::number(ui->spinBoxSijoitusAla->value());
            }
        }
        else { // PLACE has upper limit
            if (title.length() > 0 ) {
                title = title + ", Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->
                                                                     value()) + "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }
            else {
                title = "Sijoitusväli: " + QString::number(ui->spinBoxSijoitusAla->value())+
                        "-" + QString::number(ui->spinBoxSijoitusYla->value());
            }
        }
    }

    if(ui->sukupuoliCB->currentIndex() != 0) { // No gender assigned

        if (title.length() > 0) {
            title = title + ", Sukupuoli:" + ui->sukupuoliCB->currentText();
        }
        else {
            title = "Sukupuoli:" + ui->sukupuoliCB->currentText();
        }
    }

    if (ui->textEditHome->toPlainText() != "") {

        if (title.length() > 0) {
            title = title + ", Kotimaa: " + ui->textEditHome->toPlainText();
        }
        else {
            title = "Kotimaa: " + ui->textEditHome->toPlainText();
        }
    }

    return title;
}


