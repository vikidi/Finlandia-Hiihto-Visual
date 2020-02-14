#ifndef INTERFACEFILTER_H
#define INTERFACEFILTER_H

#include <map>
#include <iostream>
#include <QString>

class InterfaceFilter
{
public:
    InterfaceFilter() {}
    ~InterfaceFilter() {}

    enum Filters {
        YEAR = 0,
        YEAR_RANGE,
        DISTANCE,
        NAME
    };
    static const Filters filters;

    ///
    /// \brief
    ///     Checks that the given filters and values are valid
    /// \return
    ///     True if filter is valid.
    ///
    static bool validateFilter(std::map<Filters, QString>);

private:
    static bool validateYear(QString filterValue);
};

#endif // INTERFACEFILTER_H
