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

    //Adding only latest search to list
    //std::vector<std::vector<std::string>> data =
    //allSearches.at(allSearches.size() -1);

    //Adding all the searches in line
    for(auto data : allSearches){

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

            }
            ui->listWidgetResult->addItem(disp);
        }
        ui->listWidgetResult->addItem(+ "\n");
        ui->hakuLabelTulokset->setText("Haku: " + curr_series_title);

    }
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
        std::vector<std::vector<std::string>> data = m_datalump;

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
        std::vector<std::vector<std::string>> data = m_datalump;

        // Going through individual results in a search:
        for(std::vector<std::string> result : data){
            if(!std::isdigit(*result.at(y).c_str()))
                continue;
            QBarSet *set = new QBarSet(QString::fromStdString(result.at(x)));


            //EI TOIMI
            if(y == 2){

                QDateTime yValue;
                yValue.setTime(QTime::fromString(QString::fromStdString(result.at(y))));

                *set << yValue.toMSecsSinceEpoch();
            }
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

int Finlandia::convert_time_to_seconds(std::string time)
{
    std::string hours = time.substr(0, 2);
    std::string minutes = time.substr(3, 2);
    std::string seconds = time.substr(6, 2);


    return stoi(hours)*60*60 + stoi(minutes)*60 + stoi(seconds);
}

void Finlandia::on_pushButtoLisaaHaku_clicked()
{

    std::map<Filter_NS, QString> filter;

    //Also makes curr_series_title
    filter = makefilter();

    m_search_queue.push_back(filter);

    ui->listWidgetTehtHaut->addItem(curr_series_title);

}

void Finlandia::make_search(std::map<Filter_NS, QString> filter)
{
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

        m_datalump.insert( m_datalump.end(), newData.begin(), newData.end());

        //m_all_searches.at(curr_series_title) = newData;
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Yksi tai useampi hakuehto ei tuottanut yhtäkään"
                       " tulosta.");
        msgBox.exec();
    }
}

void Finlandia::on_pushButton_clicked()
{
    m_datalump.clear();

    //Making all the searches in the queue
    for (std::map<Filter_NS, QString> filter : m_search_queue){
        make_search(filter);

    }

    m_search_queue.clear();

    make_listview();
    make_listviweLabel();
    curr_series_title = "";

}

void Finlandia::encryptionSettingsOpened()
{
    m_encryptionSettings = std::make_unique<EncryptionSettingsWindow>();
    m_encryptionSettings->setWindowModality(Qt::WindowModality::ApplicationModal);
    m_encryptionSettings->show();
    connect(m_encryptionSettings.get(), &EncryptionSettingsWindow::closeProgram, [&](){this->close();});

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
