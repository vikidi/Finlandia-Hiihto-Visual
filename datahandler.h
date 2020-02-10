#ifndef DATAHANDLER_H
#define DATAHANDLER_H

// TODO: The format for times?

/*
 * This module is supposed to handle
 * most of the data handling and returning
 * it in right format for UI.
 */

#include <QObject>

#include "APIs/finlandiaapi.h"
#include "APIs/localapi.h"

class DataHandler : public QObject
{
    Q_OBJECT

public:
    DataHandler();
    ~DataHandler();

    void Initialize();

    /* PUBLIC FUNCTIONS TO ACCESS DATA */

    ///
    /// \brief
    ///     Gives all skiers that were in goal between given times
    /// \param year
    ///     Year to examine
    /// \param lowerBound
    ///     Lower bound for the time, given in format ???
    /// \param upperBound
    ///     Upper bound for the time, given in format ???
    /// \return
    ///     < Distance, < Results < Time, Skier > >
    ///
    std::map<QString, std::vector<std::pair<QString, QString>>> finishedWithinTimes(const QString year, const QString lowerBound, const QString upperBound) const;

    ///
    /// \brief
    ///     Gives results from same distance on two different years
    /// \param distance
    ///     The distance to get
    /// \param firstYear
    ///     The first year to compare
    /// \param secondYear
    ///     The second year to compare
    /// \return
    ///     < Year, < Results < Time, Skier > > >
    ///
    std::map<QString, std::vector<std::pair<QString, QString>>> compareSameDistanceDifferentYears(const QString distance, const QString firstYear, const QString secondYear) const;

    ///
    /// \brief
    ///     Gives results from same year on different distances
    /// \param year
    ///     The year te get
    /// \param firstDistance
    ///     First distance to compare
    /// \param secondDistance
    ///     Second distance to compare
    /// \return
    ///     < Distance, < Results < Time, Skier > > >
    ///
    std::map<QString, std::vector<std::pair<QString, QString>>> compareSameYearDifferentDistances(const QString year, const QString firstDistance, const QString secondDistance) const;

    ///
    /// \brief
    ///     Gives the amount of skiers for each year
    /// \return
    ///     < Year, Amount >
    ///
    std::map<QString, int> amountOfSkiers() const;

    ///
    /// \brief
    ///     Gives the best and worst time for a distance from all years
    /// \param distance
    ///     The distance to get the results from
    /// \return
    ///     < Year, < Best, Worst > >
    ///
    std::map<QString, std::pair<QString, QString>> bestAndWorstTimesForDistance(const QString distance) const;

    ///
    /// \brief
    ///     Gives average times for a distance for all years
    /// \param distance
    ///     The distance to get the results from
    /// \return
    ///     < Year, Average >
    ///
    std::map<QString, QString> averageTimesForDistance(const QString distance) const;

    ///
    /// \brief
    ///     Gives results for one skier in given time period
    /// \param name
    ///     Name of the skier
    /// \return
    ///     < Distance, < Year, Time > >
    ///
    std::map<QString, std::pair<QString, QString>> resultsForOneSkier(const QString name, const QString lowerYear, const QString upperYear) const;

    ///
    /// \brief
    ///     Gives average time for given placements between given years
    /// \param lowerYear
    ///     Lower bound for year
    /// \param upperYear
    ///     Upper bound for year
    /// \param lowerPlace
    ///     Lower placement
    /// \param upperPlace
    ///     Upper placement
    /// \return
    ///     < Distance, < Year, Average > >
    ///
    std::map<QString, std::pair<QString, QString>> averageTimeByPlacement(const QString lowerYear, const QString upperYear, const QString lowerPlace, const QString upperPlace) const;

    ///
    /// \brief
    ///     Gives the best placement for M/W between given years
    /// \param lowerYear
    ///     Lower bound for year
    /// \param upperYear
    ///     Upper bound for year
    /// \param sex
    ///     Sex, M for men, W for Women (use enum)
    /// \return
    ///     < Distance, < Year, Placement > >
    ///
    std::map<QString, std::pair<QString, int>> bestPlacementBetweenYears(const QString lowerYear, const QString upperYear, const QString sex) const;

    ///
    /// \brief
    ///     Gives results from one year ordered by team name
    /// \param year
    ///     The year to get results from
    /// \return
    ///     < Distance, Results< Row< Field > > >
    ///
    std::map<QString, std::vector<std::vector<QString>>> resultsAlpabeticalByTeam(const QString year) const;

    ///
    /// \brief
    ///     Return amount of participants by country
    /// \return
    ///     < Year, < Country, Amount > >
    ///
    std::map<QString, std::pair<QString, int>> amountOfSkiersByCountry() const;

    ///
    /// \brief
    ///     Results for ten best teams for given year and distance
    /// \param year
    ///     The year for results
    /// \param distance
    ///     The distance for results
    /// \return
    ///     < Results< < Team, Total time > > >
    ///
    std::vector<std::pair<QString, QString>> teamBestTeams(const QString year, const QString distance) const;

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
    void loadData();
    void loadInThread();

    bool m_loadOngoing;

    FinlandiaAPI *m_finlandiaAPI;
    LocalAPI *m_localAPI;

    // < Year, < Distance, Row< Column< Value > > > >
    std::map<QString, std::map<QString,
                std::vector<std::vector<std::string>>>> m_data;
};

#endif // DATAHANDLER_H
