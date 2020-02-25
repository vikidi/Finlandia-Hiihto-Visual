#ifndef DATAHANDLER_H
#define DATAHANDLER_H

/*
 * This module is supposed to handle
 * most of the data handling and returning
 * it in right format for UI.
 */

#include <QObject>

#include "APIs/finlandiaapi.h"
#include "APIs/localapi.h"
#include "interfacefilter.h"

namespace InternetExplorers
{

class DataHandler : public QObject
{
    Q_OBJECT

public:
    DataHandler();
    ~DataHandler();

    void Initialize();

    /* PUBLIC INTERFACE */

    std::vector<std::vector<std::string>> getDataWithFilter(std::map<InterfaceFilter::ValueFilters, QString> filters);

    void applyFilterToData(std::map<InterfaceFilter::ValueFilters, QString> filters,
                           std::vector<std::vector<std::string>>& data);

    /* /PUBLIC INTERFACE */

private slots:

    ///
    /// \brief
    ///     One thread finished loading, updates progress and sends signal forward
    ///
    void progressChangedInApi(const int);

signals:
    void progressChanged(const int);
    void loadingReady();

private:

    // Amount of columns in row
    const size_t ROW_SIZE = 12;
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

    void loadData();
    void loadInThread();

    ///
    /// \brief
    ///     Filters the whole data by given year
    /// \param
    ///     Year to filter by
    /// \return
    ///     Data only from given year
    ///
    std::vector<std::vector<std::string>> filterByYear(QString filterValue);

    ///
    /// \brief
    ///     Filters the given data by the given year
    /// \param
    ///     Year to filter by
    /// \param
    ///     Data to filter
    /// \return
    ///     Filtered data
    ///
    void filterByYear(QString filterValue, std::vector<std::vector<std::string>>& prevData);

    std::vector<std::vector<std::string>> filterByYearRange(QString filterValue);
    void filterByYearRange(QString filterValue, std::vector<std::vector<std::string>>& prevData);

    std::vector<std::vector<std::string>> filterByDistance(QString filterValue);
    void filterByDistance(QString filterValue, std::vector<std::vector<std::string>>& prevData);

    std::vector<std::vector<std::string>> filterByName(QString filterValue);
    void filterByName(QString filterValue, std::vector<std::vector<std::string>>& prevData);

    std::vector<std::vector<std::string>> filterByTimeRange(QString filterValue) {}
    void filterByTimeRange(QString filterValue, std::vector<std::vector<std::string>>& prevData) {}

    std::vector<std::vector<std::string>> filterByPlace(QString filterValue) {}
    void filterByPlace(QString filterValue, std::vector<std::vector<std::string>>& prevData) {}

    std::vector<std::vector<std::string>> filterByPlaceMen(QString filterValue) {}
    void filterByPlaceMen(QString filterValue, std::vector<std::vector<std::string>>& prevData) {}

    std::vector<std::vector<std::string>> filterByPlaceWomen(QString filterValue) {}
    void filterByPlaceWomen(QString filterValue, std::vector<std::vector<std::string>>& prevData) {}

    std::vector<std::vector<std::string>> filterBySex(QString filterValue) {}
    void filterBySex(QString filterValue, std::vector<std::vector<std::string>>& prevData) {}

    std::vector<std::vector<std::string>> filterByCity(QString filterValue) {}
    void filterByCity(QString filterValue, std::vector<std::vector<std::string>>& prevData) {}

    std::vector<std::vector<std::string>> filterByNationality(QString filterValue) {}
    void filterByNationality(QString filterValue, std::vector<std::vector<std::string>>& prevData) {}

    std::vector<std::vector<std::string>> filterByBirthYear(QString filterValue) {}
    void filterByBirthYear(QString filterValue, std::vector<std::vector<std::string>>& prevData) {}

    std::vector<std::vector<std::string>> filterByTeam(QString filterValue) {}
    void filterByTeam(QString filterValue, std::vector<std::vector<std::string>>& prevData) {}

    bool m_loadOngoing;

    InternetExplorers::FinlandiaAPI *m_finlandiaAPI;
    InternetExplorers::LocalAPI *m_localAPI;

    // < Year, < Distance, Row< Column< Value > > > >
    std::map<QString, std::map<QString,
                std::vector<std::vector<std::string>>>> m_data;
};

}

#endif // DATAHANDLER_H
