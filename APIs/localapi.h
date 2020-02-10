#ifndef LOCALAPI_H
#define LOCALAPI_H

#include <QObject>
#include <map>
#include <QString>
#include <iostream>
#include <vector>

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
    ///     Creates file having all the MD5 check sum metadatas
    ///
    void createMD5File();

private:
    const QString DATA_ROOT_NAME = "FinlandiaData";
    const QString DATA_FILE_NAME = "Data.txt";
    const QString MD5_DATA_FILE_NAME = "MD5_metadata.txt";
    const QString META_DATA_FILE_NAME = "Metadata.txt";

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
};

#endif // LOCALAPI_H
