#ifndef DATAORDERER_H
#define DATAORDERER_H

#include <vector>
#include <string>
#include "constants.h"

namespace InternetExplorers {

using Constants::Filter::OrderFilters;

/*!
 * \brief The DataOrderer class
 * handles the ordering/sorting of data rows
 */
class DataOrderer
{
public:
    DataOrderer();

    /*!
     * \brief
     * Sorts the given data according to the given filter parameter
     * \param data
     * The data to be sorted
     * \param filter
     * tells the way to sort. References to InternetExplorers::Constants::Filters::OrderFilters
     */
    void orderData(std::vector<std::vector<std::string>>& data, OrderFilters filter);

private:
    void sortByIndex(std::vector<std::vector<std::string>>& data, Constants::DataIndex::IndexInData index);
    void sortByIndexNum(std::vector<std::vector<std::string>>& data, Constants::DataIndex::IndexInData index);
    void sortByTime(std::vector<std::vector<std::string>>& data);
};

}

#endif // DATAORDERER_H
