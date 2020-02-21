#ifndef INTERFACEFILTER_H
#define INTERFACEFILTER_H

#include <map>
#include <iostream>
#include <QString>
#include <vector>

namespace InternetExplorers
{

const std::vector<std::string> DISTANCES = {
    "P50",
    "V50",
    "P100",
    "P32",
    "V20",
    "V32",
    "V20jun",
    "P42",
    "V42",
    "P20",
    "P30",
    "P44",
    "P60",
    "P62",
    "P25",
    "P35",
    "P45",
    "P52",
    "P53",
    "P75",
    "V30",
    "V45",
    "V53",
    "V75"
};

class InterfaceFilter
{
public:
    InterfaceFilter() {}
    ~InterfaceFilter() {}

    enum Filters {
        YEAR = 0,
        YEAR_RANGE,
        DISTANCE,
        NAME,
        TIME_RANGE,
        PLACE,
        PLACE_MEN,
        PLACE_WOMEN,
        SEX,
        CITY,
        NATIONALITY,
        BIRTH_YEAR,
        TEAM
    };
    static const Filters filters;

    ///
    /// \brief
    ///     Checks that the given filters and values are valid
    /// \return
    ///     True if filter is valid.
    ///
    static bool validateFilter(std::map<Filters, QString>);

private:
    static bool validateYear(QString filterValue);
    static bool validateYearRange(QString filterValue);
    static bool validateDistance(QString filterValue);
    static bool validateName(QString filterValue);
    static bool validateTimeRange(QString filterValue) { return true; }
    static bool validatePlace(QString filterValue);
    static bool validatePlaceMen(QString filterValue);
    static bool validatePlaceWomen(QString filterValue);
    static bool validateSex(QString filterValue);
    static bool validateCity(QString filterValue) { return true; }
    static bool validateNationality(QString filterValue) { return true; }
    static bool validateBirthYear(QString filterValue);
    static bool validateTeam(QString filterValue) { return true; }
};

}

#endif // INTERFACEFILTER_H
