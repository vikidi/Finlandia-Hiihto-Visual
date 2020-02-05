#include "localapi.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

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
    return {};
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
    return true;
}
