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

class DataHandler : public QObject
{
    Q_OBJECT

public:
    DataHandler();
    ~DataHandler();

    void Initialize();

    /* PUBLIC INTERFACE */

    std::vector<std::vector<std::string>> getDataWithFilter(std::map<InterfaceFilter::Filters, QString> filters);

    void applyFilterToData(std::map<InterfaceFilter::Filters, QString> filters,
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

    bool m_loadOngoing;

    FinlandiaAPI *m_finlandiaAPI;
    LocalAPI *m_localAPI;

    // < Year, < Distance, Row< Column< Value > > > >
    std::map<QString, std::map<QString,
                std::vector<std::vector<std::string>>>> m_data;
};

#endif // DATAHANDLER_H
