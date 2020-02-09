#include "localapi.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDirIterator>
#include <QDebug>

LocalAPI::LocalAPI()
{

}

LocalAPI::~LocalAPI()
{

}

void LocalAPI::saveData(const std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > &data)
{
    QString path = QFileInfo(QCoreApplication::applicationDirPath()).path();

    // If old data is there, delete it
    if(QDir(DATA_ROOT_NAME).exists()) {
        QDir(DATA_ROOT_NAME).removeRecursively();
    }

    // Create the root folder
    QDir().mkdir(DATA_ROOT_NAME);

    // Go through years
    for(auto& year : data) {

        // Create the year folder
        QString yfolder = DATA_ROOT_NAME + QString("/") + year.first;
        QDir().mkdir(yfolder);

        //Go through distances
        for(auto& distance : year.second) {

            // Create the distance folder
            QString dfolder = yfolder + QString("/") + distance.first;
            QDir().mkdir(dfolder);

            // Create file with the rows
            QFile file(path + QString("/") + dfolder + QString("/") + DATA_FILE_NAME);
            if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
            {
                QTextStream stream( &file );
                stream.setCodec("UTF-8");
                stream.setGenerateByteOrderMark(true);

                // Do throug rows
                for(auto& row : distance.second) {
                    for(auto& value : row) {
                        stream << QString::fromStdString(value) << ";";
                    }

                    // Add line end
                    stream << endl;
                }
            }

            file.close();
        }
    }
}

std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > LocalAPI::loadData()
{
    std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > data = {};
    QString path = QFileInfo(QCoreApplication::applicationDirPath()).path();

    // Go throug data if it is there
    if(!QDir(DATA_ROOT_NAME).exists()) {
        return {};
    }

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
        QString distance = QString::fromStdString(name.substr (second + 1, last - second - 1));

        // Check that year exists
        if( data.find(year) == data.end() ) {
            data.insert( {year, {  }} );
        }

        // Check that distance exists
        if( data[year].find(distance) == data[year].end() ) {
            data[year].insert( {distance, {  }} );
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

            data[year][distance].emplace_back(row);
        }

        f.close();
    }

    return data;
}

bool LocalAPI::needsToBeLoadedFromWeb()
{
    return (!isDataAvailable() && isDataCorrupted());
}

bool LocalAPI::isDataCorrupted()
{
    return false;
}

bool LocalAPI::isDataAvailable()
{
    if(QDir(DATA_ROOT_NAME).exists()) {
        return true;
    }

    return false;
}
