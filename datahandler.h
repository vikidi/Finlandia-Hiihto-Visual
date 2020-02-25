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

    bool applyAllFiltersToRow(std::map<InterfaceFilter::ValueFilters, QString> filters,
                              std::vector<std::string> row);

    std::vector<std::vector<std::string>> getAllByYear(QString year);
    std::vector<std::vector<std::string>> getAllByYearRange(QString range);
    std::vector<std::vector<std::string>> getAllByDistance(QString distance);

    ///
    /// \brief
    ///     Filters the whole data by given year
    /// \param
    ///     Year to filter by
    /// \return
    ///     Data only from given year
    ///
    bool filterByYear(std::vector<std::string> row, QString filterValue);

    bool filterByYearRange(std::vector<std::string> row, QString filterValue);
    bool filterByDistance(std::vector<std::string> row, QString filterValue);
    bool filterByName(std::vector<std::string> row, QString filterValue);
    bool filterByTimeRange(std::vector<std::string> row, QString filterValue) {}
    bool filterByPlace(std::vector<std::string> row, QString filterValue) {}
    bool filterByPlaceMen(std::vector<std::string> row, QString filterValue) {}
    bool filterByPlaceWomen(std::vector<std::string> row, QString filterValue) {}
    bool filterBySex(std::vector<std::string> row, QString filterValue) {}
    bool filterByCity(std::vector<std::string> row, QString filterValue) {}
    bool filterByNationality(std::vector<std::string> row, QString filterValue) {}
    bool filterByBirthYear(std::vector<std::string> row, QString filterValue) {}
    bool filterByTeam(std::vector<std::string> row, QString filterValue) {}

    bool m_loadOngoing;

    InternetExplorers::FinlandiaAPI *m_finlandiaAPI;
    InternetExplorers::LocalAPI *m_localAPI;

    // < Year, < Distance, Row< Column< Value > > > >
    std::map<QString, std::map<QString,
                std::vector<std::vector<std::string>>>> m_data;
};

}

#endif // DATAHANDLER_H
