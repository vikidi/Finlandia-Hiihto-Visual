#include "dataorderer.h"

#include <algorithm>
#include <functional>

InternetExplorers::DataOrderer::DataOrderer()
{

}

void InternetExplorers::DataOrderer::orderData(std::vector<std::vector<std::string> > &data, InternetExplorers::Constants::Filter::OrderFilters filter)
{
    switch(filter) {
    case OrderFilters::AGE:
        sortByIndex(data, Constants::DataIndex::IndexInData::BIRTH_YEAR); // Kinda stupid because of birth year :/
        break;
    case OrderFilters::PLACEMENT:
        sortByIndex(data, Constants::DataIndex::IndexInData::PLACE);
        break;
    case OrderFilters::ALPH_CITY:
        sortByIndex(data, Constants::DataIndex::IndexInData::CITY);
        break;
    case OrderFilters::ALPH_NAME:
        sortByIndex(data, Constants::DataIndex::IndexInData::NAME);
        break;
    case OrderFilters::ALPH_TEAM:
        sortByIndex(data, Constants::DataIndex::IndexInData::TEAM);
        break;
    case OrderFilters::YEAR_ORDER:
        sortByIndex(data, Constants::DataIndex::IndexInData::YEAR);
        break;
    case OrderFilters::DISTANCE_ORDER:
        sortByIndex(data, Constants::DataIndex::IndexInData::DISTANCE);
        break;
    case OrderFilters::ALPH_NATIONALITY:
        sortByIndex(data, Constants::DataIndex::IndexInData::NATIONALITY);
        break;
    }
}

void InternetExplorers::DataOrderer::sortByIndex(std::vector<std::vector<std::string> > &data, InternetExplorers::Constants::DataIndex::IndexInData index)
{
    // Sort with custom lambda expression
    std::sort(data.begin(), data.end(), [index](std::vector<std::string> a, std::vector<std::string> b) {
        return a[index] < b[index];
    });
}
