#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <vector>

/*!
 * \brief
 * Namespace that contains all constants used in the program.
 */
namespace InternetExplorers::Constants {

/*!
 * \brief
 * The root folder for data
 */
const QString DATA_ROOT_NAME = "FinlandiaData";

/*!
 * \brief
 * Local data files name on disk
 */
const QString DATA_FILE_NAME = "Data.txt";

/*!
 * \brief
 * Local meta data folders name on disk
 */
const QString MD5_DATA_FILE_NAME = "MD5_metadata.txt";

/*!
 * \brief
 * Local meta data files name on disk
 */
const QString META_DATA_FILE_NAME = "Metadata.txt";

/*!
 * \brief
 * Name for the log folder
 */
const QString LOG_FOLDER_NAME = "logs";

/*!
 * \brief
 * Name for the log file
 */
const QString LOG_FILE_NAME = "log.txt";

/*!
 * \brief
 * All possible distances
 */
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
 * Namespace for logger constants
 */
namespace Logger {

/*!
 * \brief
 * Holds possible values for log's severity
 */
enum Severity {
    INFO = 0,   /*!< Basic info logging */
    WARNING,    /*!< Unwanted behavior, possibly dangerous */
    CRITICAL    /*!< Critical error in program */
};

/*!
 * \brief
 * Amount of messages that gets written at once to the disk
 */
constexpr size_t WRITE_BUFFER_MAX_SIZE = 100;

}

/*!
 * \brief
 * Namespace for interface filter constants
 */
namespace Filter {

/* ValueFilter Documentation */

/*!
 * \var ValueFilters::YEAR
 * \brief
 * Single year for the races
 * \details
 * Value can not be empty.<br>
 * Value must be between [1974, 2019].<br>
 * Value is parsed with QString::toInt(), which has it's special functionality.
 * \attention
 * This can not be used with YEAR_RANGE filter.
 * \note Example:
 * 2017
 */

/*!
 * \var ValueFilters::YEAR_RANGE
 * \brief
 * Year range for the races
 * \details
 * Value can not be empty.<br>
 * Value must contain ";" as separator between start and end year.<br>
 * Start year is the first one before ";", end year is after that.<br>
 * Both years will be included in the results -> range is [start, end].<br>
 * Value must contain exactly two years defined.<br>
 * Both years must be between [1974, 2019].<br>
 * Start year must be smaller (<) than end year.
 * \attention
 * This can not be used with YEAR filter.
 * \note Example:
 * 1980;1985
 */

/*!
 * \var ValueFilters::DISTANCE
 * \brief
 * Distance/trip of the race
 * \details
 * Value can not be empty.<br>
 * Value must be exactly one that is in InternetExplorers::Constants::DISTANCES.
 * \note Example:
 * V50
 */

/*!
 * \var ValueFilters::NAME
 * \brief
 * Name of the skier
 * \details
 * Value can not be empty.<br>
 * Search does not mind about upper and lower case.<br>
 * Search only finds results with exactly same name
 * (turns both first to lower case).<br>
 * Value may not contain any special characters.
 * \note Example:
 * Mursu Esa
 */

/*!
 * \var ValueFilters::TIME_RANGE
 * \brief
 * Result time's range.
 * \details
 * Value can not be empty.<br>
 * Value must contain ";" as separator between start and end time.<br>
 * Start time is the first one before ";", end time is after that.<br>
 * Both times will be included in the results -> range is [start, end].<br>
 * Value must contain exactly two times defined.
 * Time must be in format h:mm:ss.<br>
 * Time can have more than 9 hours.<br>
 * Both times must be in the valid format described above.<br>
 * Start time must be smaller or equal (<) to end time.<br>
 * Hours must be greater or equal (>=) to 0.<br>
 * Minutes must be between [0, 59].<br>
 * Seconds must be between [0, 59].
 * \note Example:
 * 5:10:27;12:05:15
 */

/*!
 * \var ValueFilters::PLACE
 * \brief
 * Placing of the skier in general
 * \details
 * Value can not be empty.<br>
 * Value must be greater or equal (>=) to 1.
 * \attention
 * This can not be used with PLACE_RANGE filter.
 * \note Example:
 * 5
 */

/*!
 * \var ValueFilters::PLACE_RANGE
 * \brief
 * Range of placing of the skier in general
 * \details
 * Value can not be empty.<br>
 * Value must contain ";" as separator between start and end place.<br>
 * Start place is the first one before ";", end place is after that.<br>
 * Both places will be included in the results -> range is [start, end].<br>
 * Value must contain exactly two places defined.<br>
 * Both places must be greater or equal (>=) to 1.<br>
 * Start place must be smaller (<) than end place.
 * \attention
 * This can not be used with PLACE filter.
 * \note Example:
 * 1;5
 */

/*!
 * \var ValueFilters::PLACE_MEN
 * \brief
 * Placing of the skier considering only men
 * \details
 * Value can not be empty.<br>
 * Value must be greater or equal (>=) to 1.
 * \note Example:
 * 4
 */

/*!
 * \var ValueFilters::PLACE_WOMEN
 * \brief
 * Placing of the skier considering only women
 * \details
 * Value can not be empty.<br>
 * Value must be greater or equal (>=) to 1.
 * \note Example:
 * 2
 */

/*!
 * \var ValueFilters::SEX
 * \brief
 * Sex/gender of the skier
 * \details
 * Value can not be empty.<br>
 * Value must be either "M" or "N".
 * \note Example:
 * M
 */

/*!
 * \var ValueFilters::CITY
 * \brief
 * Home city of the skier.
 * \details
 * Value can not be empty.<br>
 * Search does not mind about upper and lower case.<br>
 * Search only finds results with exactly same city
 * (turns both first to lower case).<br>
 * Value may not contain any special characters.
 * \note Example:
 * Raisio
 */

/*!
 * \var ValueFilters::NATIONALITY
 * \brief
 * Nationality of the skier
 * \details
 * Value can not be empty.<br>
 * Search only finds results with exactly same nationality.<br>
 * Value may not contain any special characters.
 * Value must have exactly two characters.
 * \note Example:
 * FI
 */

/*!
 * \var ValueFilters::BIRTH_YEAR
 * \brief
 * Birth year of the skier
 * \details
 * Value can not be empty.<br>
 * Value must be exactly two numbers that describe the birth year.
 * \note Example:
 * 89
 */

/*!
 * \var ValueFilters::TEAM
 * \brief
 * Team name of the skier
 * \details
 * Value can not be empty.<br>
 * Search does not mind about upper and lower case.<br>
 * Search only finds results with exactly same team name
 * (turns both first to lower case).<br>
 * Value may not contain any special characters.
 * \note Example:
 * Vantaan Hiihtoseura
 */

/* /ValueFilter Documentation */

/*!
 * \brief
 * All of the value related filter options in interface filter
 * \details
 * These filters are used to filter in searches the wanted data.<br>
 * User can define with these parameters the conditions for result data.
 * \attention
 * YEAR and YEAR_RANGE filters can not be used together.<br>
 * PLACE and PLACE_RANGE filters can not be used together.
 */
enum ValueFilters {
    YEAR = 0,
    YEAR_RANGE,
    DISTANCE,
    NAME,
    TIME_RANGE,
    PLACE,
    PLACE_RANGE,
    PLACE_MEN,
    PLACE_WOMEN,
    PLACE_RANGE_MEN,
    PLACE_RANGE_WOMEN,
    SEX,
    CITY,
    NATIONALITY,
    BIRTH_YEAR,
    TEAM
};

/*!
 * \brief
 * All of the sorting/ordering related filter options
 * \todo
 * These sorting options are not implemented yet
 */
enum OrderFilters {
    PLACEMENT = 0,      /*!< Sort by placement */
    YEAR_ORDER,         /*!< Sort by year */
    DISTANCE_ORDER,     /*!< Sort by distance */
    AGE,                /*!< Sort by age of the skier */
    ALPH_NATIONALITY,   /*!< Sort alphabetically by nationality */
    ALPH_NAME,          /*!< Sort alphabetically by name */
    ALPH_TEAM,          /*!< Sort alphabetically by team */
    ALPH_CITY,          /*!< Sort alphabetically by city */
};

}

/*!
 * \brief
 * Namespace for constant indexes in data
 */
namespace DataIndex {

/*!
 * \brief
 * The amount of columns in data row
 */
constexpr size_t ROW_SIZE = 12;

/*!
 * \brief
 * Tells the index for parameters in the data
 */
enum IndexInData {
    YEAR = 0,       /*!< Index of year */
    DISTANCE,       /*!< Index of distance */
    TIME,           /*!< Index of time */
    PLACE,          /*!< Index of overall placing */
    PLACE_MEN,      /*!< Index of placing in men */
    PLACE_WOMEN,    /*!< Index of placing in women */
    SEX,            /*!< Index of sex */
    NAME,           /*!< Index of name */
    CITY,           /*!< Index of city */
    NATIONALITY,    /*!< Index of nationality */
    BIRTH_YEAR,     /*!< Index of birth year */
    TEAM            /*!< Index of team name */
};

}

}

#endif // CONSTANTS_H
