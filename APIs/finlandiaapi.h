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
        QString year = "kaikki";
        QString trip = "kaikki";
        QString gender = "kaikki";
        QString age = "kaikki";
        QString firstName = "";
        QString lastName = "";
        QString city = "";
        QString nationality = "0";
        QString team = "";
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
