#ifndef INTERFACEFILTER_H
#define INTERFACEFILTER_H

#include <map>
#include <iostream>
#include <QString>
#include <vector>
#include <QException>

#include "constants.h"

namespace InternetExplorers
{

/*!
 * \brief
 * Custom exception that is used in filtering errors.
 */
class FilterException : public std::exception
{
public:

    /*!
     * \brief
     * Default constructor
     * \param msg
     * Message for the exception
     * \param filterName
     * The filter parameter where error occurred
     * \param filterValue
     * The filter parameters value that caused the error
     */
    FilterException(const char* msg = "", const char* filterName = "", const char* filterValue = "") :
        message(msg),
        filterName(filterName),
        filterValue(filterValue) {}

    ~FilterException() {}

    /*!
     * \brief
     * Gets the message of the error
     * \return
     * Message text of the error
     */
    const char *what() const noexcept { return message.c_str(); }

    /*!
     * \brief
     * Gets the filter parameters name
     * \return
     * The filters name
     */
    const char *getFilterName() const noexcept { return filterName.c_str(); }

    /*!
     * \brief
     * Gets the filters value
     * \return
     * Filter parameters value
     */
    const char *getFilterValue() const noexcept { return filterValue.c_str(); }

private:
    std::string message;        // Message text
    std::string filterName;     // Filters name
    std::string filterValue;    // Filters value
};

/*!
 * \brief
 * Handles the interfaces filter
 * \details
 * This class is a declaration of the backend interface parameters.<br>
 * It handles the possible filter parameters<br>
 * and the validation of the values given to them.
 */
class InterfaceFilter
{
public:
    InterfaceFilter() {}
    ~InterfaceFilter() {}

    /*!
     * \brief
     * Validates the filter and its values.
     * \details
     * Checks that the given filter and its values
     * are correct and contain values within wanted boudaries.<br>
     * This function should be called every time fetching data.
     * \param filters
     * The filter to be validated
     * \return
     * True if the filter is valid.
     * \exception FilterException
     * is thrown if filter or its value is incorrect.
     * Details for these can be found from Contants namespace in ValueFilter enum.
     */
    static bool validateFilter(std::map<Constants::Filter::ValueFilters, QString> filters);

private:
    static bool validateYear(QString filterValue);
    static bool validateYearRange(QString filterValue);
    static bool validateDistance(QString filterValue);
    static bool validateName(QString filterValue);
    static bool validateTimeRange(QString filterValue);
    static bool validatePlace(QString filterValue);
    static bool validatePlaceRange(QString filterValue);
    static bool validatePlaceRangeMen(QString filterValue);
    static bool validatePlaceRangeWomen(QString filterValue);
    static bool validatePlaceMen(QString filterValue);
    static bool validatePlaceWomen(QString filterValue);
    static bool validateSex(QString filterValue);
    static bool validateCity(QString filterValue);
    static bool validateNationality(QString filterValue);
    static bool validateBirthYear(QString filterValue);
    static bool validateTeam(QString filterValue);
};

}

#endif // INTERFACEFILTER_H
