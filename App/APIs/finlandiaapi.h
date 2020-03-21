#ifndef FINLANDIAAPI_H
#define FINLANDIAAPI_H

#include <QObject>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <mutex>
#include <atomic>

namespace InternetExplorers
{

/*!
 * \brief
 * This class is the public interface to the finlandia web data
 */
class FinlandiaAPI : public QObject
{
    Q_OBJECT

public:
    FinlandiaAPI();
    ~FinlandiaAPI();

    /*!
     * \brief
     * Parameters for the data queries from the server
     */
    struct Parameters {
        QString year = "kaikki";    /*!< Year parameter */
        QString trip = "kaikki";    /*!< Distance/trip parameter */
        QString gender = "kaikki";  /*!< Gender/sex paramenter */
        QString age = "kaikki";     /*!< Age parameter */
        QString firstName = "";     /*!< First name of skier paramenter */
        QString lastName = "";      /*!< Last name of skier paramenter */
        QString city = "";          /*!< City of skier parameter */
        QString nationality = "0";  /*!< Nationality of skier parameter */
        QString team = "";          /*!< Team of skier parameter */
    };

    /*!
     * \brief
     * Function to load all data from the finlandia hiihto website
     * \return
     * Loaded data in format < Year, Distance, < Row > > >
     */
    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> loadAllData();


signals:

    /*!
     * \brief
     * Progress has changed in loading
     * \param progress
     * The current progress
     */
    void progressChanged(const int progress);

private slots:

private:
    void loadInThread(std::shared_ptr<std::vector<FinlandiaAPI::Parameters>>);
    void appendData(std::vector<std::vector<std::string>>);
    void removePlankLines();

    std::mutex m_mtx;
    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> m_data;

    int m_ready;
    std::atomic_int m_runners;

    std::atomic_int m_totalCalls;
    std::atomic_int m_finishedCalls;
    std::atomic_int m_currentProgress;
};

}

#endif // FINLANDIAAPI_H
