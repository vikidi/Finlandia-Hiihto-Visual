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
#include <algorithm>

Finlandia::Finlandia(InternetExplorers::DataHandler* dh,
                     QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Finlandia),
    m_DataHandler(dh),
    allSearches{},
    m_chart(new QChart()),
    m_showHashes(false),
    m_localDataHashed(false)
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

    // Connects for attribute radio buttons
    connect(ui->haeKaikkiRP, &QRadioButton::clicked, this, &Finlandia::onAllAttributesRPClicked);
    connect(ui->naytaSPRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaSVRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaMaaRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaAikaRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaNimiRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaSijaRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaMatkaRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaSijaMRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaSijaNRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaVuosiRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaJoukkueRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);
    connect(ui->naytaKaupunkiRP, &QRadioButton::clicked, this, &Finlandia::onAttributeRPClicked);

    // Connects for order radio buttons
    connect(ui->jarjestaAlkupRP, &QRadioButton::clicked, this, &Finlandia::onOrderRPClicked);
    connect(ui->jarjestaMatkaRP, &QRadioButton::clicked, this, &Finlandia::onOrderRPClicked);
    connect(ui->jarjestaVuosiRP, &QRadioButton::clicked, this, &Finlandia::onOrderRPClicked);
    connect(ui->jarjestaKotimaaRP, &QRadioButton::clicked, this, &Finlandia::onOrderRPClicked);
    connect(ui->jarjestaSijoitusRP, &QRadioButton::clicked, this, &Finlandia::onOrderRPClicked);
    connect(ui->jarjestaSeuranNimiRP, &QRadioButton::clicked, this, &Finlandia::onOrderRPClicked);
    connect(ui->nimi_jarkkaRP, &QRadioButton::clicked, this, &Finlandia::onOrderRPClicked);

    // Connects for special function radio buttons
    connect(ui->haeHitainRP, &QRadioButton::clicked, this, &Finlandia::onSpecialRPClicked);
    connect(ui->haeNopeinRP, &QRadioButton::clicked, this, &Finlandia::onSpecialRPClicked);
    connect(ui->haeOsalMaarRP, &QRadioButton::clicked, this, &Finlandia::onSpecialRPClicked);
    connect(ui->osall_lkm_maittainRP, &QRadioButton::clicked, this, &Finlandia::onSpecialRPClicked);
    connect(ui->vuodenXparhaatRP, &QRadioButton::clicked, this, &Finlandia::onSpecialRPClicked);
    connect(ui->keskinopeusRP, &QRadioButton::clicked, this, &Finlandia::onSpecialRPClicked);
    connect(ui->keskiaikaRP, &QRadioButton::clicked, this, &Finlandia::onSpecialRPClicked);

    // Set chart to the graph view
    ui->graafiWiev->setChart(m_chart);

    // Set chart styles to be cool
    m_chart->setTheme(QChart::ChartThemeBlueCerulean);
    m_chart->setAnimationOptions(QChart::AnimationOption::SeriesAnimations);
    m_chart->setAnimationDuration(1500); // 1,5s
    ui->graafiWiev->setRenderHint(QPainter::Antialiasing, true);

    if (QFile::exists("encryptionStatus.ini")) {
        QFile file("encryptionStatus.ini");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            for(auto line : file.readAll().split('\n'))
            {
                // "Chopped" would be better than chop but
                // it is not supported on remote desktops qt version
                auto lineCopy{line};
                lineCopy.chop(2);
                if(lineCopy.size() == 0) continue;
                if(lineCopy == "ShowHashes")
                {
                    if(line.at(line.size() - 1) == '1')
                    {
                        m_showHashes = true;
                    }
                } else if(lineCopy == "LocalDataHashed")
                {
                    if(line.at(line.size() - 1) == '1')
                    {
                        m_showHashes = true;
                        m_localDataHashed = true;
                        break;
                    }
                }
            }
        }
    } else
    { // File not found
        m_showHashes = false;
        m_localDataHashed = false;
    }
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

    // Special radio buttons
    ui->haeHitainRP->setChecked(false);
    ui->haeNopeinRP->setChecked(false);
    ui->haeOsalMaarRP->setChecked(false);
    ui->osall_lkm_maittainRP->setChecked(false);
    ui->vuodenXparhaatRP->setChecked(false);
    ui->keskinopeusRP->setChecked(false);
    ui->keskiaikaRP->setChecked(false);

    // Radio buttons for attributes to show
    ui->haeKaikkiRP->setChecked(true);
    ui->naytaSPRP->setChecked(false);
    ui->naytaSVRP->setChecked(false);
    ui->naytaMaaRP->setChecked(false);
    ui->naytaAikaRP->setChecked(false);
    ui->naytaNimiRP->setChecked(false);
    ui->naytaSijaRP->setChecked(false);
    ui->naytaMatkaRP->setChecked(false);
    ui->naytaSijaMRP->setChecked(false);
    ui->naytaSijaNRP->setChecked(false);
    ui->naytaVuosiRP->setChecked(false);
    ui->naytaJoukkueRP->setChecked(false);
    ui->naytaKaupunkiRP->setChecked(false);

    // Order radio buttons
    ui->jarjestaAlkupRP->setChecked(true);
    ui->jarjestaMatkaRP->setChecked(false);
    ui->jarjestaVuosiRP->setChecked(false);
    ui->jarjestaKotimaaRP->setChecked(false);
    ui->jarjestaSijoitusRP->setChecked(false);
    ui->jarjestaSeuranNimiRP->setChecked(false);
    ui->nimi_jarkkaRP->setChecked(false);

    // Clear charts
    remove_cart();

    // Clear graph axis comboboxes
    ui->x_akseliCB->clear();
    ui->y_akseliCB->clear();

    // Set graph create button to disabled until searches are done
    ui->add_graphButton->setEnabled(false);
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
                    m_localDataHashed ?
                        m_crypter.hashName(ui->textEditUrheilija->toPlainText()) :
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

void Finlandia::make_chart()
{
    // Get the chosen axis
    QString xAxis = ui->x_akseliCB->currentText();
    QString yAxis = ui->y_akseliCB->currentText();

    // Get chosen graph type
    // 0 = bar, 1 = line
    int type = ui->kuvaajatyyppiCB->currentIndex();

    // Set graph labels
    ui->x_axisTitle->setText(ui->x_akseliCB->currentText());
    ui->y_axisTitle->setText(ui->y_akseliCB->currentText());

    // Create line chart
    if (type == 1) {
        make_line_chart(xAxis, yAxis);
    }
    // Create diagram chart
    else if (type == 0) {
        make_bar_chart(xAxis, yAxis);
    }
}


void Finlandia::make_bar_chart(QString xHeader, QString yHeader)
{
    //QBarSeries* series = new QBarSeries();
    ////series->setName(curr_series_title);
    //
    //// Going through individual results in a search:
    //for(std::vector<std::string> result : data){
    //    QBarSet *set = new QBarSet(QString::fromStdString(result.at(x)));
    //
    //    if(y == 2){
    //        int secs = QTime(0, 0, 0).secsTo(QTime::fromString
    //                                         (QString::fromStdString(
    //                                              result.at(y)), "h:mm:ss"));
    //        *set << secs;
    //
    //    }
    //    series->append(set);
    //
    //}
    ////Adding the series to m_chart
    //m_chart->addSeries(series);
    //
    //m_chart->createDefaultAxes();
}

void Finlandia::make_line_chart(QString xHeader, QString yHeader)
{
    // Headers need to be numerical
    std::vector<QString> allowedHeaders = {
        " Vuosi ",
        " Matka ",
        " Aika ",
        " Sija ",
        " Sija (miehet) ",
        " Sija (naiset) ",
        " Syntymävuosi "
    };

    // Check that headers are numerical
    std::vector<QString>::iterator it_x, it_y;
    it_x = std::find(allowedHeaders.begin(), allowedHeaders.end(), xHeader);
    it_y = std::find(allowedHeaders.begin(), allowedHeaders.end(), yHeader);
    if (it_x == allowedHeaders.end()
            || it_y == allowedHeaders.end()) {
        // Not allowed
        QMessageBox msgBox;
        msgBox.setText("Kuvaajan akseleiden täytyy olla numeeriset!");
        msgBox.exec();

        return;
    }

    // Go through searches, make each of them own series

    // Range for the axis
    double xMaxValue = INT_MIN;
    double xMinValue = UINT64_MAX;
    double yMaxValue = INT_MIN;
    double yMinValue = UINT64_MAX;

    unsigned long long i = 0; // Index of the current search
    for (auto search : allSearches) {
        QLineSeries *serie = new QLineSeries();

        // Get the indexes where data is for this serie data
        std::size_t xIndex = getHeadersIndex(xHeader.toStdString(), m_headers.at(i));
        std::size_t yIndex = getHeadersIndex(yHeader.toStdString(), m_headers.at(i));

        // Go through data
        for (auto row : search) {

            // Something went wrong :/
            if (xIndex > row.size() - 1
                    || yIndex > row.size() - 1) {
                continue;
            }

            // Get the data as string
            std::string xString = row[xIndex];
            std::string yString = row[yIndex];

            // Parse data to double
            double x, y;

            // Parse x
            if (xHeader == " Aika ") {
                long t = InternetExplorers::Helper::timeToMSecs(xString);
                x = InternetExplorers::Helper::mSecsToH(static_cast<unsigned long long>(t));
            }
            else if (xHeader == " Matka ") {
                x = InternetExplorers::Helper::parseKMFromDistance(xString);
            }
            else {
                try {
                    x = std::stod(xString);
                } catch (std::exception &e) {
                    continue;
                }
            }

            // Parse y
            if (yHeader == " Aika ") {
                // TODO: Error checking
                long t = InternetExplorers::Helper::timeToMSecs(yString);
                y = InternetExplorers::Helper::mSecsToH(static_cast<unsigned long long>(t));
            }
            else if (yHeader == " Matka ") {
                // TODO: Error checking
                y = InternetExplorers::Helper::parseKMFromDistance(yString);
            }
            else {
                try {
                    y = std::stod(yString);
                } catch (std::exception &e) {
                    continue;
                }
            }

            // Add data to serie
            serie->append(x, y);

            // Check axis boundaries
            if (x < xMinValue) {
                xMinValue = x;
            }
            else if (x > xMaxValue) {
                xMaxValue = x;
            }

            if (y < yMinValue) {
                yMinValue = y;
            }
            else if (y > yMaxValue) {
                yMaxValue = y;
            }
        }

        // Add title to the serie
        serie->setName(m_titles.at(i));

        //Adding the series to m_chart
        m_chart->addSeries(serie);

        ++i;
    }

    // Create axis
    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;

    // Get rounded ranges
    int xLowRange = InternetExplorers::Helper::getLowerFullTen(static_cast<int>(xMinValue));
    int xUpRange = InternetExplorers::Helper::getUpperFullTen(static_cast<int>(xMaxValue));

    int yLowRange = InternetExplorers::Helper::getLowerFullTen(static_cast<int>(yMinValue));
    int yUpRange = InternetExplorers::Helper::getUpperFullTen(static_cast<int>(yMaxValue));

    // Set ranges
    axisX->setRange(xLowRange, xUpRange);
    axisY->setRange(yLowRange, yUpRange);

    // Set main grid
    axisX->setTickCount(5);
    axisY->setTickCount(5);

    // Set minor grid
    axisX->setMinorTickCount(4);
    axisY->setMinorTickCount(4);

    // Set numbers to be nice
    axisX->applyNiceNumbers();
    axisY->applyNiceNumbers();

    // Set axis to the chart
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_chart->addAxis(axisY, Qt::AlignLeft);

    // Set axis to all series
    QList<QAbstractSeries*> series = m_chart->series();
    for (auto& serie : series) {
        serie->attachAxis(axisX);
        serie->attachAxis(axisY);
    }
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
        std::map<std::string, std::string> data = m_DataHandler->getAverageSpeeds(filters);

        if (data.size() > 0) {

            // Change data to row format
            std::vector<std::vector<std::string>> newData = {};
            for (const auto& row : data) {
                std::vector<std::string> newRow = {};
                newRow.emplace_back(row.first); // Add year
                newRow.emplace_back(row.second + " km/h"); // Add speed

                newData.emplace_back(newRow);
            }

            // Add to searches
            allSearches.emplace_back(newData);

            // Create title
            QString title = "Keskinopeus; " + makeNormalTitle();
            ui->listWidgetTehtHaut->addItem(title);
            m_titles.emplace_back(title);

            // Create header
            std::vector<std::string> head({});
            head.emplace_back(" Vuosi ");
            head.emplace_back(" Keskinopeus ");
            m_headers.emplace_back(head);
        }
    }

    if(ui->keskiaikaRP->isChecked()){
        // Needs to have at least DISTANCE filter
        // < year, average time >
        std::map<std::string, std::string> data = m_DataHandler->getAverageTimes(filters);

        if (data.size() > 0) {

            // Change data to row format
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
            head.emplace_back(" Keskiaika ");
            m_headers.emplace_back(head);
        }
    }

    if(ui->osall_lkm_maittainRP->isChecked()){

        std::map<std::string, int> data = m_DataHandler->getParticipantsByCountry(filters);

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
            QString title = "Osallistujien määrä maittain; " + makeNormalTitle();
            ui->listWidgetTehtHaut->addItem(title);
            m_titles.emplace_back(title);

            // Create header
            std::vector<std::string> head({});
            head.emplace_back(" Kansalaisuus ");
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

            // Change data to row format
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
    if(ui->haeOsalMaarRP->isChecked()) {
        return true;
    }

    if(ui->haeHitainRP->isChecked()) {
        return true;
    }

    if( ui->haeNopeinRP->isChecked() ) {
        return true;
    }

    if(ui->keskinopeusRP->isChecked()) {
        return true;
    }

    if(ui->osall_lkm_maittainRP->isChecked()) {
        return true;
    }

    if(ui->vuodenXparhaatRP->isChecked()) {
        return true;
    }

    if(ui->keskiaikaRP->isChecked()) {
        return true;
    }

    return false;
}

bool Finlandia::check_for_order_filter()
{
    if (ui->jarjestaAlkupRP->isChecked()) {
        // No need to order
        return false;
    }

    if(ui->jarjestaMatkaRP->isChecked()) {
        return true;
    }

    if(ui->jarjestaVuosiRP->isChecked()) {
        return true;
    }

    if(ui->jarjestaAlkupRP->isChecked()) {
        return true;
    }

    if(ui->jarjestaSijoitusRP->isChecked()) {
        return true;
    }

    if(ui->jarjestaKotimaaRP->isChecked()) {
        return true;
    }

    if(ui->jarjestaSeuranNimiRP->isChecked()) {
        return true;
    }

    return false;
}

std::vector<std::vector<std::string>> Finlandia::get_ordered_data(std::map<Filter_NS,
                                                                  QString> filter)
{
    if (ui->jarjestaAlkupRP->isChecked()) {
        // No need to order
        return {};
    }

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

    return {};
}


void Finlandia::remove_cart()
{
    // Set default axis titles
    ui->x_axisTitle->setText("X-Axis Title");
    ui->y_axisTitle->setText("Y-Axis Title");

    // Clear charts
    m_chart->removeAllSeries();

    // Clear axis
    QList<QAbstractAxis*> axes = m_chart->axes();
    for (auto axis : axes) {
        m_chart->removeAxis(axis);
    }
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
    if(check_for_special_filters()){
        return {};
    }

    std::vector<int> atr_vec = {};

    // All attributes
    if(ui->haeKaikkiRP->isChecked()){
        for (std::size_t i = 0; i < InternetExplorers::Constants::DataIndex::ROW_SIZE; ++i) {
            atr_vec.emplace_back(static_cast<int>(i));
        }
        return atr_vec;
    }

    // Individual attribute choices
    if (ui->naytaSPRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::SEX);
    }

    if (ui->naytaSVRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::BIRTH_YEAR);
    }

    if (ui->naytaMaaRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::NATIONALITY);
    }

    if (ui->naytaAikaRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::TIME);
    }

    if (ui->naytaNimiRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::NAME);
    }

    if (ui->naytaSijaRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::PLACE);
    }

    if (ui->naytaMatkaRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::DISTANCE);
    }

    if (ui->naytaSijaMRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::PLACE_MEN);
    }

    if (ui->naytaSijaNRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::PLACE_WOMEN);
    }

    if (ui->naytaVuosiRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::YEAR);
    }

    if (ui->naytaJoukkueRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::TEAM);
    }

    if (ui->naytaKaupunkiRP->isChecked()) {
        atr_vec.emplace_back(InternetExplorers::Constants::DataIndex::IndexInData::CITY);
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

        if (size > 0) {

            // Parse away some columns
            if (!ui->haeKaikkiRP->isChecked()) {
                std::vector<int> attrVec = select_attributes();
                parseColumns(newData, attrVec);
            }

            // Add the new data
            allSearches.push_back(newData);

            // Create the header
            createNormalHeader();

            // Create the title
            m_titles.emplace_back(makeNormalTitle() + makeOrderTitle());

            // Add the title to the list
            ui->listWidgetTehtHaut->addItem(m_titles.back());
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Hakuehdot eivät tuottaneet yhtäkään tulosta");
            msgBox.exec();
        }
    }
}

void Finlandia::on_pushButton_clicked()
{
    // Create the tables
    make_listview();

    // Create graph things if enough headers
    std::vector<std::string> headers = getCommonHeaders();
    if (headers.size() > 1) {
        setGraphAxisCB(headers);

        // Allow graphs
        ui->add_graphButton->setEnabled(true);
    }
    else {
        // Don't allow graphs to be made
        ui->add_graphButton->setEnabled(false);

        QMessageBox msgBox;
        msgBox.setText("Hauissa liian vähän yhteisiä tietueita kuvaajia varten");
        msgBox.exec();
    }
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

void Finlandia::onAttributeRPClicked()
{
    if (ui->haeKaikkiRP->isChecked()) {
        ui->haeKaikkiRP->setChecked(false);

        // These cant be used with attribure restricting
        ui->haeHitainRP->setChecked(false);
        ui->haeNopeinRP->setChecked(false);
        ui->haeOsalMaarRP->setChecked(false);
        ui->osall_lkm_maittainRP->setChecked(false);
        ui->vuodenXparhaatRP->setChecked(false);
        ui->keskinopeusRP->setChecked(false);
        ui->keskiaikaRP->setChecked(false);
    }

    // All attribute choices off -> set RB all on
    if (!(ui->naytaSPRP->isChecked()
        || ui->naytaSVRP->isChecked()
        || ui->naytaMaaRP->isChecked()
        || ui->naytaAikaRP->isChecked()
        || ui->naytaNimiRP->isChecked()
        || ui->naytaSijaRP->isChecked()
        || ui->naytaMatkaRP->isChecked()
        || ui->naytaSijaMRP->isChecked()
        || ui->naytaSijaNRP->isChecked()
        || ui->naytaVuosiRP->isChecked()
        || ui->naytaJoukkueRP->isChecked()
        || ui->naytaKaupunkiRP->isChecked())) {
        ui->haeKaikkiRP->setChecked(true);
    }
}

void Finlandia::onAllAttributesRPClicked()
{
    ui->naytaSPRP->setChecked(false);
    ui->naytaSVRP->setChecked(false);
    ui->naytaMaaRP->setChecked(false);
    ui->naytaAikaRP->setChecked(false);
    ui->naytaNimiRP->setChecked(false);
    ui->naytaSijaRP->setChecked(false);
    ui->naytaMatkaRP->setChecked(false);
    ui->naytaSijaMRP->setChecked(false);
    ui->naytaSijaNRP->setChecked(false);
    ui->naytaVuosiRP->setChecked(false);
    ui->naytaJoukkueRP->setChecked(false);
    ui->naytaKaupunkiRP->setChecked(false);
}

void Finlandia::onSpecialRPClicked()
{
    if (!ui->haeKaikkiRP->isChecked()) {
        ui->haeKaikkiRP->setChecked(true);

        // These cant be used with special functions
        ui->naytaSPRP->setChecked(false);
        ui->naytaSVRP->setChecked(false);
        ui->naytaMaaRP->setChecked(false);
        ui->naytaAikaRP->setChecked(false);
        ui->naytaNimiRP->setChecked(false);
        ui->naytaSijaRP->setChecked(false);
        ui->naytaMatkaRP->setChecked(false);
        ui->naytaSijaMRP->setChecked(false);
        ui->naytaSijaNRP->setChecked(false);
        ui->naytaVuosiRP->setChecked(false);
        ui->naytaJoukkueRP->setChecked(false);
        ui->naytaKaupunkiRP->setChecked(false);
    }

    if (!ui->jarjestaAlkupRP->isChecked()) {
        // This also sets the other order buttons to false
        ui->jarjestaAlkupRP->setChecked(true);
    }
}

void Finlandia::onOrderRPClicked()
{
    if (!ui->jarjestaAlkupRP->isChecked()) {

        // These cant be used with order filtering
        ui->haeHitainRP->setChecked(false);
        ui->haeNopeinRP->setChecked(false);
        ui->haeOsalMaarRP->setChecked(false);
        ui->osall_lkm_maittainRP->setChecked(false);
        ui->vuodenXparhaatRP->setChecked(false);
        ui->keskinopeusRP->setChecked(false);
        ui->keskiaikaRP->setChecked(false);
    }
}

void Finlandia::createNormalHeader()
{
    std::vector<std::string> head({});

    if (ui->haeKaikkiRP->isChecked()) {
        head.emplace_back(" Vuosi ");
        head.emplace_back(" Matka ");
        head.emplace_back(" Aika ");
        head.emplace_back(" Sija ");
        head.emplace_back(" Sija (miehet) ");
        head.emplace_back(" Sija (naiset) ");
        head.emplace_back(" Sukupuoli ");
        head.emplace_back(" Nimi ");
        head.emplace_back(" Kaupunki ");
        head.emplace_back(" Kansalaisuus ");
        head.emplace_back(" Syntymävuosi ");
        head.emplace_back(" Joukkue ");
    }
    else {
        if (ui->naytaVuosiRP->isChecked()) {
            head.emplace_back(" Vuosi ");
        }

        if (ui->naytaMatkaRP->isChecked()) {
            head.emplace_back(" Matka ");
        }

        if (ui->naytaAikaRP->isChecked()) {
            head.emplace_back(" Aika ");
        }

        if (ui->naytaSijaRP->isChecked()) {
            head.emplace_back(" Sija ");
        }

        if (ui->naytaSijaMRP->isChecked()) {
            head.emplace_back(" Sija (miehet) ");
        }

        if (ui->naytaSijaNRP->isChecked()) {
            head.emplace_back(" Sija (naiset) ");
        }

        if (ui->naytaSPRP->isChecked()) {
            head.emplace_back(" Sukupuoli ");
        }

        if (ui->naytaNimiRP->isChecked()) {
            head.emplace_back(" Nimi ");
        }

        if (ui->naytaKaupunkiRP->isChecked()) {
            head.emplace_back(" Kaupunki ");
        }

        if (ui->naytaMaaRP->isChecked()) {
            head.emplace_back(" Kansalaisuus ");
        }

        if (ui->naytaSVRP->isChecked()) {
            head.emplace_back(" Syntymävuosi ");
        }

        if (ui->naytaJoukkueRP->isChecked()) {
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

    // Make table not editable
    w->setEditTriggers(QAbstractItemView::EditTriggers(nullptr));

    w->setRowCount(static_cast<int>(rows.size()) + 1);
    w->setColumnCount(static_cast<int>(header.size()));

    // Set the first ("title") row to be full width
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

    // Resize data rows
    w->resizeColumnsToContents();

    // Add title row, needs to be after column resize!
    QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(title));
    w->setItem(0, 0, newItem);

    // Resize title row
    w->resizeRowToContents(0);

    // Calculate table width
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
            title = title + ", Nimi: " + (m_showHashes ? m_crypter.hashName(ui->textEditUrheilija->toPlainText()) : ui->textEditUrheilija->toPlainText());
        }
        else {
            title = "Nimi: " + (m_showHashes ? m_crypter.hashName(ui->textEditUrheilija->toPlainText()) : ui->textEditUrheilija->toPlainText());
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

QString Finlandia::makeOrderTitle()
{
    if (ui->jarjestaAlkupRP->isChecked()) {
        return "";
    }

    QString order = "; Järjestys: ";

    if (ui->jarjestaMatkaRP->isChecked()) {
        return order + "matkan mukaan";
    }

    if (ui->jarjestaVuosiRP->isChecked()) {
        return order + "vuoden mukaan";
    }

    if (ui->jarjestaKotimaaRP->isChecked()) {
        return order + "kotimaan mukaan";
    }

    if (ui->jarjestaSijoitusRP->isChecked()) {
        return order + "sijoituksen mukaan";
    }

    if (ui->jarjestaSeuranNimiRP->isChecked()) {
        return order + "joukkueen mukaan";
    }

    if (ui->nimi_jarkkaRP->isChecked()) {
        return order + "nimen mukaan";
    }

    return "";
}

void Finlandia::parseColumns(std::vector<std::vector<std::string> > &data, const std::vector<int> &columns)
{
    std::vector<int>::const_iterator it_c;

    for (std::size_t r = 0; r < data.size(); ++r) {

        auto& row = data.at(r);

        int i = 0;
        auto it = row.begin();
        while (it != row.end()) {

            it_c = std::find(columns.begin(), columns.end(), i);
            if (it_c != columns.end()) {
                // Don't remove
                ++it;
            }
            else {
                // Remove
                it = row.erase(it);
            }

            ++i;
        }
    }
}

std::vector<std::string> Finlandia::getCommonHeaders()
{
    // No searches made
    if (m_headers.size() == 0) return {};

    // Take the first one as reference
    std::vector<std::string> com = m_headers.at(0);

    // Only header
    if (m_headers.size() == 1) return com;

    std::vector<std::string>::iterator it_c;

    // Go through other headers
    auto it_h = ++m_headers.begin();
    while (it_h != m_headers.end()) {

        // Check headers in the first header
        auto it = com.begin();
        while (it != com.end()) {

            // Try to find the header
            it_c = std::find((*it_h).begin(), (*it_h).end(), (*it));
            if (it_c != (*it_h).end()) {
                // Don't remove
                ++it;
            }
            else {
                // Remove
                it = com.erase(it);
            }
        }

        ++it_h;
    }

    return com;
}

void Finlandia::setGraphAxisCB(const std::vector<std::string> &values)
{
    for (const auto& value : values) {
        ui->x_akseliCB->addItem(QString::fromStdString(value));
        ui->y_akseliCB->addItem(QString::fromStdString(value));
    }
}

std::size_t Finlandia::getHeadersIndex(const std::string &header, const std::vector<std::string> &headers) const
{
    // Try to find the header
    std::vector<std::string>::const_iterator it = std::find(headers.begin(), headers.end(), header);
    if (it != headers.end()) {
        // Get index of element from iterator
        return static_cast<std::size_t>(std::distance(headers.begin(), it));
    }
    // Not found
    else {
        return UINT64_MAX;
    }
}
