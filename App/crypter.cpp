#include "crypter.h"
#include "logger.h"
#include <QFile>
#include <QByteArray>
#include <QList>
#include <QDirIterator>
#include "libraries/simplecrypt.h"

#include "constants.h"

InternetExplorers::Crypter::Crypter()
{
    auto msg(QString("Constructor ready"));
    auto msgSender(QString("Crypter"));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);
}

InternetExplorers::Crypter::~Crypter()
{
    auto msg(QString("Destructor called"));
    auto msgSender(QString("Crypter"));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);
}

void InternetExplorers::Crypter::hashNames(std::vector<std::vector<std::string>>& data,
                                           QCryptographicHash::Algorithm algorithm)
{
    auto msg(QString("Started function hashNames()"));
    auto msgSender(QString("Crypter"));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);

    for(auto& row : data)
    {
        QCryptographicHash hash(algorithm);
        hash.addData(row[Constants::DataIndex::IndexInData::NAME].c_str(),
                row[Constants::DataIndex::IndexInData::NAME].length());
        QString result(hash.result().toHex());
        result.truncate(6);
        row[Constants::DataIndex::IndexInData::NAME]
                = result.toStdString();
    }

    msg = QString("Done function hashNames()");
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);
}

void InternetExplorers::Crypter::hashLocalNames(QCryptographicHash::Algorithm algorithm)
{
    auto msg(QString("Started hashing local data"));
    auto msgSender(QString("Crypter"));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);
    QDirIterator iterator(Constants::DATA_ROOT_NAME, QStringList() << Constants::DATA_FILE_NAME, QDir::NoFilter, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        hashLocalNamesInFile(iterator.filePath(), algorithm);
    }
    msg = QString("Done hashing local data");
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);
}

void InternetExplorers::Crypter::encryptLocalNames()
{
    SimpleCrypt crypt(4); // Chosen by fair dice roll. Guaranteed to be random

    QDirIterator iterator(Constants::DATA_ROOT_NAME, QStringList() << Constants::DATA_FILE_NAME, QDir::NoFilter, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        QFile file(iterator.filePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            auto msg(QString("No local data found"));
            auto msgSender(QString("Crypter"));
            InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::CRITICAL, msgSender);
            return;
        }

        QString result;

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            QList<QByteArray> splitRow(line.split(';'));
            splitRow[Constants::DataIndex::IndexInData::NAME]
                    = crypt.encryptToString(QString(splitRow[Constants::DataIndex::IndexInData::NAME])).toUtf8();
            result.append(splitRow.join(';'));
        }

        file.close();
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            auto msg(QString("Local file write failed"));
            auto msgSender(QString("Crypter"));
            InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::CRITICAL, msgSender);
            return;
        }

        file.write(result.toUtf8());

        file.close();

    }

}

void InternetExplorers::Crypter::decryptLocalNames()
{
    SimpleCrypt crypt(4); // Chosen by fair dice roll. Guaranteed to be random

    QDirIterator iterator(Constants::DATA_ROOT_NAME, QStringList() << Constants::DATA_FILE_NAME, QDir::NoFilter, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        QFile file(iterator.filePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            auto msg(QString("No local data found"));
            auto msgSender(QString("Crypter"));
            InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::CRITICAL, msgSender);
            return;
        }

        QString result;

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            QList<QByteArray> splitRow(line.split(';'));
            splitRow[Constants::DataIndex::IndexInData::NAME]
                    = crypt.decryptToString(QString(splitRow[Constants::DataIndex::IndexInData::NAME])).toUtf8();
            result.append(splitRow.join(';'));
        }

        file.close();
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            auto msg(QString("Local file write failed"));
            auto msgSender(QString("Crypter"));
            InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::CRITICAL, msgSender);
            return;
        }

        file.write(result.toUtf8());

        file.close();

    }
}

void InternetExplorers::Crypter::decryptNames(std::vector<std::vector<std::string> > &data)
{
    SimpleCrypt crypt(4); // Chosen by fair dice roll. Guaranteed to be random

    for(auto& row : data)
    {
        row[Constants::DataIndex::IndexInData::NAME]
                = crypt.decryptToString(QString::fromStdString(row[Constants::DataIndex::IndexInData::NAME])).toStdString();
    }
}

void InternetExplorers::Crypter::hashLocalNamesInFile(QString filename, QCryptographicHash::Algorithm algorithm)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        auto msg(QString("No local data found"));
        auto msgSender(QString("Crypter"));
        InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::CRITICAL, msgSender);
        return;
    }

    QString result;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> splitRow(line.split(';'));

        QCryptographicHash hash(algorithm);
        hash.addData(splitRow.at(Constants::DataIndex::IndexInData::NAME).toHex());
        splitRow[Constants::DataIndex::IndexInData::NAME]
                = hash.result().toHex();
        splitRow[Constants::DataIndex::IndexInData::NAME].truncate(6);
        result.append(splitRow.join(';'));
    }

    file.close();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        auto msg(QString("Local file write failed"));
        auto msgSender(QString("Crypter"));
        InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::CRITICAL, msgSender);
        return;
    }

    file.write(result.toUtf8());

    file.close();

}