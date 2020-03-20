#include "logger.h"

#include <QDir>
#include <fstream>
#include <QDateTime>

// Declaration of static member
std::mutex InternetExplorers::Logger::m_mtx;

InternetExplorers::Logger::Logger()
{
    // Create root folder if not exists
    if(!QDir(Constants::LOG_FOLDER_NAME).exists()) {
        QDir().mkdir(Constants::LOG_FOLDER_NAME);
    }

    std::string path = Constants::LOG_FOLDER_NAME.toStdString() + "/" + Constants::LOG_FILE_NAME.toStdString();
    if (QFile::exists(QString::fromStdString(path))) {
        // Empty the file
        QFile file(QString::fromStdString(path));
        file.open(QFile::WriteOnly|QFile::Truncate);
        file.close();
    } else {
        // Create the file
        std::ofstream file;
        file.open(path);
        file.close();
    }
}

void InternetExplorers::Logger::write(QString &msg)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    QFile f(Constants::LOG_FOLDER_NAME + "/" + Constants::LOG_FILE_NAME);
    if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
        f.write(msg.toStdString().c_str());
    }
}

QString InternetExplorers::Logger::getTime()
{
    QDateTime now = QDateTime::currentDateTime();
    return now.toString("dd.MM.yyyy") + " " + now.toString("hh:mm:ss.zzz") + "\t- ";
}

QString InternetExplorers::Logger::getSeverity(Constants::Logger::Severity severity)
{
    switch (severity) {
        case Constants::Logger::Severity::INFO:
            return "INFO";
        case Constants::Logger::Severity::WARNING:
            return "WARNING";
        case Constants::Logger::Severity::CRITICAL:
            return "WARNING";
    }

    return "";
}

InternetExplorers::Logger &InternetExplorers::Logger::getInstance()
{
    static Logger loggerInstance;
    return loggerInstance;
}

void InternetExplorers::Logger::log(QString &msg, Constants::Logger::Severity severity)
{
    QString str = getInstance().getTime();
    str += getInstance().getSeverity(severity);
    str += ": " + msg + "\n";
    getInstance().write(str);
}

void InternetExplorers::Logger::log(QString &msg, Constants::Logger::Severity severity, QString &sender)
{
    QString str = getInstance().getTime();
    str += sender + " - ";
    str += getInstance().getSeverity(severity);
    str += ": " + msg + "\n";
    getInstance().write(str);
}