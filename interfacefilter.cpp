#include "interfacefilter.h"

#include <QStringList>
#include <algorithm>
#include <cctype>

bool InterfaceFilter::validateFilter(std::map<InterfaceFilter::Filters, QString> filters)
{
    bool isOK = true;

    // Can't have year and year range filters both
    if (filters.find(InterfaceFilter::YEAR) != filters.end()) {
        if (filters.find(InterfaceFilter::YEAR_RANGE) != filters.end()) {
            return false;
        }
    }

    for(auto& filter : filters) {
        switch(filter.first) {

            case InterfaceFilter::YEAR:
                isOK = validateYear(filter.second);
                break;

            case InterfaceFilter::YEAR_RANGE:
                isOK = validateYearRange(filter.second);
                break;

            case InterfaceFilter::DISTANCE:
                isOK = validateDistance(filter.second);
                break;

            case InterfaceFilter::SEX:
                isOK = validateSex(filter.second);
                break;

            case InterfaceFilter::CITY:
                isOK = validateCity(filter.second);
                break;

            case InterfaceFilter::NAME:
                isOK = validateName(filter.second);
                break;

            case InterfaceFilter::TEAM:
                isOK = validateTeam(filter.second);
                break;

            case InterfaceFilter::TIME_RANGE:
                isOK = validateTimeRange(filter.second);
                break;

            case InterfaceFilter::PLACE:
                isOK = validatePlace(filter.second);
                break;

            case InterfaceFilter::PLACE_MEN:
                isOK = validatePlaceMen(filter.second);
                break;

            case InterfaceFilter::PLACE_WOMEN:
                isOK = validatePlaceWomen(filter.second);
                break;

            case InterfaceFilter::BIRTH_YEAR:
                isOK = validateBirthYear(filter.second);
                break;

            case InterfaceFilter::NATIONALITY:
                isOK = validateNationality(filter.second);
                break;
        }

        // One filter is not okay
        if (!isOK) {
            return false;
        }
    }

    return true;
}

bool InterfaceFilter::validateYear(QString filterValue)
{
    if (filterValue == "") {
        return false;
    }

    if (filterValue.toInt() < 1974 || filterValue.toInt() > 2019) {
        return false;
    }

    return true;
}

bool InterfaceFilter::validateYearRange(QString filterValue)
{
    if (filterValue == "") {
        return false;
    }

    // Should be in style firstYear;secondYear eg. 2014;2018
    QStringList years = filterValue.split(";");

    if (years.length() != 2) {
        return false;
    }

    int lower = years[0].toInt();
    int upper = years[1].toInt();

    if (   lower < 1974
        || lower > 2019
        || upper < 1974
        || upper > 2019) {
        return false;
    }

    // TODO Which is better > or >=?
    if (lower >= upper) {
        return false;
    }

    return true;
}

bool InterfaceFilter::validateDistance(QString filterValue)
{
    if (filterValue == "") {
        return false;
    }

    if (std::find(DISTANCES.begin(), DISTANCES.end(), filterValue.toStdString()) == DISTANCES.end()) {
        return false;
    }

    return true;
}

bool InterfaceFilter::validateName(QString filterValue)
{
    if (filterValue == "") {
        return false;
    }

    // Filter value to lower case
    std::string filterVal = filterValue.toStdString();
    std::transform(filterVal.begin(), filterVal.end(), filterVal.begin(),
        [](unsigned char c){ return std::tolower(c); });

    QString alphabets("abcdefghijklmnopqrstuvwxyzåäö ");
    for(auto& letter : filterVal) {
        if(!alphabets.contains(letter)) {
            return false;
        }
    }

    return true;
}

bool InterfaceFilter::validatePlace(QString filterValue)
{
    if (filterValue == "") {
        return false;
    }

    int place = filterValue.toInt();
    if (place < 1) {
        return false;
    }

    return true;
}

bool InterfaceFilter::validatePlaceMen(QString filterValue)
{
    return validatePlace(filterValue);
}

bool InterfaceFilter::validatePlaceWomen(QString filterValue)
{
    return validatePlace(filterValue);
}

bool InterfaceFilter::validateSex(QString filterValue)
{
    if (filterValue == "") {
        return false;
    }

    if (filterValue != "M" && filterValue != "N") {
        return false;
    }

    return true;
}

bool InterfaceFilter::validateBirthYear(QString filterValue)
{
    if (filterValue == "") {
        return false;
    }

    // Should be 2 numbers
    if (filterValue.length() != 2) {
        return false;
    }

    return true;
}


