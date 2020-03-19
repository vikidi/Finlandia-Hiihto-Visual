#include "datahandler.h"

#include <iostream>
#include <QDebug>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QNetworkProxy>
#include <QFile>
#include <sstream>
#include <QXmlStreamReader>
#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>
#include <set>
#include <algorithm>
#include <cctype>
#include <QTime>
#include "logger.h"

InternetExplorers::DataHandler::DataHandler():
    m_loadOngoing(false),
    m_finlandiaAPI(new InternetExplorers::FinlandiaAPI),
    m_localAPI(new InternetExplorers::LocalAPI),
    m_data({}),
    m_dataByName({})
{
    // For progress
    connect(m_finlandiaAPI, &InternetExplorers::FinlandiaAPI::progressChanged, this, &DataHandler::progressChangedInApi);
    connect(m_localAPI, &InternetExplorers::LocalAPI::progressChanged, this, &DataHandler::progressChangedInApi);

    auto msg(QString("Constructor ready"));
    auto msgSender(QString("DataHandler"));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);
}

InternetExplorers::DataHandler::~DataHandler()
{
    auto msg(QString("Destructor called"));
    auto msgSender(QString("DataHandler"));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);

    delete m_finlandiaAPI;
    delete m_localAPI;
}

void InternetExplorers::DataHandler::Initialize()
{
    if (!m_loadOngoing) {
        auto msg(QString("Loading data"));
        auto msgSender(QString("DataHandler"));
        InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);

        m_loadOngoing = true;
        loadData();

        msg = QString("Loading finished");
        msgSender = QString("DataHandler");
        InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);
    }
}

std::vector<std::vector<std::string> > InternetExplorers::DataHandler::getDataWithFilter(std::map<Constants::Filter::ValueFilters, QString> filters)
{
    std::vector<std::vector<std::string>> data = {};

    // Check filter validity
    try {
        if (!InterfaceFilter::validateFilter(filters)) {
            auto msg(QString("Filter did not pass validation"));
            auto msgSender(QString("DataHandler"));
            InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::WARNING, msgSender);

            return {};
        }
    } catch (InternetExplorers::FilterException& e) {
        auto msg(QString(e.what()));
        auto msgSender(QString("DataHandler"));
        InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::WARNING, msgSender);

        return {};
    }

    if (filters.size() == 0) {

        auto msg(QString("Filtering values with no filter"));
        auto msgSender(QString("DataHandler"));
        InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::WARNING, msgSender);

        for(auto yearMap : m_data)
        {
            for(auto tripMap : yearMap.second)
            {
                for(auto row : tripMap.second)
                {
                    data.push_back(row);
                }
            }
        }
        return data;
    }

    // Do the filtering

    // Try to do first filterin with fast filters

    // Filter first by year if allowed
    if (filters.find(Constants::Filter::ValueFilters::YEAR) != filters.end()) {
        data = getAllByYear(filters[Constants::Filter::ValueFilters::YEAR]);

        // Remove year filter
        filters.erase(Constants::Filter::ValueFilters::YEAR);

        // Apply rest of filters
        if (filters.size() != 0 && data.size() != 0) {
            applyFilterToData(filters, data);
        }
        return data;
    }

    // If not, filter by year range if allowed
    else if (filters.find(Constants::Filter::ValueFilters::YEAR_RANGE) != filters.end()) {
        data = getAllByYearRange(filters[Constants::Filter::ValueFilters::YEAR_RANGE]);

        // Remove year filter
        filters.erase(Constants::Filter::ValueFilters::YEAR_RANGE);

        // Apply rest of filters
        if (filters.size() != 0 && data.size() != 0) {
            applyFilterToData(filters, data);
        }
        return data;
    }

    // If not, filter by distances if allowed
    else if (filters.find(Constants::Filter::ValueFilters::DISTANCE) != filters.end()) {
        data = getAllByDistance(filters[Constants::Filter::ValueFilters::DISTANCE]);

        // Remove year filter
        filters.erase(Constants::Filter::ValueFilters::DISTANCE);

        // Apply rest of filters
        if (filters.size() != 0 && data.size() != 0) {
            applyFilterToData(filters, data);
        }
        return data;
    }

    // If not, filter by name if allowed
    else if (filters.find(Constants::Filter::ValueFilters::NAME) != filters.end()) {
        data = getAllByName(filters[Constants::Filter::ValueFilters::NAME]);

        // Remove year filter
        filters.erase(Constants::Filter::ValueFilters::NAME);

        // Apply rest of filters
        if (filters.size() != 0 && data.size() != 0) {
            applyFilterToData(filters, data);
        }
        return data;
    }

    // If not, filter each row in data. Quite slow ATM!
    else {
        // Go through data row by row and apply all filters to each row
        // If row passes, add it to result data
        for(auto& year : m_data) {
            for(auto& distance : year.second) {
                for(auto& row : distance.second) {
                    if (applyAllFiltersToRow(filters, row)) {
                        data.emplace_back(row);
                    }
                }
            }
        }
    }

    return data;
}

void InternetExplorers::DataHandler::applyFilterToData(std::map<Constants::Filter::ValueFilters, QString> filters, std::vector<std::vector<std::string> > &data)
{
    // Check filter validity
    try {
        if (!InterfaceFilter::validateFilter(filters)) {
            auto msg(QString("Filter did not pass validation"));
            auto msgSender(QString("DataHandler"));
            InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::WARNING, msgSender);
            return;
        }
    } catch (InternetExplorers::FilterException& e) {
        auto msg(QString(e.what()));
        auto msgSender(QString("DataHandler"));
        InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::WARNING, msgSender);
        return;
    }

    if (filters.size() == 0 || data.size() == 0) {
        return;
    }

    // Do the filtering
    std::vector<std::vector<std::string>> resultData = {};

    // Go through each row and apply all filters to each one
    // If row passess them all, add it to data
    for(auto& row : data) {
        if (applyAllFiltersToRow(filters, row)) {
            resultData.emplace_back(row);
        }
    }

    data = resultData;

    return;
}

void InternetExplorers::DataHandler::progressChangedInApi(const int progress)
{
    emit progressChanged(progress);
}

void InternetExplorers::DataHandler::loadData()
{
    std::thread t(&DataHandler::loadInThread, this);
    t.detach();
}

void InternetExplorers::DataHandler::loadInThread()
{
    // CLOCKING
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    // Load data either from web or local
    // depending if local data is available
    if (m_localAPI->needsToBeLoadedFromWeb()) {
       m_data = m_finlandiaAPI->loadAllData();

       // Save loaded data in local storage for later use
       m_localAPI->saveData(m_data);
    } else {
        m_data = m_localAPI->loadData();
    }

    setRowsByName();

    // CLOCKING
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Data fetch done in " << time_span.count() << " s" << std::endl;

    m_loadOngoing = false;
    emit loadingReady();
}

void InternetExplorers::DataHandler::setRowsByName()
{
    for(auto& year : m_data) {
        for(auto& distance : year.second) {
            for(auto& row : distance.second) {

                // Add names as lower case
                std::string name = row[Constants::DataIndex::IndexInData::NAME];
                std::transform(name.begin(), name.end(), name.begin(),
                    [](unsigned char c){ return std::tolower(c); });

                // Add the row
                m_dataByName[QString::fromStdString(name)].emplace_back(row);
            }
        }
    }
}

bool InternetExplorers::DataHandler::applyAllFiltersToRow(std::map<Constants::Filter::ValueFilters, QString> filters, std::vector<std::string> row)
{
    bool passes = true;

    // Apply each filter to the row
    for(auto& filter : filters) {
        switch (filter.first) {
        case Constants::Filter::ValueFilters::YEAR:
            passes = filterByYear(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::DISTANCE:
            passes = filterByDistance(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::YEAR_RANGE:
            passes = filterByYearRange(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::NAME:
            passes = filterByName(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::TIME_RANGE:
            passes = filterByTimeRange(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::PLACE:
            passes = filterByPlace(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::PLACE_MEN:
            passes = filterByPlaceMen(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::PLACE_WOMEN:
            passes = filterByPlaceWomen(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::SEX:
            passes = filterBySex(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::CITY:
            passes = filterByCity(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::NATIONALITY:
            passes = filterByNationality(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::BIRTH_YEAR:
            passes = filterByBirthYear(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::TEAM:
            passes = filterByTeam(row, filter.second);
            break;
        }

        if (!passes) {
            break;
        }
    }

    return passes;
}

std::vector<std::vector<std::string> > InternetExplorers::DataHandler::getAllByYear(QString year)
{
    std::vector<std::vector<std::string>> data = {};

    // Year not found from data
    if (m_data.find(year) == m_data.end()) {
        return {};
    }

    // Go through distances
    for(auto& dist : m_data[year]) {
        // Add rows to end of data
        data.insert( data.end(), dist.second.begin(), dist.second.end() );
    }

    return data;
}

std::vector<std::vector<std::string> > InternetExplorers::DataHandler::getAllByYearRange(QString range)
{
    std::vector<std::vector<std::string>> data = {};

    // Should be in style firstYear;secondYear eg. 2014;2018
    QStringList years = range.split(";");

    QString lower = years[0];
    QString upper = years[1];

    // Go through given years
    for (int i = lower.toInt(); i <= upper.toInt(); i++) {

        // Year not found from data
        if (m_data.find(QString::number(i)) == m_data.end()) {
            continue;
        }

        // Go through distances
        for(auto& dist : m_data[QString::number(i)]) {
            // Add rows to end of data
            data.insert( data.end(), dist.second.begin(), dist.second.end() );
        }
    }

    return data;
}

std::vector<std::vector<std::string> > InternetExplorers::DataHandler::getAllByDistance(QString distance)
{
    std::vector<std::vector<std::string>> data = {};

    for (auto& year : m_data) {

        // Distance not found from years data
        if (year.second.find(distance) == year.second.end()) {
            continue;
        }

        // Get rows
        std::vector<std::vector<std::string>> rows = year.second[distance];

        // Add rows to end of data
        data.insert( data.end(), rows.begin(), rows.end() );
    }

    return data;
}

std::vector<std::vector<std::string> > InternetExplorers::DataHandler::getAllByName(QString name)
{
    // name to lower case
    name = name.toLower();

    std::unordered_map<QString, std::vector<std::vector<std::string>>, QStringKeyHash>::iterator it = m_dataByName.find(name);
    if (it != m_dataByName.end()) {
        return it->second;
    }

    // No name found
    return {};
}

bool InternetExplorers::DataHandler::filterByYear(std::vector<std::string> row, QString filterValue)
{
    if (row[Constants::DataIndex::IndexInData::YEAR] == filterValue.toStdString()) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByYearRange(std::vector<std::string> row, QString filterValue)
{
    // Should be in style firstYear;secondYear eg. 2014;2018
    QStringList years = filterValue.split(";");

    QString lower = years[0];
    QString upper = years[1];

    std::string year = row[Constants::DataIndex::IndexInData::YEAR];

    if (year <= upper.toStdString() && year >= lower.toStdString()) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByDistance(std::vector<std::string> row, QString filterValue)
{
    if (row[Constants::DataIndex::IndexInData::DISTANCE] == filterValue.toStdString()) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByName(std::vector<std::string> row, QString filterValue)
{
    // Filter value to lower case
    std::string filterVal = filterValue.toStdString();
    std::transform(filterVal.begin(), filterVal.end(), filterVal.begin(),
        [](unsigned char c){ return std::tolower(c); });

    std::string name = row[Constants::DataIndex::IndexInData::NAME];
    std::transform(name.begin(), name.end(), name.begin(),
        [](unsigned char c){ return std::tolower(c); });

    // ATM needs to be exact
    if (name == filterVal) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByTimeRange(std::vector<std::string> row, QString filterValue)
{
    QStringList times = filterValue.split(";");

    QTime lower = QTime::fromString(times[0], "h:mm:ss");
    QTime upper = QTime::fromString(times[1], "h:mm:ss");

    QTime rowValue = QTime::fromString(QString::fromStdString(row[Constants::DataIndex::IndexInData::TIME]), "h:mm:ss");

    if (rowValue >= lower && rowValue <= upper) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByPlace(std::vector<std::string> row, QString filterValue)
{
    if (row[Constants::DataIndex::IndexInData::PLACE] == filterValue.toStdString()) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByPlaceMen(std::vector<std::string> row, QString filterValue)
{
    if (row[Constants::DataIndex::IndexInData::PLACE_MEN] == filterValue.toStdString()) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByPlaceWomen(std::vector<std::string> row, QString filterValue)
{
    if (row[Constants::DataIndex::IndexInData::PLACE_WOMEN] == filterValue.toStdString()) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterBySex(std::vector<std::string> row, QString filterValue)
{
    if (row[Constants::DataIndex::IndexInData::SEX] == filterValue.toStdString()) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByCity(std::vector<std::string> row, QString filterValue)
{
    // Filter value to lower case
    std::string filterVal = filterValue.toStdString();
    std::transform(filterVal.begin(), filterVal.end(), filterVal.begin(),
        [](unsigned char c){ return std::tolower(c); });

    std::string city = row[Constants::DataIndex::IndexInData::CITY];
    std::transform(city.begin(), city.end(), city.begin(),
        [](unsigned char c){ return std::tolower(c); });

    // ATM needs to be exact
    if (city == filterVal) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByNationality(std::vector<std::string> row, QString filterValue)
{
    if (row[Constants::DataIndex::IndexInData::NATIONALITY] == filterValue.toStdString()) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByBirthYear(std::vector<std::string> row, QString filterValue)
{
    if (row[Constants::DataIndex::IndexInData::BIRTH_YEAR] == filterValue.toStdString()) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByTeam(std::vector<std::string> row, QString filterValue)
{
    // Filter value to lower case
    std::string filterVal = filterValue.toStdString();
    std::transform(filterVal.begin(), filterVal.end(), filterVal.begin(),
        [](unsigned char c){ return std::tolower(c); });

    std::string team = row[Constants::DataIndex::IndexInData::TEAM];
    std::transform(team.begin(), team.end(), team.begin(),
        [](unsigned char c){ return std::tolower(c); });

    // ATM needs to be exact
    if (team == filterVal) {
        return true;
    }

    return false;
}
