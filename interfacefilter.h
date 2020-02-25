#ifndef INTERFACEFILTER_H
#define INTERFACEFILTER_H

#include <map>
#include <iostream>
#include <QString>
#include <vector>
#include <QException>

// TODO Fix not to be global!
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

class FilterException : public std::exception
{
public:
    FilterException(const char* msg = " ") : message(msg) {}
    ~FilterException() {}
    const char *what() const noexcept { return message.c_str(); }

private:
    std::string message;
};

class InterfaceFilter
{
public:
    InterfaceFilter() {}
    ~InterfaceFilter() {}

    enum ValueFilters {
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
    static const ValueFilters valueFilters;

    enum OrderFilters {
        PLACEMENT = 0,
        AGE
    };
    static const OrderFilters OrderFilters;

    ///
    /// \brief
    ///     Checks that the given filters and values are valid
    /// \return
    ///     True if filter is valid.
    ///
    static bool validateFilter(std::map<ValueFilters, QString>);

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

#endif // INTERFACEFILTER_H
