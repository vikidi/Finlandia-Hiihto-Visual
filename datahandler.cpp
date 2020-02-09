#include "datahandler.h"

#include "APIs/finlandiaapi.h"
#include "APIs/localapi.h"

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

DataHandler::DataHandler():
    m_loadOngoing(false),
    m_data({})
{

}

DataHandler::~DataHandler()
{

}

void DataHandler::Initialize()
{
    if (!m_loadOngoing) {
        m_loadOngoing = true;
        loadData();
    }
}

std::map<QString, std::vector<std::pair<QString, QString> > > DataHandler::finishedWithinTimes(const QString year, const QString lowerBound, const QString upperBound) const
{
    Q_UNUSED(year);
    Q_UNUSED(lowerBound);
    Q_UNUSED(upperBound);

    return {};
}

std::map<QString, std::vector<std::pair<QString, QString> > > DataHandler::compareSameDistanceDifferentYears(const QString distance, const QString firstYear, const QString secondYear) const
{
    Q_UNUSED(distance);
    Q_UNUSED(firstYear);
    Q_UNUSED(secondYear);

    return {};
}

std::map<QString, std::vector<std::pair<QString, QString> > > DataHandler::compareSameYearDifferentDistances(const QString year, const QString firstDistance, const QString secondDistance) const
{
    Q_UNUSED(year);
    Q_UNUSED(firstDistance);
    Q_UNUSED(secondDistance);

    return {};
}

std::map<QString, int> DataHandler::amountOfSkiers() const
{
    return {};
}

std::map<QString, std::pair<QString, QString> > DataHandler::bestAndWorstTimesForDistance(const QString distance) const
{
    Q_UNUSED(distance);

    return {};
}

std::map<QString, QString> DataHandler::averageTimesForDistance(const QString distance) const
{
    Q_UNUSED(distance);

    return {};
}

std::map<QString, std::pair<QString, QString> > DataHandler::resultsForOneSkier(const QString name, const QString lowerYear, const QString upperYear) const
{
    Q_UNUSED(name);
    Q_UNUSED(lowerYear);
    Q_UNUSED(upperYear);

    return {};
}

std::map<QString, std::pair<QString, QString> > DataHandler::averageTimeByPlacement(const QString lowerYear, const QString upperYear, const QString lowerPlace, const QString upperPlace) const
{
    Q_UNUSED(lowerYear);
    Q_UNUSED(upperYear);
    Q_UNUSED(lowerPlace);
    Q_UNUSED(upperPlace);

    return {};
}

std::map<QString, std::pair<QString, int> > DataHandler::bestPlacementBetweenYears(const QString lowerYear, const QString upperYear, const QString sex) const
{
    Q_UNUSED(sex);
    Q_UNUSED(lowerYear);
    Q_UNUSED(upperYear);

    return {};
}

std::map<QString, std::vector<std::vector<QString> > > DataHandler::resultsAlpabeticalByTeam(const QString year) const
{
    Q_UNUSED(year);

    return {};
}

std::map<QString, std::pair<QString, int> > DataHandler::amountOfSkiersByCountry() const
{
    return {};
}

std::vector<std::pair<QString, QString> > DataHandler::teamBestTeams(const QString year, const QString distance) const
{
    Q_UNUSED(year);
    Q_UNUSED(distance);

    return {};
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

    FinlandiaAPI api = FinlandiaAPI();
    m_data = api.loadAllData();

    // CLOCKING
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Data fetch done in " << time_span.count() << " s" << std::endl;

    LocalAPI localAPI = LocalAPI();
    localAPI.saveData(m_data);

    m_data = localAPI.loadData();

    // CLOCKING
    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    duration<double> time_span2 = duration_cast<duration<double>>(t3 - t2);
    std::cout << "Data save done in " << time_span2.count() << " s" << std::endl;

    m_loadOngoing = false;
}
