#ifndef LOCALAPI_H
#define LOCALAPI_H

#include <QObject>
#include <map>
#include <QString>
#include <iostream>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>

namespace InternetExplorers
{

/*!
 * \brief
 * Provides a public interface to the local
 * data on disk.
 */
class LocalAPI : public QObject
{
    Q_OBJECT

public:
    LocalAPI();
    ~LocalAPI();

    /*!
     * \brief
     * Overrides and saves data in folder/txt structure in working directory.
     * \param data
     * Data to be saved locally in format < Year, < Distance, < Rows > > >
     */
    void saveData(const std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>>& data);

    /*!
     * \brief
     * Loads data from the folder/txt structure.
     * \return
     * The data in format < Year, < Distance, < Rows > > >
     */
    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> loadData();

    /*!
     * \brief
     * Checks if the data in folder structure has been corrupted or if there is not any data.
     * \return
     * True, if the data needs to be loaded from web
     */
    bool needsToBeLoadedFromWeb() const;

    /*!
     * \brief
     * Reads the general metadata file
     * \return
     * < Key/Data identifier, Value >
     */
    std::map<QString, QString> readMetaDataFile();

    /*!
     * \brief
     * Creates file having all the MD5 check sum metadatas
     */
    void createMD5File();

signals:

    /*!
     * \brief
     * Signals that the progress in loading has changed
     * \param progress
     * Current progress
     */
    void progressChanged(const int progress) const;

    /*!
     * \brief
     * Append text to main window info box
     * \param text
     * Text to be appended
     */
    void appendInfo(const QString text) const;

public slots:

    /*!
     * \brief
     * Updates the current progress
     */
    void updateProgress();

private:

    const QString m_name = "LocalAPI";

    /*!
     * \brief
     * Gets the amount of data files on disk
     * \return
     * The amount of files
     */
    int getAmountOfFiles() const;

    /*!
     * \brief
     * Loads data from disk in thread from given years.
     * \param years
     * Years from to read the data.
     */
    void loadDataInThread(std::shared_ptr<std::vector<std::string>> years);

    /*!
     * \brief
     * Appends data to the common data structure
     * \param data
     * Data to append
     * \attention
     * Method is thread safe.
     */
    void appendData(const std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>>& data);

    template<typename T> // Template for the method below
    /*!
     * \brief
     * Splits a vector to multiple same size vectors
     * \param vec
     * The original vector to spilt
     * \param n
     * Amount of new vectors after splitting
     * \return
     * Splitted vectors in vector
     */
    std::vector<std::shared_ptr<std::vector<T>>> SplitVector(const std::vector<T>& vec, size_t n);

    /*!
     * \brief
     * Reads the MD5 checksum metadata file
     * \return
     * Results < Path, MD5 checksum >
     */
    std::map<QString, QString> readMD5File() const;

    /*!
     * \brief
     * Creates general metadata file for the data load
     */
    void createGeneralMetaDataFile();

    /*!
     * \brief
     * Gets the MD5 checksum for the given file
     * \param file
     * Path to the file
     * \return
     * MD5 check sum for the file, empty QByteArray if failure
     */
    QByteArray getMD5CheckSum(const QString &file) const;

    /*!
     * \brief
     * Checks if data is corrupted.
     * \return
     * True, if data on disk is corrupted.
     */
    bool isDataCorrupted() const;

    /*!
     * \brief
     * Checks if there even is data to be read.
     * \return
     * True, if there is data
     */
    bool isDataAvailable() const;

    /*!
     * \brief
     * Counts the amount of rows in data
     * \return
     * Amount of rows
     */
    int getAmountOfRows() const;

    int m_fileCount;
    int m_currentProgress;
    int m_maxProgress; // aka amount of files

    std::mutex m_mtx;
    std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > m_data;
};

}

#endif // LOCALAPI_H
