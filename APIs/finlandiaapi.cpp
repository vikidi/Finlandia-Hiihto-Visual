#include "finlandiaapi.h"
#include "finlandiacaller.h"
#include "logger.h"

#include <thread>
#include <QDebug>

InternetExplorers::FinlandiaAPI::FinlandiaAPI():
    m_ready(0),
    m_runners(0),
    m_totalCalls(0),
    m_currentProgress(0)
{
    auto msg(QString("Constructor ready"));
    auto msgSender(QString("FinlandiaAPI"));
    InternetExplorers::Logger::getInstance().log(msg, InternetExplorers::Logger::Severity::INFO, msgSender);
}

InternetExplorers::FinlandiaAPI::~FinlandiaAPI()
{
    auto msg(QString("Destructor called"));
    auto msgSender(QString("FinlandiaAPI"));
    InternetExplorers::Logger::getInstance().log(msg, InternetExplorers::Logger::Severity::INFO, msgSender);
}

std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > InternetExplorers::FinlandiaAPI::loadAllData()
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

    auto searchVector = std::make_shared<std::vector<FinlandiaAPI::Parameters>>();
    searchVector->reserve(220); // Educated guess

    threads.reserve(optimalAmountOfThreads);


    for(int i = 1974; i < 2020; ++i) {
        FinlandiaAPI::Parameters search;
        search.year = QString::number(i);
        searchVector->push_back(search);
    }

    m_totalCalls = static_cast<int>(searchVector->size());

    for(int i(0); i < optimalAmountOfThreads; i++)
    {
        m_runners++;
        threads.push_back(std::thread(&FinlandiaAPI::loadInThread, this, searchVector));
    }

    for (auto& th : threads) {
        th.join();
    }
    // Loading ready

    removePlankLines();

    emit progressChanged(100);

    return m_data;
}

void InternetExplorers::FinlandiaAPI::loadInThread(std::shared_ptr<std::vector<FinlandiaAPI::Parameters>> searchVector)
{
    qDebug() << "Thread started";
    FinlandiaCaller caller;

    bool thisRunnerIsDone(false);

    while (m_runners > 0) {
        std::vector<std::vector<std::string> > data = caller.loadData(searchVector);
        if(Q_LIKELY(data.size() > 0))
        {
            if(Q_UNLIKELY(thisRunnerIsDone))
            {
                m_runners++;
                thisRunnerIsDone = false;
            }
            if(int progress(static_cast<int>(100*(m_totalCalls-searchVector->size())/m_totalCalls)); (progress != m_currentProgress))
            {
                if((progress < 0) || (progress > 100))
                {
                    // Many new calls were added. Increasing m_totalCalls somewhat compensates it
                    m_currentProgress = 0;
                    m_totalCalls += 10;
                } else
                {
                    m_currentProgress = progress;
                    emit progressChanged(m_currentProgress);
                }
            }
            appendData(data);
        } else
        {
            if(!thisRunnerIsDone && (searchVector->size() == 0))
            {
                // There's no work to do
                m_runners--;
                thisRunnerIsDone = true;
            }
        }
    }
    qDebug() << "Thread ended";
}

void InternetExplorers::FinlandiaAPI::appendData(std::vector<std::vector<std::string>> data)
{

    QString year(QString::fromStdString(data.at(0).at(0)));


    // Sort the data
    std::vector<std::vector<std::vector<std::string>>> sortedData;
    for(std::vector<std::string>& row : data)
    {
        if(int ranking(std::stoi(row.at(3))); ranking < 1)
        {
            // Someone got better than 1st place
        } else
        {
            int correctTripIndex(0);
            bool correctTripIndexFound(false);
            for(int index(0); index < static_cast<int>(sortedData.size()); index++)
            {
                if(sortedData.at(index).at(0).at(1) == row.at(1))
                {
                    correctTripIndex = index;
                    correctTripIndexFound = true;
                    break;
                }
            }
            if(correctTripIndexFound)
            {
                // Trip was already in list

                bool addNewRow(true);
                for(auto oldRow : sortedData.at(correctTripIndex))
                {
                    if(oldRow.at(7) == row.at(7))
                    {
                        // Duplicate name
                        if(oldRow == row)
                        {
                            // Duplicate
                            addNewRow = false;
                        }
                    }
                }
                if(addNewRow)
                {
                    sortedData.at(correctTripIndex).push_back(row);
                }
            } else
            {
                // This is a new trip
                std::vector<std::vector<std::string>> newRow{row};
                sortedData.push_back(newRow);
            }
        }
    }


    std::lock_guard<std::mutex> lock(m_mtx);


    if(m_data.find(year) == m_data.end())
    {
        // No data for that year
        // Create empty fields to maps

        std::map<QString, std::vector<std::vector<std::string>>> newYear;
        for(auto trip : TRIPS)
        {
            newYear.insert({QString::fromStdString(trip),{}});
        }
        m_data.insert({year, newYear});

    }

    for(auto& trip : sortedData)
    {
        std::vector<std::vector<std::string>> newTrip(trip.size());
        newTrip.resize(trip.size());
        for(auto& row : trip)
        {
            while (static_cast<int>(newTrip.size()) < std::stoi(row.at(3)))
            {
                // Make sure newTrip is not too small
                newTrip.push_back({});
            }
            auto& oldData = m_data[year][QString::fromStdString(trip.at(0).at(1))];
            for(auto& oldRow : oldData)
            {
                bool duplicatefound(false);
                for(auto& newRow : newTrip)
                {
                    if(newRow == oldRow)
                    {
                        duplicatefound = true;
                        break;
                    }
                }
                if(duplicatefound)
                {
                    oldData = {};
                }
            }
            newTrip.at(std::stoi(row.at(3))-1) = row;
        }
        m_data[year][QString::fromStdString(trip.at(0).at(1))] = newTrip;
    }

}

void InternetExplorers::FinlandiaAPI::removePlankLines()
{
    std::lock_guard<std::mutex> lock(m_mtx);

    for(auto& years : m_data)
    {
        for(auto& trips : years.second)
        {
            for(auto it = trips.second.begin(); it != trips.second.end(); it++)
            {
                if(it->size() == 0)
                {
                    trips.second.erase(it);
                    it--;
                }
            }
        }
    }
}
