#ifndef LOCALDATALOADER_H
#define LOCALDATALOADER_H

#include <QObject>
#include <memory>

namespace InternetExplorers
{

/*!
 * \brief
 * Worker class that loads data from the local storage on disk.
 */
class LocalDataLoader : public QObject
{
    Q_OBJECT
public:
    LocalDataLoader();

    /*!
     * \brief
     * Loads data from disk from the given years
     * \param years
     * Years from to read data
     * \return
     * The data in format < Year, < Distance, < Rows > > >
     */
    std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > >
        loadData(std::shared_ptr<std::vector<std::string> > years);

signals:

    /*!
     * \brief
     * Signals that one file of data has been read.
     */
    void progressChanged();

private:
    const QString DATA_ROOT_NAME = "FinlandiaData";
    const QString DATA_FILE_NAME = "Data.txt";
    const QString MD5_DATA_FILE_NAME = "MD5_metadata.txt";
    const QString META_DATA_FILE_NAME = "Metadata.txt";

    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> m_data;
};

}

#endif // LOCALDATALOADER_H
