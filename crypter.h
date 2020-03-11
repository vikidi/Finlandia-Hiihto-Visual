#ifndef CRYPTER_H
#define CRYPTER_H

#include <vector>
#include <string>
#include <QCryptographicHash>

namespace InternetExplorers {

class Crypter
{
public:
    Crypter();
    /*!
     * \brief hashNames converts skier names in given data to their short hash version
     * \param data what to hash
     * \param algorithm what hash algorithm to use
     */
    void hashNames(std::vector<std::vector<std::string>>& data,
                   QCryptographicHash::Algorithm algorithm = QCryptographicHash::Algorithm::Md5);
};

}

#endif // CRYPTER_H
