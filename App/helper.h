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

    static long timeToMSecs(const QString time);
    static long timeToMSecs(const std::string time);

    static QString mSecsToQString(const unsigned long long time);
    static std::string mSecsToString(const unsigned long long time);

    static double mSecsToH(const unsigned long long time);

    static int getLowerFullTen(const int num);
    static int getUpperFullTen(const int num);

    static int parseKMFromDistance(const std::string distance);

private:
    Helper() {}
};

}

#endif // HELPER_H
