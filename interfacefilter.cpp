#include "interfacefilter.h"

bool InterfaceFilter::validateFilter(std::map<InterfaceFilter::Filters, QString> filters)
{
    bool isOK = true;

    for(auto& filter : filters) {
        switch(filter.first) {

            case InterfaceFilter::YEAR:
                isOK = validateYear(filter.second);
                break;

            default:
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
    if (filterValue < "1974" || filterValue > "2019") {
        return false;
    }

    return true;
}


