#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>

#include "APIs/finlandiaapi.h"
#include "APIs/localapi.h"
#include "interfacefilter.h"
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
     * \param filters
     * Filter to be applied
     * \attention
     * The filter must be validated with InterfaceFilter before calling
     * \return
     * Data that passed the given filter. Outer vector consists of data rows
     */
    std::vector<std::vector<std::string>> getDataWithFilter(std::map<InterfaceFilter::ValueFilters, QString> filters);

    /*!
     * \brief
     * Apply filter to data. The data itself will be filtered.
     * \param filters
     * Filter to be applied
     * \param data
     * Data where the filter will be applied
     * \attention
     * The filter must be validated with InterfaceFilter before calling
     */
    void applyFilterToData(std::map<InterfaceFilter::ValueFilters, QString> filters,
                           std::vector<std::vector<std::string>>& data);

    /* /PUBLIC INTERFACE */

private slots:

    /*!
     * \brief
     * One thread finished loading, updates progress and sends signal forward
     * \param progress
     * The current progress
     */
    void progressChangedInApi(const int progress);

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
     * Data has been loaded
     */
    void loadingReady();

private:

    // Amount of columns in row
    const size_t ROW_SIZE = 12;

    /*!
     * \brief
     * Tells the index for parameters in the data
     */
    enum IndexInData {
        YEAR = 0,
        DISTANCE,
        TIME,
        PLACE,
        PLACE_MEN,
        PLACE_WOMEN,
        SEX,
        NAME,
        CITY,
        NATIONALITY,
        BIRTH_YEAR,
        TEAM
    };

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
    bool applyAllFiltersToRow(std::map<InterfaceFilter::ValueFilters, QString> filters,
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

    // < Year, < Distance, Row< Column< Value > > > >
    std::map<QString, std::map<QString,
                std::vector<std::vector<std::string>>>> m_data;

    // < Name, < Row< Column< Value > > > >
    std::unordered_map<QString, std::vector<std::vector<std::string>>, QStringKeyHash> m_dataByName;
};

}

#endif // DATAHANDLER_H
