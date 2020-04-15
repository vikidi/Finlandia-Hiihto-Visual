#include "dataorderer.h"

#include <algorithm>
#include <functional>
#include <QTime>
#include <limits>

InternetExplorers::DataOrderer::DataOrderer()
{

}

void InternetExplorers::DataOrderer::orderData(std::vector<std::vector<std::string> > &data, InternetExplorers::Constants::Filter::OrderFilters filter)
{
    switch(filter) {
    case OrderFilters::AGE:
        sortByIndexNum(data, Constants::DataIndex::IndexInData::BIRTH_YEAR); // Kinda stupid because of birth year :/
        break;
    case OrderFilters::PLACEMENT:
        sortByIndexNum(data, Constants::DataIndex::IndexInData::PLACE);
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
        sortByIndexNum(data, Constants::DataIndex::IndexInData::YEAR);
        break;
    case OrderFilters::DISTANCE_ORDER:
        sortByIndex(data, Constants::DataIndex::IndexInData::DISTANCE);
        break;
    case OrderFilters::ALPH_NATIONALITY:
        sortByIndex(data, Constants::DataIndex::IndexInData::NATIONALITY);
        break;
    case OrderFilters::TIME:
        sortByTime(data);
        break;
    }
}

void InternetExplorers::DataOrderer::sortByIndex(std::vector<std::vector<std::string> > &data, InternetExplorers::Constants::DataIndex::IndexInData index)
{
    // Sort with custom lambda expression
    std::sort(data.begin(), data.end(), [index](std::vector<std::string> a, std::vector<std::string> b) {
        std::string f = a[index];
        std::string s = b[index];
        std::transform(f.begin(), f.end(), f.begin(), ::tolower);
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return f < s;
    });
}

void InternetExplorers::DataOrderer::sortByIndexNum(std::vector<std::vector<std::string> > &data, InternetExplorers::Constants::DataIndex::IndexInData index)
{
    // Sort with custom lambda expression
    std::sort(data.begin(), data.end(), [index](std::vector<std::string> a, std::vector<std::string> b) {

        int f, s;
        try {
            f = std::stoi(a[index]);
        } catch (std::exception) {
            f = INT_MAX;
        }
        try {
            s = std::stoi(b[index]);
        } catch (std::exception) {
            s = INT_MAX;
        }

        return f < s;
    });
}

void InternetExplorers::DataOrderer::sortByTime(std::vector<std::vector<std::string> > &data)
{
    // Sort with custom lambda expression
    std::sort(data.begin(), data.end(), [](std::vector<std::string> a, std::vector<std::string> b) {

        int f, s;
        QString as = QString::fromStdString(a[Constants::DataIndex::IndexInData::TIME]);
        QString bs = QString::fromStdString(b[Constants::DataIndex::IndexInData::TIME]);

        if (as.contains('.')) {
            f = QTime(0, 0, 0).msecsTo(QTime::fromString(as, "h:mm:ss.z"));
        } else {
            f = QTime(0, 0, 0).msecsTo(QTime::fromString(as, "h:mm:ss"));
        }

        if (bs.contains('.')) {
            s = QTime(0, 0, 0).msecsTo(QTime::fromString(bs, "h:mm:ss.z"));
        } else {
            s = QTime(0, 0, 0).msecsTo(QTime::fromString(bs, "h:mm:ss"));
        }

        return f < s;
    });
}
