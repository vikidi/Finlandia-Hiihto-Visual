#include "finlandiaapi.h"
#include "finlandiacaller.h"

#include <thread>
#include <QDebug>

FinlandiaAPI::FinlandiaAPI():
    m_ready(0),
    m_runners(0),
    m_totalCalls(0),
    m_currentProgress(0)
{

}

FinlandiaAPI::~FinlandiaAPI()
{

}

std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > FinlandiaAPI::loadAllData()
{

    m_ready = 0;
    m_runners = 0;
    m_currentProgress = 0;

    emit progressChanged(0);

    int optimalAmountOfThreads(std::thread::hardware_concurrency());
    std::cout << optimalAmountOfThreads << " concurrent threads are supported.\n";

    if(optimalAmountOfThreads < 1)
    {
        std::cout << "Number of concurrent threads is not well defined.\n"
                     "Setting it to 4\n";
        optimalAmountOfThreads = 4;
    }

    std::vector<std::thread> threads;
    m_totalCalls = static_cast<int>(TRIPS.size()*(2020-1974));

    auto searchVector = std::make_shared<std::vector<FinlandiaAPI::Parameters>>();
    searchVector->reserve(2020-1974);

    threads.reserve(optimalAmountOfThreads);


    for(int i = 1974; i < 1985; ++i) {
        FinlandiaAPI::Parameters search;
        search.year = QString::number(i);
        searchVector->push_back(search);
    }

    for(int i(0); i < optimalAmountOfThreads; i++)
    {
        m_runners++;
        threads.push_back(std::thread(&FinlandiaAPI::loadInThread, this, searchVector));
    }


    for (auto& th : threads) {
        th.join();
    }

    // Loading ready
    emit progressChanged(100);

    return m_data;
}

void FinlandiaAPI::loadInThread(std::shared_ptr<std::vector<FinlandiaAPI::Parameters>> searchVector)
{
    qDebug() << "Thread started";
    FinlandiaCaller caller;

    bool thisRunnerIsReady(false);

    while (m_runners > 0) {
        std::vector<std::vector<std::string> > data = caller.loadData(searchVector);
        if(Q_LIKELY(data.size() > 0))
        {
            if(Q_UNLIKELY(thisRunnerIsReady))
            {
                m_runners++;
                thisRunnerIsReady = false;
            }
            // NOTE: Appending data does not work
            //appendData(data, QString::fromStdString(data.at(0).at(0)),QString::fromStdString(data.at(0).at(1)));
        } else
        {
            if(!thisRunnerIsReady)
            {
                m_runners--;
                thisRunnerIsReady = true;
            }
        }
    }
    qDebug() << "Thread ended";
}

void FinlandiaAPI::appendData(std::vector<std::vector<std::string>> data, QString year, QString distance)
{
    std::map<QString, std::vector<std::vector<std::string>>> innerData = { {distance, data} };
    std::lock_guard<std::mutex> lock(m_mtx);

    if( m_data.find(year) == m_data.end() ) {
        m_data.insert( {year, innerData} );
    } else {
        m_data[year].insert( {distance, data} );
    }

    m_ready++;

    std::cout << m_ready << ": " << year.toStdString() << " " << distance.toStdString() << std::endl;
    //qDebug() << m_ready << ": " << year << " " << distance;

    // Update progress
    if (static_cast<int>(((m_ready / static_cast<double>(m_totalCalls)) * 100)) > m_currentProgress) {
        emit progressChanged(static_cast<int>((m_ready / static_cast<double>(m_totalCalls)) * 100));
        m_currentProgress = static_cast<int>((m_ready / static_cast<double>(m_totalCalls)) * 100);
    }
}
