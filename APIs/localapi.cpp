#include "localapi.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDirIterator>
#include <QCryptographicHash>
#include <QTextCodec>
#include <QDebug>
#include <thread>

#include "APIs/localdataloader.h"

InternetExplorers::LocalAPI::LocalAPI() :
    m_fileCount(0),
    m_currentProgress(0),
    m_maxProgress(getAmountOfFiles()),
    m_data({})
{

}

InternetExplorers::LocalAPI::~LocalAPI()
{

}

void InternetExplorers::LocalAPI::saveData(const std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > &data)
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

std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > InternetExplorers::LocalAPI::loadData()
{
    // Go throug data if it is there
    if(!QDir(DATA_ROOT_NAME).exists()) {
        qDebug() << "Data-folder was not found";
        return {};
    }

    size_t optimalAmountOfThreads(std::thread::hardware_concurrency());

    // Create year vectors
    auto years = std::vector<std::string>();
    for (int i = 1974; i < 2020; i++) {
        years.emplace_back(std::to_string(i));
    }
    std::vector<std::shared_ptr<std::vector<std::string>>> yearVectors = SplitVector(years, optimalAmountOfThreads);

    std::vector<std::thread> threads;
    threads.reserve(optimalAmountOfThreads);
    for(size_t i = 0; i < optimalAmountOfThreads; i++)
    {
        threads.push_back(std::thread(&LocalAPI::loadDataInThread, this, yearVectors[i]));
    }

    for (auto& th : threads) {
        th.join();
    }

    return m_data;
}

template<typename T>
std::vector<std::shared_ptr<std::vector<T>>> InternetExplorers::LocalAPI::SplitVector(const std::vector<T>& vec, size_t n)
{
    std::vector<std::shared_ptr<std::vector<T>>> outVec;

    size_t length = vec.size() / n;
    size_t remain = vec.size() % n;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < std::min(n, vec.size()); ++i)
    {
        end += (remain > 0) ? (length + !!(remain--)) : length;

        std::vector<T> vector = std::vector<T>(vec.begin() + begin, vec.begin() + end);

        outVec.push_back(std::make_shared<std::vector<T>>(vector));

        begin = end;
    }

    return outVec;
}

bool InternetExplorers::LocalAPI::needsToBeLoadedFromWeb()
{
    return ((!isDataAvailable()) || isDataCorrupted());
}

std::map<QString, QString> InternetExplorers::LocalAPI::readMetaDataFile()
{
    return {};
}

void InternetExplorers::LocalAPI::updateProgress()
{
    m_fileCount++;

    int progress = static_cast<int>(100 * (m_fileCount/static_cast<double>(m_maxProgress)));

    if (progress > m_currentProgress) {
        m_currentProgress = progress;
        emit progressChanged(progress);
    }
}

int InternetExplorers::LocalAPI::getAmountOfFiles()
{
    QDirIterator it(DATA_ROOT_NAME, QStringList() << "Data.txt", QDir::NoFilter, QDirIterator::Subdirectories);
    int count = 0;
    while (it.hasNext()){
        it.next();
        count++;
    }
    return count;
}

void InternetExplorers::LocalAPI::loadDataInThread(std::shared_ptr<std::vector<std::string> > years)
{
    LocalDataLoader loader;
    connect(&loader, &LocalDataLoader::progressChanged, this, &LocalAPI::updateProgress);
    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> data = loader.loadData(years);
    appendData(data);
}

void InternetExplorers::LocalAPI::appendData(std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > &data)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    m_data.insert(data.begin(), data.end());
}

std::vector<std::pair<QString, QString> > InternetExplorers::LocalAPI::readMD5File()
{
    return {};
}

void InternetExplorers::LocalAPI::createGeneralMetaDataFile()
{

}

void InternetExplorers::LocalAPI::createMD5File()
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

QByteArray InternetExplorers::LocalAPI::getMD5CheckSum(const QString &file)
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

bool InternetExplorers::LocalAPI::isDataCorrupted()
{
    return false;
}

bool InternetExplorers::LocalAPI::isDataAvailable()
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
