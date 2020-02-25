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
    m_data({})
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
    if (!InterfaceFilter::validateFilter(filters)) {
        // TODO What
        qDebug("filter failed \n");
        return {};
    }

    if (filters.size() == 0) {
        // TODO Really return everything??
        return {};
    }

    // Do the filtering
    std::vector<std::vector<std::string>> data = {};

    // Get first filter
    std::pair<InterfaceFilter::ValueFilters, QString> firstFilter = *(filters.begin());

    // Do the first filtering
    switch(firstFilter.first) {

        case InterfaceFilter::ValueFilters::YEAR:
            data = filterByYear(firstFilter.second);
            break;

        case InterfaceFilter::ValueFilters::YEAR_RANGE:
            data = filterByYearRange(firstFilter.second);
            break;

        case InterfaceFilter::ValueFilters::DISTANCE:
            data = filterByDistance(firstFilter.second);
            break;

        case InterfaceFilter::ValueFilters::NAME:
            qDebug() << firstFilter.second;
            data = filterByName(firstFilter.second);
            break;

        default:
            break;
    }

    // Remove the filter, first filtering done
    filters.erase(firstFilter.first);

    // Apply rest of the filters
    applyFilterToData(filters, data);

    return data;
}

void InternetExplorers::DataHandler::applyFilterToData(std::map<InterfaceFilter::ValueFilters, QString> filters, std::vector<std::vector<std::string> > &data)
{
    // Check filter validity
    if (!InterfaceFilter::validateFilter(filters)) {
        // TODO What
        return;
    }

    if (filters.size() == 0 || data.size() == 0) {
        return;
    }

    // Go through each filter
    for(auto& filter : filters) {
        switch(filter.first) {

            case InterfaceFilter::ValueFilters::YEAR:
                filterByYear(filter.second, data);
                break;

            case InterfaceFilter::ValueFilters::YEAR_RANGE:
                filterByYearRange(filter.second, data);
                break;

            case InterfaceFilter::ValueFilters::DISTANCE:
                filterByDistance(filter.second, data);
                break;

            case InterfaceFilter::ValueFilters::NAME:
                filterByName(filter.second, data);
                break;

            default:
                break;
        }
    }

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

    // CLOCKING
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Data fetch done in " << time_span.count() << " s" << std::endl;

    m_loadOngoing = false;
    emit loadingReady();
}

std::vector<std::vector<std::string> > InternetExplorers::DataHandler::filterByYear(QString filterValue)
{
    std::vector<std::vector<std::string>> data = {};

    // Year not found from data
    if (m_data.find(filterValue) == m_data.end()) {
        return {};
    }

    // Go through distances
    for(auto& dist : m_data[filterValue]) {
        // Add rows to end of data
        data.insert( data.end(), dist.second.begin(), dist.second.end() );
    }

    return data;
}

void InternetExplorers::DataHandler::filterByYear(QString filterValue, std::vector<std::vector<std::string> > &prevData)
{
    if (filterValue == "" || prevData.size() == 0) {
        return;
    }

    // Go through all rows and add rows that pass the filter to the
    // temporary data structure
    std::vector<std::vector<std::string> > data = {};
    for(auto& row : prevData) {

        // Wrong amount of columns, remove row
        if (row.size() != ROW_SIZE) {
            continue;
        }

        if (row[IndexInData::YEAR] == filterValue.toStdString()) {
            data.emplace_back(row);
        }
    }

    // Return the filtered data
    prevData = data;
    return;
}

std::vector<std::vector<std::string> > InternetExplorers::DataHandler::filterByYearRange(QString filterValue)
{
    std::vector<std::vector<std::string>> data = {};

    if (filterValue == "") {
        return {};
    }

    // Should be in style firstYear;secondYear eg. 2014;2018
    QStringList years = filterValue.split(";");

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

void InternetExplorers::DataHandler::filterByYearRange(QString filterValue, std::vector<std::vector<std::string> > &prevData)
{
    if (filterValue == "" || prevData.size() == 0) {
        return;
    }

    // Should be in style firstYear;secondYear eg. 2014;2018
    QStringList years = filterValue.split(";");

    int lower = years[0].toInt();
    int upper = years[1].toInt();

    // Go through all rows and add rows that pass the filter to the
    // temporary data structure
    std::vector<std::vector<std::string> > data = {};

    for(auto& row : prevData) {

        // Wrong amount of columns, remove row
        if (row.size() != ROW_SIZE) {
            continue;
        }

        // If year is between given boundaries, add to vector
        if (   std::stoi(row[IndexInData::YEAR]) >= lower
            && std::stoi(row[IndexInData::YEAR]) <= upper) {

            data.emplace_back(row);
        }
    }

    // Return the filtered data
    prevData = data;
    return;
}

std::vector<std::vector<std::string> > InternetExplorers::DataHandler::filterByDistance(QString filterValue)
{
    std::vector<std::vector<std::string>> data = {};

    if (filterValue == "") {
        return {};
    }

    for (auto& year : m_data) {

        // Distance not found from years data
        if (year.second.find(filterValue) == year.second.end()) {
            continue;
        }

        // Get rows
        std::vector<std::vector<std::string>> rows = year.second[filterValue];

        // Add rows to end of data
        data.insert( data.end(), rows.begin(), rows.end() );
    }

    return data;
}

void InternetExplorers::DataHandler::filterByDistance(QString filterValue, std::vector<std::vector<std::string> > &prevData)
{
    if (filterValue == "" || prevData.size() == 0) {
        return;
    }

    // Go through all rows and add rows that pass the filter to the
    // temporary data structure
    std::vector<std::vector<std::string> > data = {};
    for(auto& row : prevData) {

        // Wrong amount of columns, remove row
        if (row.size() != ROW_SIZE) {
            continue;
        }

        if (row[IndexInData::DISTANCE] == filterValue.toStdString()) {
            data.emplace_back(row);
        }
    }

    // Return the filtered data
    prevData = data;
    return;
}

std::vector<std::vector<std::string> > InternetExplorers::DataHandler::filterByName(QString filterValue)
{
    std::vector<std::vector<std::string>> data = {};

    if (filterValue == "") {
        return {};
    }

    // Filter value to lower case
    std::string filterVal = filterValue.toStdString();
    std::transform(filterVal.begin(), filterVal.end(), filterVal.begin(),
        [](unsigned char c){ return std::tolower(c); });

    // Go through years
    for (auto& year : m_data) {

        // go through distances
        for (auto& distance : year.second) {

            // Go through rows
            for (auto& row : distance.second) {

                // Wrong amount of columns, remove row
                if (row.size() != ROW_SIZE) {
                    continue;
                }

                // Name to lower case
                std::string name = row[IndexInData::NAME];
                std::transform(name.begin(), name.end(), name.begin(),
                    [](unsigned char c){ return std::tolower(c); });

                // If name matches, add it to data
                if (name == filterVal) {
                    data.emplace_back(row);
                }
            }
        }
    }

    return data;
}

void InternetExplorers::DataHandler::filterByName(QString filterValue, std::vector<std::vector<std::string> > &prevData)
{
    if (filterValue == "" || prevData.size() == 0) {
        return;
    }

    // Filter value to lower case
    std::string filterVal = filterValue.toStdString();
    std::transform(filterVal.begin(), filterVal.end(), filterVal.begin(),
        [](unsigned char c){ return std::tolower(c); });

    // Go through all rows and add rows that pass the filter to the
    // temporary data structure
    std::vector<std::vector<std::string> > data = {};
    for(auto& row : prevData) {

        // Wrong amount of columns, remove row
        if (row.size() != ROW_SIZE) {
            continue;
        }

        // Name to lower case
        std::string name = row[IndexInData::NAME];
        std::transform(name.begin(), name.end(), name.begin(),
            [](unsigned char c){ return std::tolower(c); });

        // If name matches, add it to data
        if (name == filterVal) {
            data.emplace_back(row);
        }
    }

    // Return the filtered data
    prevData = data;
    return;
}
