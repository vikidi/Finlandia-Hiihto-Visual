#include "crypter.h"
#include "logger.h"
#include <QFile>
#include <QByteArray>
#include <QList>
#include <QDirIterator>


InternetExplorers::Crypter::Crypter()
{
    auto msg(QString("Constructor ready"));
    auto msgSender(QString("Crypter"));
    InternetExplorers::Logger::getInstance().log(msg, InternetExplorers::Logger::Severity::INFO, msgSender);
}

InternetExplorers::Crypter::~Crypter()
{
    auto msg(QString("Destructor called"));
    auto msgSender(QString("Crypter"));
    InternetExplorers::Logger::getInstance().log(msg, InternetExplorers::Logger::Severity::INFO, msgSender);
}

void InternetExplorers::Crypter::hashNames(std::vector<std::vector<std::string>>& data,
                                           QCryptographicHash::Algorithm algorithm)
{
    auto msg(QString("Started function hashNames()"));
    auto msgSender(QString("Crypter"));
    InternetExplorers::Logger::getInstance().log(msg, InternetExplorers::Logger::Severity::INFO, msgSender);

    for(auto& row : data)
    {
        QCryptographicHash hash(algorithm);
        hash.addData(row[InternetExplorers::Crypter::IndexInData::NAME].c_str(),
                row[InternetExplorers::Crypter::IndexInData::NAME].length());
        row[InternetExplorers::Crypter::IndexInData::NAME]
                = QString(hash.result().chopped(6)).toStdString();
    }

    msg = QString("Done function hashNames()");
    InternetExplorers::Logger::getInstance().log(msg, InternetExplorers::Logger::Severity::INFO, msgSender);
}

void InternetExplorers::Crypter::hashLocalNames(QCryptographicHash::Algorithm algorithm)
{
    auto msg(QString("Started hashing local data"));
    auto msgSender(QString("Crypter"));
    InternetExplorers::Logger::getInstance().log(msg, InternetExplorers::Logger::Severity::INFO, msgSender);
    QDirIterator iterator(DATA_ROOT_NAME, QStringList() << DATA_FILE_NAME, QDir::NoFilter, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        hashLocalNamesInFile(iterator.filePath(), algorithm);
    }
    msg = QString("Done hashing local data");
    InternetExplorers::Logger::getInstance().log(msg, InternetExplorers::Logger::Severity::INFO, msgSender);
}

void InternetExplorers::Crypter::hashLocalNamesInFile(QString filename, QCryptographicHash::Algorithm algorithm)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        auto msg(QString("No local data found"));
        auto msgSender(QString("Crypter"));
        InternetExplorers::Logger::getInstance().log(msg, InternetExplorers::Logger::Severity::CRITICAL, msgSender);
        return;
    }

    QString result;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> splitRow(line.split(';'));

        QCryptographicHash hash(algorithm);
        hash.addData(splitRow.at(InternetExplorers::Crypter::IndexInData::NAME));
        splitRow[InternetExplorers::Crypter::IndexInData::NAME]
                = hash.result().chopped(6);
        result.append(splitRow.join(';'));
    }

    file.close();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        auto msg(QString("Local file write failed"));
        auto msgSender(QString("Crypter"));
        InternetExplorers::Logger::getInstance().log(msg, InternetExplorers::Logger::Severity::CRITICAL, msgSender);
        return;
    }

    file.write(result.toUtf8());

    file.close();

}
