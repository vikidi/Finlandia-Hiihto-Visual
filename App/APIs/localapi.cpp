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
#include "logger.h"

InternetExplorers::LocalAPI::LocalAPI() :
    m_fileCount(0),
    m_currentProgress(0),
    m_maxProgress(getAmountOfFiles()),
    m_data({})
{
    auto msg(QString("Constructor ready"));
    auto msgSender(QString("LocalAPI"));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);
}

InternetExplorers::LocalAPI::~LocalAPI()
{
    auto msg(QString("Destructor called"));
    auto msgSender(QString("LocalAPI"));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);
}

void InternetExplorers::LocalAPI::saveData(const std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > &data)
{
    // If old data is there, delete it
    if(QDir(Constants::DATA_ROOT_NAME).exists()) {
        qDebug() << "Poistetaan kansioita";
        QDir(Constants::DATA_ROOT_NAME).removeRecursively();
    }

    // Create the root folder
    QDir().mkdir(Constants::DATA_ROOT_NAME);


    // Go through years
    for(auto& year : data) {

        // Create the year folder
        QString yfolder = Constants::DATA_ROOT_NAME + QString("/") + year.first;
        QDir().mkdir(yfolder);

        //Go through distances
        for(auto& distance : year.second) {

            // Create the distance folder
            QString dfolder = yfolder + QString("/") + distance.first;
            QDir().mkdir(dfolder);

            // Create file with the rows
            QFile file(dfolder + QString("/") + Constants::DATA_FILE_NAME);
            if ( Q_LIKELY(file.open(QIODevice::WriteOnly | QIODevice::Text)) )
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
    if(!QDir(Constants::DATA_ROOT_NAME).exists()) {
        qDebug() << "Data-folder was not found";
        return {};
    }

    size_t optimalAmountOfThreads(std::thread::hardware_concurrency());

    // Create year vectors
    auto years = std::vector<std::string>();
    for (int i = 1974; i < 2020; ++i) {
        years.emplace_back(std::to_string(i));
    }
    std::vector<std::shared_ptr<std::vector<std::string>>> yearVectors = SplitVector(years, optimalAmountOfThreads);

    std::vector<std::thread> threads;
    threads.reserve(optimalAmountOfThreads);
    for(size_t i = 0; i < optimalAmountOfThreads; ++i)
    {
        threads.push_back(std::thread(&LocalAPI::loadDataInThread, this, yearVectors[i]));
    }

    for (auto& th : threads) {
        th.join();
    }

    std::cout << getAmountOfRows() << " rows fetched!" << std::endl;

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
    ++m_fileCount;

    int progress = static_cast<int>(100 * (m_fileCount/static_cast<double>(m_maxProgress)));

    if (progress > m_currentProgress) {
        m_currentProgress = progress;
        emit progressChanged(progress);
    }
}

int InternetExplorers::LocalAPI::getAmountOfFiles()
{
    QDirIterator it(Constants::DATA_ROOT_NAME, QStringList() << Constants::DATA_FILE_NAME, QDir::NoFilter, QDirIterator::Subdirectories);
    int count = 0;
    while (it.hasNext()){
        it.next();
        ++count;
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

std::map<QString, QString> InternetExplorers::LocalAPI::readMD5File()
{
    std::map<QString, QString> sums = {};

    QFile file(Constants::MD5_DATA_FILE_NAME);
    if (Q_UNLIKELY(!file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        return {};
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        QStringList parts = line.split(";");

        sums.insert({parts[0], parts[1]});
    }

    return sums;
}

void InternetExplorers::LocalAPI::createGeneralMetaDataFile()
{

}

void InternetExplorers::LocalAPI::createMD5File()
{
    // Check if MD5 metadata file already exists
    if (QFile::exists(Constants::MD5_DATA_FILE_NAME)) {

        // Delete it if exists
        QFile f(Constants::MD5_DATA_FILE_NAME);
        f.remove();
    }

    // Create file with the rows
    QFile file(Constants::MD5_DATA_FILE_NAME);
    if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream stream( &file );
        stream.setCodec("UTF-8");
        stream.setGenerateByteOrderMark(true);

        // Go throug all files
        QDirIterator it(Constants::DATA_ROOT_NAME, QStringList() << Constants::DATA_FILE_NAME, QDir::NoFilter, QDirIterator::Subdirectories);
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
    if (Q_LIKELY(f.open(QFile::ReadOnly))) {
        QCryptographicHash hash(QCryptographicHash::Algorithm::Md5);
        if (Q_LIKELY(hash.addData(&f))) {
            return hash.result();
        }
    }
    return QByteArray();
}

bool InternetExplorers::LocalAPI::isDataCorrupted()
{
    // Check that MD5 metadata file exists
    if (!QFile::exists(Constants::MD5_DATA_FILE_NAME)) {
        return true;
    }

    // Check that it has as many rows as there is files
    // in FinlandiaData
    int amount = 0;
    QFile file(Constants::MD5_DATA_FILE_NAME);
    if (Q_UNLIKELY(!file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        return true;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (Q_LIKELY(line.trimmed() != "")) {
            ++amount;
        }
    }

    if (amount != m_maxProgress) {
        return true;
    }

    // Check that the sums match
    std::map<QString, QString> sums = readMD5File();

    // Go throug all files and check that the hash is same
    QDirIterator it(Constants::DATA_ROOT_NAME, QStringList() << Constants::DATA_FILE_NAME, QDir::NoFilter, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFile f(it.next());

        QString name = f.fileName();

        QByteArray checkSum = getMD5CheckSum(name);

        QString DataAsString = QString(checkSum.toHex());

        if (sums.at(name) != DataAsString) {
            return true;
        }
    }

    // Data is not corrupted
    return false;
}

bool InternetExplorers::LocalAPI::isDataAvailable()
{
    if(!QDir(Constants::DATA_ROOT_NAME).exists()) {
        return false;
    }

    // Iterate year folders
    QDirIterator it(Constants::DATA_ROOT_NAME, QDirIterator::NoIteratorFlags);
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
            if (!QFile::exists(it2.filePath() + QString("/") + Constants::DATA_FILE_NAME)) {
                return false;
            }
        }
    }

    return true;
}

int InternetExplorers::LocalAPI::getAmountOfRows() const
{
    if (m_data.size() == 0) return 0;

    int count = 0;
    for (auto& year : m_data) {
        for (auto& dist : year.second) {
            count += static_cast<int>(dist.second.size());
        }
    }

    return count;
}
