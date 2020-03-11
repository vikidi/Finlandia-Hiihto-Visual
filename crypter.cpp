#include "crypter.h"
#include "interfacefilter.h"
#include <QString>

InternetExplorers::Crypter::Crypter()
{

}

void InternetExplorers::Crypter::hashNames(std::vector<std::vector<std::string>>& data,
                                           QCryptographicHash::Algorithm algorithm)
{
    for(auto& row : data)
    {
        QCryptographicHash hash(algorithm);
        hash.addData(row[InternetExplorers::InterfaceFilter::ValueFilters::NAME].c_str(),
                row[InternetExplorers::InterfaceFilter::ValueFilters::NAME].length());
        row[InternetExplorers::InterfaceFilter::ValueFilters::NAME]
                = QString(hash.result()).toStdString().substr(6);
    }
}
