#include "finlandiaapi.h"
#include "finlandiacaller.h"

#include <thread>
#include <QDebug>

FinlandiaAPI::FinlandiaAPI():
    m_ready(0),
    m_runners(0)
{

}

FinlandiaAPI::~FinlandiaAPI()
{

}

std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > FinlandiaAPI::loadAllData()
{
    std::vector<std::string> matkat = {
        "P50",
        "V50",
        "P100",
        "P32",
        "V20",
        "V32",
        "V20jun",
        "P42",
        "V42",
        "P20",
        "P30",
        "P44",
        "P60",
        "P62",
        "P25",
        "P35",
        "P45",
        "P52",
        "P53",
        "P75",
        "V30",
        "V45",
        "V53",
        "V75"
    };

    // REAL DEAL
    //unsigned long long const amount = matkat.size() * 2;
    m_ready = 0;
    m_runners = 0;

    int optimalAmountOfThreads(std::thread::hardware_concurrency());
    std::cout << optimalAmountOfThreads << " concurrent threads are supported.\n";

    if(optimalAmountOfThreads < 1)
    {
        std::cout << "Number of concurrent threads is not well defined.\n"
                     "Setting it to 4\n";
        optimalAmountOfThreads = 4;
    }

    std::vector<std::thread> threads;
    threads.reserve(matkat.size()*(2020-1974));

    int j = 0;
    for(int i = 1974; i < 2020; ++i) {
        for(auto m : matkat) {
            while (m_runners >= optimalAmountOfThreads)
            {
                // Constant polling could be changed to event-based system using std::condition_variable
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
            threads.push_back(std::thread(&FinlandiaAPI::loadInThread, this, QString::number(i), QString::fromStdString(m)));
            m_runners++;
            j++;
        }
    }

    for (auto& th : threads) {
        th.join();
    }

    // TESTING
    //std::thread th(&FinlandiaAPI::loadInThread, this, QString("2017"), QString("P50"));
    //th.join();

    return m_data;
}

void FinlandiaAPI::loadInThread(QString year, QString distance)
{
    FinlandiaCaller caller = FinlandiaCaller();

    std::vector<std::vector<std::string> > data = caller.loadAllData(year, distance);

    appendData(data, year, distance);
    m_runners--;
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
    //qDebug() << ready << ": " << year << " " << distance;

}
