#include "localapi.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDirIterator>
#include <QCryptographicHash>
#include <QTextCodec>
#include <QDebug>

LocalAPI::LocalAPI()
{

}

LocalAPI::~LocalAPI()
{

}

void LocalAPI::saveData(const std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > &data)
{

    // If old data is there, delete it
    if(QDir(DATA_ROOT_NAME).exists()) {
        qDebug() << "Poistetaan kansioita";
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
            QFile file(dfolder + QString("/") + DATA_FILE_NAME);
            if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
            {
                QTextStream stream( &file );
                stream.setCodec("UTF-8");
                stream.setGenerateByteOrderMark(true);

                // Do through rows
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

    // Create MD5 checksum metadata file
    createMD5File();
}

std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > LocalAPI::loadData()
{
    std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > data = {};

    // Go throug data if it is there
    if(!QDir(DATA_ROOT_NAME).exists()) {
        qDebug() << "Data-folder was not found";
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
    return ((!isDataAvailable()) || isDataCorrupted());
}

void LocalAPI::createMD5File()
{
    // Check if MD5 metadata file already exists
    if (QFile::exists(MD5_DATA_FILE_NAME)) {

        // Delete it if exists
        QFile f(MD5_DATA_FILE_NAME);
        f.remove();
    }

    // Create file with the rows
    QFile file(MD5_DATA_FILE_NAME);
    if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream stream( &file );
        stream.setCodec("UTF-8");
        stream.setGenerateByteOrderMark(true);

        // Go throug all files
        QDirIterator it(DATA_ROOT_NAME, QStringList() << "Data.txt", QDir::NoFilter, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QFile f(it.next());

            QString name = f.fileName();

            QByteArray checkSum = getMD5CheckSum(name);

            QString DataAsString = QString(checkSum.toHex());

            // Add row to MD5 metadata file
            stream << name << ";" << DataAsString << endl;
        }
    }
}

QByteArray LocalAPI::getMD5CheckSum(const QString &file)
{
    QFile f(file);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Algorithm::Md5);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}

bool LocalAPI::isDataCorrupted()
{
    return false;
}

bool LocalAPI::isDataAvailable()
{
    if(!QDir(DATA_ROOT_NAME).exists()) {
        return false;
    }

    // Iterate year folders
    QDirIterator it(DATA_ROOT_NAME, QDirIterator::NoIteratorFlags);
    while (it.hasNext()) {
        it.next();

        if (it.fileName() == "." || it.fileName() == "..") {
            continue;
        }

        // Iterate distance folders
        QDirIterator it2(it.filePath(), QDirIterator::NoIteratorFlags);
        while (it2.hasNext()) {
            it2.next();

            if (it2.fileName() == "." || it2.fileName() == "..") {
                continue;
            }

            // Check that data file exists
            if (!QFile::exists(it2.filePath() + QString("/") + DATA_FILE_NAME)) {
                return false;
            }
        }
    }

    return true;
}
