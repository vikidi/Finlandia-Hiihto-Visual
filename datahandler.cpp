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

DataHandler::DataHandler():
    m_loadOngoing(false),
    m_finlandiaAPI(new FinlandiaAPI),
    m_localAPI(new LocalAPI),
    m_data({})
{
    // For progress
    connect(m_finlandiaAPI, &FinlandiaAPI::progressChanged, this, &DataHandler::progressChangedInApi);
}

DataHandler::~DataHandler()
{
    delete m_finlandiaAPI;
    delete m_localAPI;
}

void DataHandler::Initialize()
{
    if (!m_loadOngoing) {
        m_loadOngoing = true;
        loadData();
    }
}

std::vector<std::vector<std::string> > DataHandler::getDataWithFilter(std::map<InterfaceFilter::Filters, QString> filters)
{
    if (!InterfaceFilter::validateFilter(filters)) {
        // TODO What
        return {};
    }

    if (filters.size() == 0) {
        // TODO Really return everything??
        return {};
    }

    // Do the filtering
    std::vector<std::vector<std::string>> data = {};

    // Get first filter
    std::pair<InterfaceFilter::Filters, QString> firstFilter = *(filters.begin());

    // Do the first filtering
    switch(firstFilter.first) {
        case InterfaceFilter::YEAR:
            data = filterByYear(firstFilter.second);
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

void DataHandler::applyFilterToData(std::map<InterfaceFilter::Filters, QString> filters, std::vector<std::vector<std::string> > &data)
{
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

            case InterfaceFilter::YEAR:
                filterByYear(filter.second, data);
                break;

            default:
                break;
        }
    }

    return;
}

void DataHandler::progressChangedInApi(const int progress)
{
    emit progressChanged(progress);
}

void DataHandler::loadData()
{
    std::thread t(&DataHandler::loadInThread, this);
    t.detach();
}

void DataHandler::loadInThread()
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

std::vector<std::vector<std::string> > DataHandler::filterByYear(QString filterValue)
{
    std::vector<std::vector<std::string>> data = {};

    // Year not found from data
    if (m_data.find(filterValue) == m_data.end()) {
        return {};
    }

    // Go through distances
    for(auto dist : m_data[filterValue]) {
        // Add rows to end of data
        data.insert( data.end(), dist.second.begin(), dist.second.end() );
    }

    return data;
}

void DataHandler::filterByYear(QString filterValue, std::vector<std::vector<std::string> > &prevData)
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
