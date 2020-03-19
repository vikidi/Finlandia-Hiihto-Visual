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

}

/*!
 * \brief
 * Namespace for interface filter constants
 */
namespace Filter {

/*!
 * \brief
 * All of the value related filter options in interface filter
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
const size_t ROW_SIZE = 12;

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
