#ifndef LOCALAPI_H
#define LOCALAPI_H

#include <QObject>
#include <map>
#include <QString>
#include <iostream>
#include <vector>
#include <mutex>
#include <atomic>

namespace InternetExplorers
{

class LocalAPI : public QObject
{
    Q_OBJECT

public:
    LocalAPI();
    ~LocalAPI();

    ///
    /// \brief Overrides and saves data in folder/txt structure in working directory.
    ///
    void saveData(const std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>>&);

    ///
    /// \brief Loads data from the folder/txt structure.
    /// \return
    ///
    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> loadData();

    ///
    /// \brief Checks if the data in folder structure has been corrupted or if there is not any data.
    ///
    bool needsToBeLoadedFromWeb();

    ///
    /// \brief
    ///     Reads the general metadata file
    /// \return
    ///     < Key/Data identifier, Value >
    ///
    std::map<QString, QString> readMetaDataFile();

signals:
    void progressChanged(const int);

public slots:
    void updateProgress();

private:
    const QString DATA_ROOT_NAME = "FinlandiaData";
    const QString DATA_FILE_NAME = "Data.txt";
    const QString MD5_DATA_FILE_NAME = "MD5_metadata.txt";
    const QString META_DATA_FILE_NAME = "Metadata.txt";

    int getAmountOfFiles();

    void loadDataInThread(std::shared_ptr<std::vector<std::string>>);

    void appendData(std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>>& data);

    template<typename T>
    std::vector<std::shared_ptr<std::vector<T>>> SplitVector(const std::vector<T>& vec, size_t n);

    ///
    /// \brief
    ///     Reads the MD5 checksum metadata file
    /// \return
    ///     Results< Path, MD5 checksum >
    ///
    std::vector<std::pair<QString, QString>> readMD5File();

    ///
    /// \brief
    ///     Creates general metadata file for the data load
    ///
    void createGeneralMetaDataFile();

    ///
    /// \brief
    ///     Creates file having all the MD5 check sum metadatas
    ///
    void createMD5File();

    ///
    /// \brief
    ///     Gets the MD5 checksum for the given file
    /// \param file
    ///     Path to the file
    /// \return
    ///     MD5 check sum for the file, empty QByteArray if failure
    ///
    QByteArray getMD5CheckSum(const QString &file);

    ///
    /// \brief Checks if data is corrupted.
    ///
    bool isDataCorrupted();

    ///
    /// \brief Checks if there even is data to be read.
    ///
    bool isDataAvailable();

    int m_fileCount;
    int m_currentProgress;
    int m_maxProgress; // aka amount of files

    std::mutex m_mtx;
    std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > m_data;
};

}

#endif // LOCALAPI_H
