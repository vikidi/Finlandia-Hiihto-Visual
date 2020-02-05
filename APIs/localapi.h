#ifndef LOCALAPI_H
#define LOCALAPI_H

#include <QObject>
#include <map>
#include <QString>
#include <iostream>
#include <vector>

class LocalAPI : QObject
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

private:
    const QString DATA_ROOT_NAME = "FinlandiaData";
    const QString DATA_FILE_NAME = "Data.txt";

    ///
    /// \brief Checks if data is corrupted.
    ///
    bool isDataCorrupted();

    ///
    /// \brief Checks if there is data to be read.
    ///
    bool isDataAvailable();
};

#endif // LOCALAPI_H
