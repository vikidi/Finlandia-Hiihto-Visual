#ifndef DATAORDERER_H
#define DATAORDERER_H

#include <vector>

#include "constants.h"

namespace InternetExplorers {

using Constants::Filter::OrderFilters;

class DataOrderer
{
public:
    DataOrderer();

    void orderData(std::vector<std::vector<std::string>>& data, OrderFilters filter);

private:
    void sortByIndex(std::vector<std::vector<std::string>>& data, Constants::DataIndex::IndexInData index);
};

}

#endif // DATAORDERER_H
