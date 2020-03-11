#include "interfacefilter.h"

#include <QStringList>
#include <algorithm>
#include <cctype>
#include <QTime>


bool InternetExplorers::InterfaceFilter::validateFilter(std::map<InterfaceFilter::ValueFilters, QString> filters)
{
    bool isOK = true;

    // Can't have year and year range filters both
    if (filters.find(InterfaceFilter::ValueFilters::YEAR) != filters.end()) {
        if (filters.find(InterfaceFilter::ValueFilters::YEAR_RANGE) != filters.end()) {
            throw FilterException("Can not use Year and Year range same time");
        }
    }

    for(auto& filter : filters) {
        switch(filter.first) {

            case InterfaceFilter::ValueFilters::YEAR:
                isOK = validateYear(filter.second);
                break;

            case InterfaceFilter::ValueFilters::YEAR_RANGE:
                isOK = validateYearRange(filter.second);
                break;

            case InterfaceFilter::ValueFilters::DISTANCE:
                isOK = validateDistance(filter.second);
                break;

            case InterfaceFilter::ValueFilters::SEX:
                isOK = validateSex(filter.second);
                break;

            case InterfaceFilter::ValueFilters::CITY:
                isOK = validateCity(filter.second);
                break;

            case InterfaceFilter::ValueFilters::NAME:
                isOK = validateName(filter.second);
                break;

            case InterfaceFilter::ValueFilters::TEAM:
                isOK = validateTeam(filter.second);
                break;

            case InterfaceFilter::ValueFilters::TIME_RANGE:
                isOK = validateTimeRange(filter.second);
                break;

            case InterfaceFilter::ValueFilters::PLACE:
                isOK = validatePlace(filter.second);
                break;

            case InterfaceFilter::ValueFilters::PLACE_MEN:
                isOK = validatePlaceMen(filter.second);
                break;

            case InterfaceFilter::ValueFilters::PLACE_WOMEN:
                isOK = validatePlaceWomen(filter.second);
                break;

            case InterfaceFilter::ValueFilters::BIRTH_YEAR:
                isOK = validateBirthYear(filter.second);
                break;

            case InterfaceFilter::ValueFilters::NATIONALITY:
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

bool InternetExplorers::InterfaceFilter::validateYear(QString filterValue)
{
    if (filterValue == "") {
        throw FilterException("Year value empty", "YEAR", filterValue.toStdString().c_str());
    }

    if (filterValue.toInt() < 1974 || filterValue.toInt() > 2019) {
        throw FilterException("Year value not between 1974-2019", "YEAR", filterValue.toStdString().c_str());
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validateYearRange(QString filterValue)
{
    if (filterValue == "") {
        throw FilterException("Year range value empty", "YEAR RANGE", filterValue.toStdString().c_str());
    }

    if (!filterValue.contains(';')) {
        throw FilterException("Year range value does not have separator ';'", "YEAR RANGE", filterValue.toStdString().c_str());
    }

    // Should be in style firstYear;secondYear eg. 2014;2018
    QStringList years = filterValue.split(";");

    if (years.length() != 2) {
        throw FilterException("Year range value does not have two years", "YEAR RANGE", filterValue.toStdString().c_str());
    }

    int lower = years[0].toInt();
    int upper = years[1].toInt();

    if (   lower < 1974
        || lower > 2019
        || upper < 1974
        || upper > 2019) {
        throw FilterException("Year range value is not between range 1974-2019", "YEAR RANGE", filterValue.toStdString().c_str());
    }

    if (lower > upper) {
        throw FilterException("Year range values lower bound is bigger than upper", "YEAR RANGE", filterValue.toStdString().c_str());
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validateDistance(QString filterValue)
{
    if (filterValue == "") {
        throw FilterException("Distance value is empty", "DISTANCE", filterValue.toStdString().c_str());
    }

    if (std::find(DISTANCES.begin(), DISTANCES.end(), filterValue.toStdString()) == DISTANCES.end()) {
        throw FilterException("Distance value is not valid", "DISTANCE", filterValue.toStdString().c_str());
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validateName(QString filterValue)
{
    if (filterValue == "") {
        throw FilterException("Name value is empty", "NAME", filterValue.toStdString().c_str());
    }

    // Filter value to lower case
    std::string filterVal = filterValue.toStdString();
    std::transform(filterVal.begin(), filterVal.end(), filterVal.begin(),
        [](unsigned char c){ return std::tolower(c); });

    QString alphabets("?+-_\\/`´'*.:,;€^¨~|=})]([{&%¤$#£\"@!½§<>");
    for(auto& letter : filterVal) {
        if(alphabets.contains(letter)) {
            throw FilterException("Name contains unallowed characters", "NAME", filterValue.toStdString().c_str());
        }
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validateTimeRange(QString filterValue)
{
    // Needs to be in format of lowerBound;upperBound
    // eg. 2:30:00;3:30:00
    // Time is in format hh:mm:ss

    if (filterValue == "") {
        throw FilterException("Time range value empty", "TIME RANGE", filterValue.toStdString().c_str());
    }

    if (!filterValue.contains(';')) {
        throw FilterException("Time range value does not have separator ';'", "TIME RANGE", filterValue.toStdString().c_str());
    }

    QStringList times = filterValue.split(";");

    if (times.length() != 2) {
        throw FilterException("Time range value does not have two years", "TIME RANGE", filterValue.toStdString().c_str());
    }

    QTime lower = QTime::fromString(times[0], "h:mm:ss");
    QTime upper = QTime::fromString(times[1], "h:mm:ss");

    if (!lower.isValid()) {
        throw FilterException("Lower time is invalid", "TIME RANGE", filterValue.toStdString().c_str());
    }

    if (!upper.isValid()) {
        throw FilterException("Upper time is invalid", "TIME RANGE", filterValue.toStdString().c_str());
    }

    if (lower > upper) {
        throw FilterException("Lower time can not be mor than upper in time range", "TIME RANGE", filterValue.toStdString().c_str());
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validatePlace(QString filterValue)
{
    if (filterValue == "") {
        throw FilterException("Placing value is empty", "PLACE", filterValue.toStdString().c_str());
    }

    int place = filterValue.toInt();
    if (place < 1) {
        throw FilterException("Placing value is smaller than 1", "PLACE", filterValue.toStdString().c_str());
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validatePlaceMen(QString filterValue)
{
    // Try-Catch to change the possible error
    bool success = true;
    try {
        success = validatePlace(filterValue);
    } catch (FilterException& e) {
        throw FilterException(e.what(), "PLACE MEN", filterValue.toStdString().c_str());
    }
    return success;
}

bool InternetExplorers::InterfaceFilter::validatePlaceWomen(QString filterValue)
{
    // Try-Catch to change the possible error
    bool success = true;
    try {
        success = validatePlace(filterValue);
    } catch (FilterException& e) {
        throw FilterException(e.what(), "PLACE WOMEN", filterValue.toStdString().c_str());
    }
    return success;
}

bool InternetExplorers::InterfaceFilter::validateSex(QString filterValue)
{
    if (filterValue == "") {
        throw FilterException("Sex value is empty", "SEX", filterValue.toStdString().c_str());
    }

    if (filterValue != "M" && filterValue != "N") {
        throw FilterException("Sex value is not 'M' or 'N'", "SEX", filterValue.toStdString().c_str());
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validateCity(QString filterValue)
{
    // Try-Catch to change the possible error
    bool success = true;
    try {
        success = validateName(filterValue);
    } catch (FilterException& e) {
        throw FilterException(e.what(), "CITY", filterValue.toStdString().c_str());
    }
    return success;
}

bool InternetExplorers::InterfaceFilter::validateNationality(QString filterValue)
{
    // Try-Catch to change the possible error
    bool success = true;

    if (filterValue.length() != 2) {
        throw FilterException("Nationality length is not 2 chars", "NATIONALITY", filterValue.toStdString().c_str());
    }

//    if (filterValue.isUpper()) {
//        throw FilterException("Nationality is not all upper case", "NATIONALITY", filterValue.toStdString().c_str());
//    }

    try {
        success = validateName(filterValue);
    } catch (FilterException& e) {
        throw FilterException(e.what(), "NATIONALITY", filterValue.toStdString().c_str());
    }
    return success;
}

bool InternetExplorers::InterfaceFilter::validateBirthYear(QString filterValue)
{
    if (filterValue == "") {
        throw FilterException("Birth year value is empty", "BIRTH YEAR", filterValue.toStdString().c_str());
    }

    // Should be 2 numbers
    if (filterValue.length() != 2) {
        throw FilterException("Birth year value does not have 2 numbers", "BIRTH YEAR", filterValue.toStdString().c_str());
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validateTeam(QString filterValue)
{
    // Try-Catch to change the possible error
    bool success = true;
    try {
        success = validateName(filterValue);
    } catch (FilterException& e) {
        throw FilterException(e.what(), "TEAM", filterValue.toStdString().c_str());
    }
    return success;
}


