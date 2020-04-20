#ifndef CRYPTER_H
#define CRYPTER_H

#include <vector>
#include <string>
#include <QCryptographicHash>
#include <QString>

namespace InternetExplorers {

/*!
 * \brief
 * Crypter is used to crypt local data
 * \details
 * Crypter can hash the local data on disc and in program memory<br>
 * so that it can not be decrypted back.<br>
 * It can also encrypt the data so that it can be decrypted.
 */
class Crypter
{
public:
    Crypter();
    ~Crypter();

    /*!
     * \brief
     * Converts skier names in given data to their short hash version
     * \param data
     * What to hash
     * \param algorithm
     * What hash algorithm to use
     */
    void hashNames(std::vector<std::vector<std::string>>& data,
                   QCryptographicHash::Algorithm algorithm = QCryptographicHash::Algorithm::Md5);

    /*!
     * \brief
     * Hashes one single name to its short hash version
     * \param name
     * What to hash
     * \param algorithm
     * What hash algorithm to use
     * \return
     * Short version of the hashed name
     */
    QString hashName(QString name,
                     QCryptographicHash::Algorithm algorithm = QCryptographicHash::Algorithm::Md5);

    /*!
     * \brief
     * Converts skier names in data in disk to their short hash version
     * \param algorithm
     * Algorithm what hash algorithm to use
     */
    void hashLocalNames(QCryptographicHash::Algorithm algorithm = QCryptographicHash::Algorithm::Md5);

    /*!
     * \brief
     * Converts skier names in disk to their encrypted version
     */
    void encryptLocalNames();

    /*!
     * \brief
     * Converts encrypted skier names in disk to their original version
     */
    void decryptLocalNames();

    /*!
     * \brief
     * Converts encrypted skier names to their original version
     * \param data
     * What to decrypt
     */
    void decryptNames(std::vector<std::vector<std::string>>& data);

private:

    void hashLocalNamesInFile(QString filename, QCryptographicHash::Algorithm algorithm);
};

}

#endif // CRYPTER_H
