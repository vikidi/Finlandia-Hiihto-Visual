#ifndef CRYPTER_H
#define CRYPTER_H

#include <vector>
#include <string>
#include <QCryptographicHash>
#include <QString>

namespace InternetExplorers {

class Crypter
{
public:
    Crypter();
    ~Crypter();
    /*!
     * \brief hashNames converts skier names in given data to their short hash version
     * \param data what to hash
     * \param algorithm what hash algorithm to use
     */
    void hashNames(std::vector<std::vector<std::string>>& data,
                   QCryptographicHash::Algorithm algorithm = QCryptographicHash::Algorithm::Md5);
    /*!
     * \brief hashLocalNames converts skier names in data in disk to their short hash version
     * \param algorithm what hash algorithm to use
     */
    void hashLocalNames(QCryptographicHash::Algorithm algorithm = QCryptographicHash::Algorithm::Md5);

private:
    enum IndexInData {
        YEAR = 0,
        DISTANCE,
        TIME,
        PLACE,
        PLACE_MEN,
        PLACE_WOMEN,
        SEX,
        NAME,
        CITY,
        NATIONALITY,
        BIRTH_YEAR,
        TEAM
    };
    const QString DATA_ROOT_NAME = "FinlandiaData";
    const QString DATA_FILE_NAME = "Data.txt";
    const QString MD5_DATA_FILE_NAME = "MD5_metadata.txt";
    const QString META_DATA_FILE_NAME = "Metadata.txt";

    void hashLocalNamesInFile(QString filename, QCryptographicHash::Algorithm algorithm);
};

}

#endif // CRYPTER_H
