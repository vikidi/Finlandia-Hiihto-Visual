#include "localdataloader.h"

#include <QDirIterator>
#include <QTextStream>
#include "logger.h"

InternetExplorers::LocalDataLoader::LocalDataLoader() :
    m_data({})
{

}

std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> InternetExplorers::LocalDataLoader::loadData(std::shared_ptr<std::vector<std::string> > years)
{
    using logger = InternetExplorers::Logger;

    QDirIterator it(DATA_ROOT_NAME, QStringList() << "Data.txt", QDir::NoFilter, QDirIterator::Subdirectories);
    while (it.hasNext()) {

        QFile f(it.next());

        f.open(QIODevice::ReadOnly);

        std::string name = f.fileName().toStdString();

        // Parse year and distance
        std::size_t first = name.find("/");
        std::size_t second = name.find("/", first + 1);
        std::size_t last = name.find("/", second + 1);

        QString year = QString::fromStdString(name.substr (first + 1, second - first - 1));

        // Check that the year is for this loader
        if (std::find(years->begin(), years->end(), year.toStdString()) == years->end()) {
            continue;
        }

        QString distance = QString::fromStdString(name.substr (second + 1, last - second - 1));

        // Check that year exists
        if( m_data.find(year) == m_data.end() ) {
            m_data.insert( {year, {  }} );
        }

        // Check that distance exists
        if( m_data[year].find(distance) == m_data[year].end() ) {
            m_data[year].insert( {distance, {  }} );
        }

        // Read lines
        QTextStream in(&f);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList parts = line.split(";");

            // Go through columns
            std::vector<std::string> row = {};
            for(auto& col : parts) {
                row.emplace_back(col.toStdString());
            }

            // Last is extra because of split
            row.pop_back();

            m_data[year][distance].emplace_back(row);
        }

        f.close();

        QString msg = year + QString(" ") + distance + QString(" loaded");
        logger::getInstance().log(msg, InternetExplorers::Logger::Severity::INFO);

        emit progressChanged();
    }

    return m_data;
}
