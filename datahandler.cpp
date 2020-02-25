#include "datahandler.h"

#include <iostream>
#include <QDebug>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QNetworkProxy>
#include <QFile>
#include <string>
#include <sstream>
#include <vector>
#include <QXmlStreamReader>
#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>
#include <set>
#include <algorithm>
#include <cctype>

InternetExplorers::DataHandler::DataHandler():
    m_loadOngoing(false),
    m_finlandiaAPI(new InternetExplorers::FinlandiaAPI),
    m_localAPI(new InternetExplorers::LocalAPI),
    m_data({}),
    m_dataDyName({})
{
    // For progress
    connect(m_finlandiaAPI, &InternetExplorers::FinlandiaAPI::progressChanged, this, &DataHandler::progressChangedInApi);
}

InternetExplorers::DataHandler::~DataHandler()
{
    delete m_finlandiaAPI;
    delete m_localAPI;
}

void InternetExplorers::DataHandler::Initialize()
{
    if (!m_loadOngoing) {
        m_loadOngoing = true;
        loadData();
    }
}

std::vector<std::vector<std::string> > InternetExplorers::DataHandler::getDataWithFilter(std::map<InterfaceFilter::ValueFilters, QString> filters)
{
    // Check filter validity
    try {
        if (!InterfaceFilter::validateFilter(filters)) {
            return {};
        }
    } catch (InternetExplorers::FilterException) {
        return {};
    }

    if (filters.size() == 0) {
        // TODO Really return everything??
        return {};
    }

    // Do the filtering
    std::vector<std::vector<std::string>> data = {};

    // Try to do first filterin with fast filters

    // Filter first by year if allowed
    if (filters.find(InternetExplorers::InterfaceFilter::ValueFilters::YEAR) != filters.end()) {
        data = getAllByYear(filters[InternetExplorers::InterfaceFilter::ValueFilters::YEAR]);

        // Remove year filter
        filters.erase(InternetExplorers::InterfaceFilter::ValueFilters::YEAR);

        // Apply rest of filters
        if (filters.size() != 0 && data.size() != 0) {
            applyFilterToData(filters, data);
        }
        return data;
    }

    // If not, filter by year range if allowed
    else if (filters.find(InternetExplorers::InterfaceFilter::ValueFilters::YEAR_RANGE) != filters.end()) {
        data = getAllByYearRange(filters[InternetExplorers::InterfaceFilter::ValueFilters::YEAR_RANGE]);

        // Remove year filter
        filters.erase(InternetExplorers::InterfaceFilter::ValueFilters::YEAR_RANGE);

        // Apply rest of filters
        if (filters.size() != 0 && data.size() != 0) {
            applyFilterToData(filters, data);
        }
        return data;
    }

    // If not, filter by distances if allowed
    else if (filters.find(InternetExplorers::InterfaceFilter::ValueFilters::DISTANCE) != filters.end()) {
        data = getAllByDistance(filters[InternetExplorers::InterfaceFilter::ValueFilters::DISTANCE]);

        // Remove year filter
        filters.erase(InternetExplorers::InterfaceFilter::ValueFilters::DISTANCE);

        // Apply rest of filters
        if (filters.size() != 0 && data.size() != 0) {
            applyFilterToData(filters, data);
        }
        return data;
    }

    // If not, filter by name if allowed
    else if (filters.find(InternetExplorers::InterfaceFilter::ValueFilters::NAME) != filters.end()) {
        data = getAllByName(filters[InternetExplorers::InterfaceFilter::ValueFilters::NAME]);

        // Remove year filter
        filters.erase(InternetExplorers::InterfaceFilter::ValueFilters::NAME);

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

void InternetExplorers::DataHandler::applyFilterToData(std::map<InterfaceFilter::ValueFilters, QString> filters, std::vector<std::vector<std::string> > &data)
{
    // Check filter validity
    try {
        if (!InterfaceFilter::validateFilter(filters)) {
            return;
        }
    } catch (InternetExplorers::FilterException) {
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
                std::string name = row[IndexInData::NAME];
                std::transform(name.begin(), name.end(), name.begin(),
                    [](unsigned char c){ return std::tolower(c); });

                // Add the row
                m_dataDyName[QString::fromStdString(name)].emplace_back(row);
            }
        }
    }
}

bool InternetExplorers::DataHandler::applyAllFiltersToRow(std::map<InternetExplorers::InterfaceFilter::ValueFilters, QString> filters, std::vector<std::string> row)
{
    bool passes = true;

    // Apply each filter to the row
    for(auto& filter : filters) {
        switch (filter.first) {
        case InternetExplorers::InterfaceFilter::ValueFilters::YEAR:
            passes = filterByYear(row, filter.second);
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::DISTANCE:
            passes = filterByDistance(row, filter.second);
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::YEAR_RANGE:
            passes = filterByYearRange(row, filter.second);
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::NAME:
            passes = filterByName(row, filter.second);
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::TIME_RANGE:
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::PLACE:
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::PLACE_MEN:
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::PLACE_WOMEN:
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::SEX:
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::CITY:
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::NATIONALITY:
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::BIRTH_YEAR:
            break;
        case InternetExplorers::InterfaceFilter::ValueFilters::TEAM:
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

    std::unordered_map<QString, std::vector<std::vector<std::string>>>::iterator it = m_dataDyName.find(name);
    if (it != m_dataDyName.end()) {
        return it->second;
    }

    // No name found
    return {};
}

bool InternetExplorers::DataHandler::filterByYear(std::vector<std::string> row, QString filterValue)
{
    if (row[IndexInData::YEAR] == filterValue.toStdString()) {
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

    std::string year = row[IndexInData::YEAR];

    if (year <= upper.toStdString() && year >= lower.toStdString()) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByDistance(std::vector<std::string> row, QString filterValue)
{
    if (row[IndexInData::DISTANCE] == filterValue.toStdString()) {
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

    std::string name = row[IndexInData::NAME];
    std::transform(name.begin(), name.end(), name.begin(),
        [](unsigned char c){ return std::tolower(c); });

    // ATM needs to be exact
    if (name == filterVal) {
        return true;
    }

    return false;
}
