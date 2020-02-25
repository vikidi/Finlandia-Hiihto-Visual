#include "interfacefilter.h"

#include <QStringList>
#include <algorithm>
#include <cctype>

#define yeet throw // :)

bool InternetExplorers::InterfaceFilter::validateFilter(std::map<InterfaceFilter::ValueFilters, QString> filters)
{
    bool isOK = true;

    // Can't have year and year range filters both
    if (filters.find(InterfaceFilter::ValueFilters::YEAR) != filters.end()) {
        if (filters.find(InterfaceFilter::ValueFilters::YEAR_RANGE) != filters.end()) {
            yeet FilterException("Can not use Year and Year range same time");
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
        yeet FilterException("Year value empty");
    }

    if (filterValue.toInt() < 1974 || filterValue.toInt() > 2019) {
        yeet FilterException("Year value not between 1974-2019");
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validateYearRange(QString filterValue)
{
    if (filterValue == "") {
        yeet FilterException("Year range value empty");
    }

    if (!filterValue.contains(';')) {
        yeet FilterException("Year range value does not have separator ';'");
    }

    // Should be in style firstYear;secondYear eg. 2014;2018
    QStringList years = filterValue.split(";");

    if (years.length() != 2) {
        yeet FilterException("Year range value does not have two years");
    }

    int lower = years[0].toInt();
    int upper = years[1].toInt();

    if (   lower < 1974
        || lower > 2019
        || upper < 1974
        || upper > 2019) {
        yeet FilterException("Year range value is not between range 1974-2019");
    }

    // TODO Which is better > or >=?
    if (lower >= upper) {
        yeet FilterException("Year range values lower bound is bigger than upper");
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validateDistance(QString filterValue)
{
    if (filterValue == "") {
        yeet FilterException("Distance value is empty");
    }

    if (std::find(DISTANCES.begin(), DISTANCES.end(), filterValue.toStdString()) == DISTANCES.end()) {
        yeet FilterException("Distance value is not valid");
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validateName(QString filterValue)
{
    if (filterValue == "") {
        yeet FilterException("Name value is empty");
    }

    // Filter value to lower case
    std::string filterVal = filterValue.toStdString();
    std::transform(filterVal.begin(), filterVal.end(), filterVal.begin(),
        [](unsigned char c){ return std::tolower(c); });

    QString alphabets("?+-_\\/`´'*.:,;€^¨~|=})]([{&%¤$#£\"@!½§<>");
    for(auto& letter : filterVal) {
        if(alphabets.contains(letter)) {
            yeet FilterException("Name value contains unallowed characters");
        }
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validatePlace(QString filterValue)
{
    if (filterValue == "") {
        yeet FilterException("Placing value is empty");
    }

    int place = filterValue.toInt();
    if (place < 1) {
        yeet FilterException("Placing value is smaller than 1");
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validatePlaceMen(QString filterValue)
{
    return validatePlace(filterValue);
}

bool InternetExplorers::InterfaceFilter::validatePlaceWomen(QString filterValue)
{
    return validatePlace(filterValue);
}

bool InternetExplorers::InterfaceFilter::validateSex(QString filterValue)
{
    if (filterValue == "") {
        yeet FilterException("Sex value is empty");
    }

    if (filterValue != "M" && filterValue != "N") {
        yeet FilterException("Sex value is not 'M' or 'N'");
    }

    return true;
}

bool InternetExplorers::InterfaceFilter::validateBirthYear(QString filterValue)
{
    if (filterValue == "") {
        yeet FilterException("Birth year value is empty");
    }

    // Should be 2 numbers
    if (filterValue.length() != 2) {
        yeet FilterException("Birth year value does not have 2 numbers");
    }

    return true;
}


