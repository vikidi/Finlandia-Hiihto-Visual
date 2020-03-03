#ifndef LOCALDATALOADER_H
#define LOCALDATALOADER_H

#include <QObject>

namespace InternetExplorers
{

class LocalDataLoader : public QObject
{
    Q_OBJECT
public:
    LocalDataLoader();
    std::map<QString, std::map<QString, std::vector<std::vector<std::string> > > > loadData(std::shared_ptr<std::vector<std::string> > years);

private:
    const QString DATA_ROOT_NAME = "FinlandiaData";
    const QString DATA_FILE_NAME = "Data.txt";
    const QString MD5_DATA_FILE_NAME = "MD5_metadata.txt";
    const QString META_DATA_FILE_NAME = "Metadata.txt";

    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> m_data;
};

}

#endif // LOCALDATALOADER_H
