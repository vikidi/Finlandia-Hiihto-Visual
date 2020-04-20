#include "localdataloader.h"

#include <QDirIterator>
#include <QTextStream>
#include "crypter.h"
#include "../constants.h"

InternetExplorers::LocalDataLoader::LocalDataLoader() :
    m_data({}),
    m_encrypted(false)
{
    if (QFile::exists("encryptionStatus.ini")) {
        QFile file("encryptionStatus.ini");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            for(auto line : file.readAll().split('\n'))
            {
                // "Chopped" would be better than chop but
                // it is not supported on remote desktops qt version
                auto lineCopy{line};
                lineCopy.chop(2);
                if(lineCopy.size() == 0) continue;
                if(lineCopy == "LocalDataEncrypted")
                {
                    if(line.at(line.size() - 1) == '1')
                    {
                        m_encrypted = true;
                    }
                }
            }
        }
    } else
    { // File not found
        m_encrypted = false;
    }
}

std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> InternetExplorers::LocalDataLoader::loadData(std::shared_ptr<std::vector<std::string> > years)
{

    QDirIterator it(Constants::DATA_ROOT_NAME, QStringList() << Constants::DATA_FILE_NAME, QDir::NoFilter, QDirIterator::Subdirectories);
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

        emit progressChanged();
    }

    if(m_encrypted)
    { // Decrypt names in the data
        InternetExplorers::Crypter crypter;
        for(auto& year : m_data)
        {
            for(auto& distance : year.second)
            {
                crypter.decryptNames(distance.second);
            }
        }
    }

    return m_data;
}
