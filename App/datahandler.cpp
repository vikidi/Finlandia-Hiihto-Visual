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
#include <limits.h>
#include "helper.h"

typedef std::function<bool(std::pair<std::string, long>, std::pair<std::string, long>)> Comparator;

namespace InternetExplorers {

static Comparator compFunctor =
[](std::pair<std::string, long> elem1 ,std::pair<std::string, long> elem2)
{
    return elem1.second < elem2.second;
};

}

InternetExplorers::DataHandler::DataHandler():
    m_loadOngoing(false),
    m_finlandiaAPI(new InternetExplorers::FinlandiaAPI),
    m_localAPI(new InternetExplorers::LocalAPI),
    m_orderer(new InternetExplorers::DataOrderer),
    m_data({}),
    m_dataByName({})
{
    // For progress
    connect(m_finlandiaAPI, &InternetExplorers::FinlandiaAPI::progressChanged,
            this, &DataHandler::progressChangedInApi);
    connect(m_localAPI, &InternetExplorers::LocalAPI::progressChanged,
            this, &DataHandler::progressChangedInApi);

    // For info box
    connect(m_finlandiaAPI, &InternetExplorers::FinlandiaAPI::appendInfo,
            this, &DataHandler::appendInfoFromApi);
    connect(m_localAPI, &InternetExplorers::LocalAPI::appendInfo,
            this, &DataHandler::appendInfoFromApi);

    auto msg(QString("Luokan rakentaja on valmis."));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);
}

InternetExplorers::DataHandler::~DataHandler()
{
    auto msg(QString("Luokan tuhoaja on kutsuttu."));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);

    delete m_finlandiaAPI;
    delete m_localAPI;
    delete m_orderer;
}

void InternetExplorers::DataHandler::Initialize()
{
    if (!m_loadOngoing) {
        QString msg("Aloitetaan datan lataaminen.");
        emit appendInfo(msg);
        InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);

        m_loadOngoing = true;
        loadData();

        msg = QString("Datan lataaminen on valmis.");
        emit appendInfo(msg);
        InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);
    }
    else {
        QString msg("Datan lataus on jo käynnissä!");
        emit appendInfo(msg);
        InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::WARNING, m_name);
    }
}

std::vector<std::vector<std::string>> InternetExplorers::DataHandler::getDataWithFilter(std::map<Constants::Filter::ValueFilters, QString> filters)
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
            data = applyFilterToData(filters, data);
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
            data = applyFilterToData(filters, data);
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
            data = applyFilterToData(filters, data);
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
            data = applyFilterToData(filters, data);
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

std::vector<std::vector<std::string>> InternetExplorers::DataHandler::getDataWithFilter(std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filters, InternetExplorers::Constants::Filter::OrderFilters order)
{
    std::vector<std::vector<std::string> > data = getDataWithFilter(filters);
    m_orderer->orderData(data, order);
    return data;
}

std::vector<std::vector<std::string>> InternetExplorers::DataHandler::applyFilterToData(std::map<Constants::Filter::ValueFilters, QString> filters, std::vector<std::vector<std::string> > &data)
{
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

    if (filters.size() == 0 || data.size() == 0) {
        return {};
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

    return resultData;
}

std::vector<std::vector<std::string>> InternetExplorers::DataHandler::applyFilterToData(std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filters, std::vector<std::vector<std::string> > &data, InternetExplorers::Constants::Filter::OrderFilters order)
{
    std::vector<std::vector<std::string> > d = applyFilterToData(filters, data);
    m_orderer->orderData(d, order);
    return d;
}

std::vector<std::pair<std::string, std::string> > InternetExplorers::DataHandler::getRacesWithParticipants()
{
    std::vector<std::pair<std::string, std::string> > results = {};

    // Loop years and distances
    for(auto& yearMap : m_data)
    {
        for(auto& tripMap : yearMap.second)
        {
            // Has participants
            if (tripMap.second.size() > 0) {
                // Add to results
                results.emplace_back(std::pair<std::string, std::string>{yearMap.first.toStdString(), tripMap.first.toStdString()});
            }
        }
    }

    return results;
}

std::map<std::string, int> InternetExplorers::DataHandler::getAmountOfParticipants(std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filters)
{
    std::map<std::string, int> results = {};

    // Year given
    if (filters.find(Constants::Filter::ValueFilters::YEAR) != filters.end()) {
        QString year = filters[Constants::Filter::ValueFilters::YEAR];

        // Year and distance
        if (filters.find(Constants::Filter::ValueFilters::DISTANCE) != filters.end()) {
            QString distance = filters[Constants::Filter::ValueFilters::DISTANCE];

            results.insert({year.toStdString(), m_data.at(year).at(distance).size()});
        }
        // Only year
        else {
            int amount = 0;
            // loop distances
            for (auto& dist : m_data.at(year)) {
                amount += dist.second.size();
            }

            results.insert({year.toStdString(), amount});
        }
    }

    // Year range given
    else if (filters.find(Constants::Filter::ValueFilters::YEAR_RANGE) != filters.end()) {

        // Should be in style firstYear;secondYear eg. 2014;2018
        QStringList years = filters[Constants::Filter::ValueFilters::YEAR_RANGE].split(";");

        QString lower = years[0];
        QString upper = years[1];

        // Go through given years
        for (int i = lower.toInt(); i <= upper.toInt(); ++i) {
            QString year = QString::number(i);

            // Year and distance
            if (filters.find(Constants::Filter::ValueFilters::DISTANCE) != filters.end()) {
                QString distance = filters[Constants::Filter::ValueFilters::DISTANCE];

                results.insert({year.toStdString(), m_data.at(year).at(distance).size()});
            }
            // Only year
            else {
                int amount = 0;
                // loop distances
                for (auto& dist : m_data.at(year)) {
                    amount += dist.second.size();
                }

                results.insert({year.toStdString(), amount});
            }
        }
    }

    // Distance given
    else if (filters.find(Constants::Filter::ValueFilters::DISTANCE) != filters.end()) {
        QString dist = filters[Constants::Filter::ValueFilters::DISTANCE];

        // Go through all years
        for (auto& _year : m_data) {
            QString year = _year.first;

            results.insert({year.toStdString(), _year.second.at(dist).size()});
        }
    }

    // All years all distances
    else {
        // Go through all years
        for (auto& _year : m_data) {
            QString year = _year.first;

            int amount = 0;
            // loop distances
            for (auto& dist : _year.second) {
                amount += dist.second.size();
            }

            results.insert({year.toStdString(), amount});
        }
    }

    return results;
}

std::map<std::string, std::vector<std::string> > InternetExplorers::DataHandler::getSlowest(std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filters)
{
    // Get rows that pass filter
    std::vector<std::vector<std::string>> data = getDataWithFilter(filters);

    if (data.size() == 0) return {};

    // Go through data to get slowests
    std::map<std::string, std::vector<std::string> > result = {};
    std::map<std::string, long> times = {};
    for (auto& row : data) {
        std::string year = row[Constants::DataIndex::IndexInData::YEAR];
        QString stime = QString::fromStdString(row[Constants::DataIndex::IndexInData::TIME]);

        long time = Helper::timeToMSecs(stime);

        if (time == -1) continue; // Invalid time

        // There is no times yet or time is solver
        if (times.find(year) == times.end() || time > times[year]) {
            times[year] = time;
            result[year] = row;
        }
    }

    return result;
}

std::map<std::string, std::vector<std::string> > InternetExplorers::DataHandler::getFastest(std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filters)
{
    // Get rows that pass filter
    std::vector<std::vector<std::string>> data = getDataWithFilter(filters);

    if (data.size() == 0) return {};

    // Go through data to get slowests
    std::map<std::string, std::vector<std::string> > result = {};
    std::map<std::string, long> times = {};
    for (auto& row : data) {
        std::string year = row[Constants::DataIndex::IndexInData::YEAR];
        QString stime = QString::fromStdString(row[Constants::DataIndex::IndexInData::TIME]);

        long time = Helper::timeToMSecs(stime);

        if (time == -1) continue; // Invalid time

        // There is no times yet or time is solver
        if (times.find(year) == times.end() || time < times[year]) {
            times[year] = time;
            result[year] = row;
        }
    }

    return result;
}

std::map<std::string, std::string> InternetExplorers::DataHandler::getAverageTimes(std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filters)
{
    // Get rows that pass filter
    std::vector<std::vector<std::string>> data = getDataWithFilter(filters);

    if (data.size() == 0) return {};

    // < year, value >
    std::map<std::string, unsigned long long> times = {}; // In milliseconds
    std::map<std::string, int> counts = {};

    for (auto& row : data) {
        std::string year = row[Constants::DataIndex::IndexInData::YEAR];

        // Add time
        QString time = QString::fromStdString(row[Constants::DataIndex::IndexInData::TIME]);

        long itime = Helper::timeToMSecs(time);

        if (itime == -1) continue; // Invalid time

        times[year] += static_cast<unsigned long long>(itime);

        // Add count
        ++counts[year];
    }

    // Calculate averages
    std::map<std::string, std::string> results = {};
    for (auto& time : times) {
        std::string st = Helper::mSecsToString(time.second/static_cast<unsigned long long>(counts[time.first]));

        results.insert({time.first, st});
    }

    return results;
}

std::map<std::string, std::string> InternetExplorers::DataHandler::getAverageSpeeds(std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filters)
{
    // Get rows that pass filter
    std::vector<std::vector<std::string>> data = getDataWithFilter(filters);

    if (data.size() == 0) return {};

    // < year, value >
    std::map<std::string, double> times = {}; // In milliseconds
    std::map<std::string, int> counts = {};

    for (auto& row : data) {
        std::string year = row[Constants::DataIndex::IndexInData::YEAR];

        // Add time
        QString time = QString::fromStdString(row[Constants::DataIndex::IndexInData::TIME]);

        long itime = Helper::timeToMSecs(time);

        if (itime == -1) continue; // Invalid time

        // Turn time to km/h
        double hours = Helper::mSecsToH(static_cast<unsigned long long>(itime));
        int dist = Helper::parseKMFromDistance(row[Constants::DataIndex::IndexInData::DISTANCE]);

        times[year] += dist/hours;

        // Add count
        ++counts[year];
    }

    // Calculate averages
    std::map<std::string, std::string> results = {};
    for (auto& time : times) {
        QString st = QString::number(time.second/static_cast<double>(counts[time.first]));

        results.insert({time.first, st.toStdString()});
    }

    return results;
}

std::map<std::string, int> InternetExplorers::DataHandler::getParticipantsByCountry(std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filters)
{
    std::map<std::string, int> results = {};

    // Year given
    if (filters.find(Constants::Filter::ValueFilters::YEAR) != filters.end()) {
        QString year = filters[Constants::Filter::ValueFilters::YEAR];

        // Year and distance
        if (filters.find(Constants::Filter::ValueFilters::DISTANCE) != filters.end()) {
            QString distance = filters[Constants::Filter::ValueFilters::DISTANCE];

            // Loop rows
            for (auto& row : m_data.at(year).at(distance)) {
                ++results[row[Constants::DataIndex::IndexInData::NATIONALITY]];
            }
        }
        // Only year
        else {
            // Loop distances
            for (auto& dist : m_data.at(year)) {
                // Loop rows
                for (auto& row : dist.second) {
                    ++results[row[Constants::DataIndex::IndexInData::NATIONALITY]];
                }
            }
        }
    }

    // Year range given
    else if (filters.find(Constants::Filter::ValueFilters::YEAR_RANGE) != filters.end()) {

        // Should be in style firstYear;secondYear eg. 2014;2018
        QStringList years = filters[Constants::Filter::ValueFilters::YEAR_RANGE].split(";");

        QString lower = years[0];
        QString upper = years[1];

        // Go through given years
        for (int i = lower.toInt(); i <= upper.toInt(); ++i) {
            QString year = QString::number(i);

            // Year and distance
            if (filters.find(Constants::Filter::ValueFilters::DISTANCE) != filters.end()) {
                QString distance = filters[Constants::Filter::ValueFilters::DISTANCE];

                // Loop rows
                for (auto& row : m_data.at(year).at(distance)) {
                    ++results[row[Constants::DataIndex::IndexInData::NATIONALITY]];
                }
            }
            // Only year
            else {
                // Loop distances
                for (auto& dist : m_data.at(year)) {
                    // Loop rows
                    for (auto& row : dist.second) {
                        ++results[row[Constants::DataIndex::IndexInData::NATIONALITY]];
                    }
                }
            }
        }
    }

    // Distance given
    else if (filters.find(Constants::Filter::ValueFilters::DISTANCE) != filters.end()) {
        QString dist = filters[Constants::Filter::ValueFilters::DISTANCE];

        // Go through all years
        for (auto& _year : m_data) {
            QString year = _year.first;

            for (auto& row : _year.second.at(dist)) {
                ++results[row[Constants::DataIndex::IndexInData::NATIONALITY]];
            }
        }
    }

    // All years all distances
    else {
        // Go through all years
        for (auto& _year : m_data) {
            QString year = _year.first;

            for (auto& dist : _year.second) {
                for (auto& row : dist.second) {
                    ++results[row[Constants::DataIndex::IndexInData::NATIONALITY]];
                }
            }
        }
    }

    return results;
}

std::vector<std::pair<std::string, std::string> > InternetExplorers::DataHandler::getBestTenTeams(std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filters)
{
    if (filters.find(Constants::Filter::ValueFilters::DISTANCE) == filters.end()) return {};

    // Get rows that pass filter
    std::vector<std::vector<std::string>> data = getDataWithFilter(filters);

    if (data.size() == 0) return {};

    // Get teams and times
    std::map<std::string, long> teams = {};
    std::map<std::string, int> teamsCount = {};
    std::map<std::string, std::vector<long>> teamTimes = {};
    for (auto& row : data) {

        // No empty teams
        if (row[Constants::DataIndex::IndexInData::TEAM] == "") continue;

        QString stime = QString::fromStdString(row[Constants::DataIndex::IndexInData::TIME]);
        std::string team = row[Constants::DataIndex::IndexInData::TEAM];

        long time = Helper::timeToMSecs(stime);

        if (time == -1) continue; // Invalid time

        // Not enough times yet
        if (teamsCount[team] < 4) {

            // Add time
            teams[team] += time;

            // Add to count
            ++teamsCount[team];

            // Add to the teams times
            teamTimes[team].emplace_back(time);

            // Sort teams times so that slowest is last
            std::sort(teamTimes[team].begin(), teamTimes[team].end());
        }

        // If time is better than teams current worst,
        // replace the worst with this
        else if (teamTimes[team].back() > time){

            // Substract the time difference from teams total time
            teams[team] -= (teamTimes[team].back() - time);

            // Replace the last time with this
            teamTimes[team].back() = time;

            // Sort teams times so that slowest is last
            std::sort(teamTimes[team].begin(), teamTimes[team].end());
        }
    }

    // Sort teams
    std::set<std::pair<std::string, long>, Comparator> sortedTeams(
                teams.begin(), teams.end(), compFunctor);

    // Take top ten
    std::vector<std::pair<std::string, std::string> > results = {};
    int i = 0;
    for (std::pair<std::string, long> team : sortedTeams) {
        if (i >= 10) break; // Top ten

        // Check that team actually has 4 times
        if (teamsCount.at(team.first) != 4) continue;

        std::string st = Helper::mSecsToString(static_cast<unsigned long long>(team.second)/4);

        results.emplace_back(std::pair<std::string, std::string>(team.first, st));
        ++i;
    }

    return results;
}

void InternetExplorers::DataHandler::progressChangedInApi(const int progress)
{
    emit progressChanged(progress);
}

void InternetExplorers::DataHandler::appendInfoFromApi(const QString text)
{
    emit appendInfo(text);
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

    QString msg("Luodaan ohjelman muistissa olevia säiliöitä.");
    emit appendInfo(msg);
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);

    setRowsByName();

    // CLOCKING
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    qDebug() << "Data fetch done in " << time_span.count() << " s";

    msg = QString("Datan lataaminen suoritettu " + QString::number(time_span.count()) + " sekunnissa.");
    emit appendInfo(msg);
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);

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
        case Constants::Filter::ValueFilters::PLACE_RANGE:
            passes = filterByPlaceRange(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::PLACE_RANGE_MEN:
            passes = filterByPlaceRangeMen(row, filter.second);
            break;
        case Constants::Filter::ValueFilters::PLACE_RANGE_WOMEN:
            passes = filterByPlaceRangeWomen(row, filter.second);
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
    for (int i = lower.toInt(); i <= upper.toInt(); ++i) {

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

bool InternetExplorers::DataHandler::filterByPlaceRange(std::vector<std::string> row, QString filterValue)
{
    int place = std::stoi(row[Constants::DataIndex::IndexInData::PLACE]);

    QStringList places = filterValue.split(";");

    int lower = places[0].toInt();
    int upper = places[1].toInt();

    if (place >= lower && place <= upper) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByPlaceRangeMen(std::vector<std::string> row, QString filterValue)
{
    // Check that string is not empty
    if (row[Constants::DataIndex::IndexInData::PLACE_MEN].find_first_not_of(' ') == std::string::npos) return false;

    int place = std::stoi(row[Constants::DataIndex::IndexInData::PLACE_MEN]);

    QStringList places = filterValue.split(";");

    int lower = places[0].toInt();
    int upper = places[1].toInt();

    if (place >= lower && place <= upper) {
        return true;
    }

    return false;
}

bool InternetExplorers::DataHandler::filterByPlaceRangeWomen(std::vector<std::string> row, QString filterValue)
{
    // Check that string is not empty
    if (row[Constants::DataIndex::IndexInData::PLACE_WOMEN].find_first_not_of(' ') == std::string::npos) return false;

    int place = std::stoi(row[Constants::DataIndex::IndexInData::PLACE_WOMEN]);

    QStringList places = filterValue.split(";");

    int lower = places[0].toInt();
    int upper = places[1].toInt();

    if (place >= lower && place <= upper) {
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
