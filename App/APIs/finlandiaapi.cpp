#include "finlandiaapi.h"
#include "finlandiacaller.h"
#include "logger.h"
#include "constants.h"
#include <thread>

InternetExplorers::FinlandiaAPI::FinlandiaAPI():
    m_runners(0),
    m_totalCalls(0),
    m_finishedCalls(0),
    m_currentProgress(0)
{
    auto msg(QString("Luokan rakentaja on valmis"));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);
}

InternetExplorers::FinlandiaAPI::~FinlandiaAPI()
{
    auto msg(QString("Luokan tuhoaja on kutsuttu."));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);
}

std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > InternetExplorers::FinlandiaAPI::loadAllData()
{
    QString msg("Aloitetaan datan lataaminen Finlandia hiihdon sivuilta.");
    emit appendInfo(msg);
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);

    m_runners = 0;
    m_finishedCalls = 0;
    m_currentProgress = 0;
    m_totalCalls = 114+1; // Precalculated value. With +1, 100% is reached only at the end

    emit progressChanged(0);

    int optimalAmountOfThreads(std::thread::hardware_concurrency());

    if(optimalAmountOfThreads < 1)
    {
        QString msgThreads(QString("Number of concurrent threads is not well defined. Setting it to 4"));
        InternetExplorers::Logger::getInstance().log(msgThreads, Constants::Logger::Severity::WARNING, m_name);
        optimalAmountOfThreads = 4;
    }

    std::vector<std::thread> threads;

    auto searchVector = std::make_shared<std::vector<FinlandiaAPI::Parameters>>();
    searchVector->reserve(120); // Educated guess

    threads.reserve(optimalAmountOfThreads);


    for(int i = 1974; i < 2020; ++i) {
        FinlandiaAPI::Parameters search;
        search.year = QString::number(i);
        searchVector->push_back(search);
    }

    msg = QString("Ladataan data käyttäen " + QString::number(optimalAmountOfThreads) + " säiettä.");
    emit appendInfo(msg);
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);

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

    // Make sure 100% is reached
    emit progressChanged(100);

    msg = QString(QString::number(getAmountOfRows()) + " riviä dataa ladattu.");
    emit appendInfo(msg);
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, m_name);

    return m_data;
}

void InternetExplorers::FinlandiaAPI::loadInThread(std::shared_ptr<std::vector<FinlandiaAPI::Parameters>> searchVector)
{
    FinlandiaCaller caller;

    bool thisRunnerIsDone(false);

    while (m_runners > 0) {
        std::vector<std::vector<std::string> > data{caller.loadData(searchVector)};
        if(Q_LIKELY(data.size() > 0))
        {
            if(Q_UNLIKELY(thisRunnerIsDone))
            {
                m_runners++;
                thisRunnerIsDone = false;
            }
            m_finishedCalls++;
            appendData(data);
            if(int progress(static_cast<int>((100*m_finishedCalls)/m_totalCalls)); (progress != m_currentProgress))
            { // Update progress bar
                if((progress < 0) || (progress > 100))
                {
                    // More calls than expected were added. Increasing m_totalCalls somewhat compensates it
                    auto msg(QString("Etukäteen laskettu m_totalCalls saattaa olla virheellinen"));
                    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::WARNING, m_name);

                    m_currentProgress = 0;
                    m_totalCalls += 24;
                } else
                {
                    m_currentProgress = progress;
                    emit progressChanged(m_currentProgress);
                }
            }
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
}

void InternetExplorers::FinlandiaAPI::appendData(std::vector<std::vector<std::string>> data)
{
    using namespace InternetExplorers::Constants::DataIndex;

    QString year(QString::fromStdString(data.at(0).at(IndexInData::YEAR)));


    // Sort the data
    std::vector<std::vector<std::vector<std::string>>> sortedData;
    for(std::vector<std::string>& row : data)
    {
        if(int ranking(std::stoi(row.at(IndexInData::PLACE))); ranking < 1)
        {
            // Someone got better than 1st place
        } else
        {
            int correctTripIndex(0);
            bool correctTripIndexFound(false);
            for(int index(0); index < static_cast<int>(sortedData.size()); index++)
            {
                if(sortedData.at(index).at(0).at(IndexInData::DISTANCE) == row.at(IndexInData::DISTANCE))
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
                    if(oldRow.at(IndexInData::NAME) == row.at(IndexInData::NAME))
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
        for(auto trip : Constants::DISTANCES)
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
            while (static_cast<int>(newTrip.size()) < std::stoi(row.at(IndexInData::PLACE)))
            {
                // Make sure newTrip is not too small
                newTrip.push_back({});
            }
            auto& oldData = m_data[year][QString::fromStdString(trip.at(0).at(IndexInData::DISTANCE))];
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
            newTrip.at(std::stoi(row.at(IndexInData::PLACE))-1) = row;
        }
        m_data[year][QString::fromStdString(trip.at(0).at(IndexInData::DISTANCE))] = newTrip;
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

int InternetExplorers::FinlandiaAPI::getAmountOfRows() const
{
    if (m_data.size() == 0) return 0;

    int count = 0;
    for (auto& year : m_data) {
        for (auto& dist : year.second) {
            count += static_cast<int>(dist.second.size());
        }
    }

    return count;
}
