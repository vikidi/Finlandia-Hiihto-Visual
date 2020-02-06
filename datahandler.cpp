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

DataHandler::DataHandler()
{
    m_data = {};
    m_loadOngoing = false;
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
