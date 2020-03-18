#ifndef INTERFACEFILTER_H
#define INTERFACEFILTER_H

#include <map>
#include <iostream>
#include <QString>
#include <vector>
#include <QException>

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
     * All of the value related filter options
     */
    enum ValueFilters {
        YEAR = 0,       /*!< Year of the race */
        YEAR_RANGE,     /*!< Year range for the races */
        DISTANCE,       /*!< Distance/trip of the race */
        NAME,           /*!< Name of the skier */
        TIME_RANGE,     /*!< Result times range */
        PLACE,          /*!< Placing of the skier in general */
        PLACE_MEN,      /*!< Placing of the skier considering only men */
        PLACE_WOMEN,    /*!< Placing of the skier considering only women */
        SEX,            /*!< Sex of the skier */
        CITY,           /*!< Home city of the skier */
        NATIONALITY,    /*!< Nationality of the skier */
        BIRTH_YEAR,     /*!< Birth year of the skier */
        TEAM            /*!< Team name of the skier */
    };

    /*!
     * \brief
     * All of the sorting/ordering related filter options
     * \todo
     * These sorting options are not implemented yet
     */
    enum OrderFilters {
        PLACEMENT = 0,      /*!< Sort by placement */
        PLACEMENT_MEN,      /*!< Sort by placement within men */
        PLACEMENT_WOMEN,    /*!< Sort by placement within women */
        AGE,                /*!< Sort by age of the skier */
        ALPH_NATIONALITY,   /*!< Sort alphabetically by nationality */
        ALPH_NAME,          /*!< Sort alphabetically by name */
        ALPH_TEAM,          /*!< Sort alphabetically by team */
        ALPH_CITY,          /*!< Sort alphabetically by city */
        TIME                /*!< Sort by result time*/
    };

    /*!
     * \brief
     * Validates the filter and its values.
     * \details
     * Checks that the given filter and its values<br>
     * are correct and contain values within wanted boudaries.<br>
     * This function should be called every time
     * \param filters
     * The filter to be validated
     * \return
     * True if the filter is valid.
     * \todo
     * Write all value boundaries eg. cases when exception is thrown
     * \exception FilterException
     * is thrown if filter or its value is incorrect
     */
    static bool validateFilter(std::map<ValueFilters, QString> filters);

private:
    // TODO: should these be public?
    static bool validateYear(QString filterValue);
    static bool validateYearRange(QString filterValue);
    static bool validateDistance(QString filterValue);
    static bool validateName(QString filterValue);
    static bool validateTimeRange(QString filterValue);
    static bool validatePlace(QString filterValue);
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
