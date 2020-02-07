#include "finlandiaapi.h"
#include "finlandiacaller.h"

#include <thread>
#include <QDebug>

FinlandiaAPI::FinlandiaAPI():
    ready(0)
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
    ready = 0;
    std::thread threads[1104]; // 46 * 24
    int j = 0;
    for(int i = 1974; i < 2020; ++i) {
        for(auto m : matkat) {
            threads[j] = std::thread(&FinlandiaAPI::loadInThread, this, QString::number(i), QString::fromStdString(m));
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
}

void FinlandiaAPI::appendData(std::vector<std::vector<std::string>> data, QString year, QString distance)
{
    std::map<QString, std::vector<std::vector<std::string>>> innerData = { {distance, data} };
    m_mtx.lock();

    if( m_data.find(year) == m_data.end() ) {
        m_data.insert( {year, innerData} );
    } else {
        m_data[year].insert( {distance, data} );
    }

    ready++;

    std::cout << ready << ": " << year.toStdString() << " " << distance.toStdString() << std::endl;
    //qDebug() << ready << ": " << year << " " << distance;

    m_mtx.unlock();
}
