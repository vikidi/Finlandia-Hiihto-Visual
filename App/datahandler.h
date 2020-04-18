#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>

#include "APIs/finlandiaapi.h"
#include "APIs/localapi.h"
#include "interfacefilter.h"
#include "dataorderer.h"
#include <unordered_map>
#include <QString>
#include <QHash>
#include <vector>
#include <string>
#include <functional>

namespace InternetExplorers
{

/*! \struct QStringKeyHash
 *  \brief Hash function for QString
 */
struct QStringKeyHash {

    /*!
     * \brief
     * Hashes QString. Used when QString is key in map.
     * \param key
     * The Qstring to hash
     * \return
     * The hash value
     */
    std::size_t operator()(const QString& key) const
    {
        return qHash(key);
    }
};

/*!
 * \brief
 * Provides a interface to the finlandia hiihto data.
 * \details
 * Remeber to call Initialize() before using this class.<br>
 * This class gets the data to its member variables in initialization.<br>
 * After that the data can be fetched via public methods.
 */
class DataHandler : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief
     * Default constructor
     */
    DataHandler();
    ~DataHandler();

    /*!
     * \brief
     * Initializes the class by starting the loading of data.
     */
    void Initialize();

    /* PUBLIC INTERFACE */

    /*!
     * \brief
     * Get data from the whole database with filter.
     * \details
     * If a filter parameter is left unused
     * (not included in the filters map parameter)
     * that will not restrict the results
     * eg. everything considering that filter is returned.
     * \param filters
     * Filter to be applied
     * \attention
     * The filter must be validated with InterfaceFilter before calling
     * \return
     * Data that passed the given filter. Outer vector consists of data rows
     */
    std::vector<std::vector<std::string>> getDataWithFilter(std::map<Constants::Filter::ValueFilters, QString> filters);

    /*!
     * \brief
     * Gets data using the value filter
     * and return the results sorted by the given order filter
     * \param filters
     * Value filters, refers to InternetExplorers::Constants::Filters::ValueFilters
     * \param order
     * Order filter, refers to InternetExplorers::Constants::Filters::OrderFilters
     * \attention
     * The filter must be validated with InterfaceFilter before calling
     * \return
     * Ordered data that passed the filter
     */
    std::vector<std::vector<std::string>> getDataWithFilter(std::map<Constants::Filter::ValueFilters, QString> filters,
                                                            Constants::Filter::OrderFilters order);

    /*!
     * \brief
     * Apply filter to data. The data itself will be filtered.
     * \details
     * If a filter parameter is left unused
     * (not included in the filters map parameter)
     * that will not restrict the results
     * eg. everything considering that filter is returned.
     * \param filters
     * Filter to be applied
     * \param data
     * Data where the filter will be applied
     * \attention
     * The filter must be validated with InterfaceFilter before calling
     * \return
     * The data got with the filter
     */
    std::vector<std::vector<std::string>> applyFilterToData(std::map<Constants::Filter::ValueFilters, QString> filters,
                           std::vector<std::vector<std::string>>& data);

    /*!
     * \brief
     * Apply filter to data and sort the result.
     * \param filters
     * Value filter to be applied,
     * refers to InternetExplorers::Constants::Filters::ValueFilters
     * \param data
     * Data where the filters will be applied
     * \param order
     * Filter used in sorting,
     * refers to InternetExplorers::Constants::Filters::OrderFilters
     * \return
     * The data got with the filter
     */
    std::vector<std::vector<std::string>> applyFilterToData(std::map<Constants::Filter::ValueFilters, QString> filters,
                           std::vector<std::vector<std::string>>& data,
                           Constants::Filter::OrderFilters order);

    /* Special functions */

    /*!
     * \brief
     * Gets the races that has had participants
     * \return
     * Results in format < year, distance >
     */
    std::vector<std::pair<std::string, std::string>> getRacesWithParticipants();

    /*!
     * \brief
     * Gets the amount of participants per year
     * \param filters
     * Uses the filter to narrow down data to get participants from
     * \attention
     * Uses only YEAR or YEAR_RANGE and DISTANCE filters.
     * \return
     * Results in format < year, amount >
     */
    std::map<std::string, int> getAmountOfParticipants(std::map<Constants::Filter::ValueFilters, QString> filters);

    /*!
     * \brief
     * Gets the slowes result
     * \param filters
     * Uses the filter to narrow down data from where to get the slowest
     * \return
     * Results in format < year, row of the slowest >
     */
    std::map<std::string, std::vector<std::string>> getSlowest(std::map<Constants::Filter::ValueFilters, QString> filters);

    /*!
     * \brief
     * Gets the fastest result
     * \param filters
     * Uses the filter to narrow down data from where to get the fastest
     * \return
     * Results in format < year, row of the fastest >
     */
    std::map<std::string, std::vector<std::string>> getFastest(std::map<Constants::Filter::ValueFilters, QString> filters);

    /*!
     * \brief
     * Gets the average result time
     * \param filters
     * Uses the filter to narrow down data from where to get the average time
     * \return
     * Results in format < year, average time >
     */
    std::map<std::string, std::string> getAverageTimes(std::map<Constants::Filter::ValueFilters, QString> filters);

    /*!
     * \brief
     * Gets the average speed of skiers
     * \param filters
     * Uses the filter to narrow down data from where to get the average time
     * \return
     * Results in format < year, average speed (km/h) >
     */
    std::map<std::string, std::string> getAverageSpeeds(std::map<Constants::Filter::ValueFilters, QString> filters);

    /*!
     * \brief
     * Gets the amount of participants by country
     * \param filters
     * Uses the filter to narrow down data from where to get the participants
     * \return
     * Results in format < country, amount >
     */
    std::map<std::string, int > getParticipantsByCountry(std::map<Constants::Filter::ValueFilters, QString> filters);

    /*!
     * \brief
     * Gets the top ten best teams and their average result time.
     * \param filters
     * Uses the filter to narrow down data from where to get the participants
     * \attention
     * DISTANCE filter must be used!
     * \return
     * Results in format < team, average time of the four best participants from that team >
     */
    std::vector<std::pair<std::string, std::string>> getBestTenTeams(std::map<Constants::Filter::ValueFilters, QString> filters);

    /* /Special functions */

    /* /PUBLIC INTERFACE */

private slots:

    /*!
     * \brief
     * One thread finished loading, updates progress and sends signal forward
     * \param progress
     * The current progress
     */
    void progressChangedInApi(const int progress);

    /*!
     * \brief
     * Append text to main window info box
     * \param text
     * Text to be appended
     */
    void appendInfoFromApi(const QString text);

signals:
    /*!
     * \brief
     * Loading progress has been changed
     * \param progress
     * The current progress
     */
    void progressChanged(const int progress);

    /*!
     * \brief
     * Append text to main window info box
     * \param text
     * Text to be appended
     */
    void appendInfo(const QString text);

    /*!
     * \brief
     * Data has been loaded
     */
    void loadingReady();

private:

    const QString m_name = "DataHandler";

    /*!
     * \brief
     * Loads the data.
     */
    void loadData();

    /*!
     * \brief
     * Loads the data in separate thread
     */
    void loadInThread();

    /*!
     * \brief
     * Creates a map with name as key and corresponding rows as value.
     */
    void setRowsByName();

    /*!
     * \brief
     * Uses filters to one row
     * \param filters
     * Filters and values to use
     * \param row
     * One row of data
     * \return
     * True if row passes the filter
     */
    bool applyAllFiltersToRow(std::map<Constants::Filter::ValueFilters, QString> filters,
                              std::vector<std::string> row);

    /*!
     * \brief
     * Gets all rows that are from given year
     * \param year
     * The year from to get rows
     * \return
     * The rows from the year
     */
    std::vector<std::vector<std::string>> getAllByYear(QString year);

    /*!
     * \brief
     * Gets all rows that are from given year range
     * \param range
     * The year range from to get rows
     * \return
     * The rows from the year range
     */
    std::vector<std::vector<std::string>> getAllByYearRange(QString range);

    /*!
     * \brief
     * Gets all rows that are from given distance
     * \param distance
     * The distance from to get rows
     * \return
     * The rows from the distance
     */
    std::vector<std::vector<std::string>> getAllByDistance(QString distance);

    /*!
     * \brief
     * Gets all rows that have given name
     * \param name
     * The name to get rows
     * \return
     * The rows with the name
     */
    std::vector<std::vector<std::string>> getAllByName(QString name);

    /* FILTERING FUNCTIONS */

    bool filterByYear(std::vector<std::string> row, QString filterValue);
    bool filterByYearRange(std::vector<std::string> row, QString filterValue);
    bool filterByDistance(std::vector<std::string> row, QString filterValue);
    bool filterByName(std::vector<std::string> row, QString filterValue);
    bool filterByTimeRange(std::vector<std::string> row, QString filterValue);
    bool filterByPlace(std::vector<std::string> row, QString filterValue);
    bool filterByPlaceRange(std::vector<std::string> row, QString filterValue);
    bool filterByPlaceRangeMen(std::vector<std::string> row, QString filterValue);
    bool filterByPlaceRangeWomen(std::vector<std::string> row, QString filterValue);
    bool filterByPlaceMen(std::vector<std::string> row, QString filterValue);
    bool filterByPlaceWomen(std::vector<std::string> row, QString filterValue);
    bool filterBySex(std::vector<std::string> row, QString filterValue);
    bool filterByCity(std::vector<std::string> row, QString filterValue);
    bool filterByNationality(std::vector<std::string> row, QString filterValue);
    bool filterByBirthYear(std::vector<std::string> row, QString filterValue);
    bool filterByTeam(std::vector<std::string> row, QString filterValue);

    /* /FILTERING FUNCTIONS */

    bool m_loadOngoing;

    // API to the finlandia web data
    InternetExplorers::FinlandiaAPI *m_finlandiaAPI;

    // API to the local data in folders
    InternetExplorers::LocalAPI *m_localAPI;

    InternetExplorers::DataOrderer *m_orderer;

    // < Year, < Distance, Row< Column< Value > > > >
    std::map<QString, std::map<QString,
                std::vector<std::vector<std::string>>>> m_data;

    // < Name, < Row< Column< Value > > > >
    std::unordered_map<QString, std::vector<std::vector<std::string>>, QStringKeyHash> m_dataByName;
};

}

#endif // DATAHANDLER_H
