#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <QString>

namespace InternetExplorers {

/*!
 * \brief The Helper class
 * Static class that offers universal helper functions
 */
class Helper
{
public:
    /*!
     * \brief
     * Copy constructor not allowed
     */
    Helper(const Helper&) = delete;

    /*!
     * \brief
     * Copy constructor not allowed
     */
    Helper& operator=(const Helper&) = delete;

    /*!
     * \brief timeToMSecs
     * Turns "h:mm:ss" and "h:mm:ss.z" format times to seconds
     * \param time
     * Time in format "h:mm:ss" and "h:mm:ss.z"
     * \return
     * Time as milliseconds
     * \attention
     * Returns -1 if parsing does not succeed
     */
    static long timeToMSecs(const QString time);

    /*!
     * \brief timeToMSecs
     * Turns "h:mm:ss" and "h:mm:ss.z" format times to seconds
     * \param time
     * Time in format "h:mm:ss" and "h:mm:ss.z"
     * \return
     * Time as milliseconds
     * \attention
     * Returns -1 if parsing does not succeed
     */
    static long timeToMSecs(const std::string time);

    /*!
     * \brief mSecsToQString
     * Turns the milliseconds to "h:mm:ss.z" format
     * \param time
     * Time in milliseconds
     * \return
     * The time as QString
     */
    static QString mSecsToQString(const unsigned long long time);

    /*!
     * \brief mSecsToQString
     * Turns the milliseconds to "h:mm:ss.z" format
     * \param time
     * Time in milliseconds
     * \return
     * The time as std::string
     */
    static std::string mSecsToString(const unsigned long long time);

    /*!
     * \brief mSecsToH
     * Turns time in milliseconds to hours
     * \param time
     * Time in milliseconds
     * \return
     * Time in hours
     */
    static double mSecsToH(const unsigned long long time);

    /*!
     * \brief getLowerFullTen
     * Rounds to the closest smaller full ten
     * \param num
     * Number to round
     * \attention
     * num must be positive
     * \return
     * The rounded number
     */
    static int getLowerFullTen(const int num);

    /*!
     * \brief getLowerFullTen
     * Rounds to the closest bigger full ten
     * \param num
     * Number to round
     * \attention
     * num must be positive
     * \return
     * The rounded number
     */
    static int getUpperFullTen(const int num);

    /*!
     * \brief parseKMFromDistance
     * Parses the kilometers from finlandia hiihto distance string
     * \param distance
     * The distance to parse
     * \return
     * Distances kilometers
     */
    static int parseKMFromDistance(const std::string distance);

private:
    Helper() {}
};

}

#endif // HELPER_H
