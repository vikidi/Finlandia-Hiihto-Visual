#include "logger.h"

#include <QDir>
#include <fstream>
#include <QDateTime>

// Declaration of static member
std::mutex InternetExplorers::Logger::m_mtx;

InternetExplorers::Logger::Logger()
{
    // Create root folder if not exists
    if(!QDir(InternetExplorers::Constants::LOG_FOLDER_NAME).exists()) {
        QDir().mkdir(InternetExplorers::Constants::LOG_FOLDER_NAME);
    }

    std::string path = InternetExplorers::Constants::LOG_FOLDER_NAME.toStdString() + "/"
            + InternetExplorers::Constants::LOG_FILE_NAME.toStdString();
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

    QFile f(InternetExplorers::Constants::LOG_FOLDER_NAME + "/" + InternetExplorers::Constants::LOG_FILE_NAME);
    if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QString msg(getTime() + "Logger initialized\n");
        f.write(msg.toStdString().c_str());
        f.close();
    }

    m_writeBuffer.reserve(InternetExplorers::Constants::Logger::WRITE_BUFFER_MAX_SIZE);

}

InternetExplorers::Logger::~Logger()
{
    if(getInstance().m_writeBuffer.size() != 0)
    {
        getInstance().writeBuffer();
    }
    QFile f(InternetExplorers::Constants::LOG_FOLDER_NAME + "/" + InternetExplorers::Constants::LOG_FILE_NAME);
    if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QString msg(getInstance().getTime() + "Logger closing\n");
        f.write(msg.toStdString().c_str());
        f.close();
    }
}

void InternetExplorers::Logger::writeBuffer()
{
    QFile f(InternetExplorers::Constants::LOG_FOLDER_NAME + "/" + InternetExplorers::Constants::LOG_FILE_NAME);
    if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
        for(QString& msg : m_writeBuffer)
        {
            f.write(msg.toStdString().c_str());
        }
        f.close();
    }
    getInstance().m_writeBuffer.clear();
}

QString InternetExplorers::Logger::getTime() const
{
    QDateTime now(QDateTime::currentDateTime());
    return now.toString("dd.MM.yyyy") + " " + now.toString("hh:mm:ss.zzz") + "\t- ";
}

QString InternetExplorers::Logger::getSeverity(const Constants::Logger::Severity severity) const
{
    switch (severity) {
        case InternetExplorers::Constants::Logger::Severity::INFO:
            return "INFO";
        case InternetExplorers::Constants::Logger::Severity::WARNING:
            return "WARNING";
        case InternetExplorers::Constants::Logger::Severity::CRITICAL:
            return "CRITICAL";
        default:
            Q_UNREACHABLE();
            return "";
    }
}

InternetExplorers::Logger &InternetExplorers::Logger::getInstance()
{
    static Logger loggerInstance;
    return loggerInstance;
}

void InternetExplorers::Logger::log(const QString &msg, const Constants::Logger::Severity severity)
{
    QString str = getInstance().getTime();
    str += getInstance().getSeverity(severity);
    str += ": " + msg + "\n";
    std::lock_guard<std::mutex> lock(m_mtx);
    getInstance().m_writeBuffer.push_back(str);
    if(Q_UNLIKELY((getInstance().m_writeBuffer.size() == InternetExplorers::Constants::Logger::WRITE_BUFFER_MAX_SIZE)
        || (severity == InternetExplorers::Constants::Logger::Severity::CRITICAL)))
    { // Buffer is full or message severity was critical
        getInstance().writeBuffer();
    }
}

void InternetExplorers::Logger::log(const QString &msg, InternetExplorers::Constants::Logger::Severity severity, const QString &sender)
{
    QString str = getInstance().getTime();
    str += sender + " - ";
    str += getInstance().getSeverity(severity);
    str += ": " + msg + "\n";
    std::lock_guard<std::mutex> lock(m_mtx);
    getInstance().m_writeBuffer.push_back(str);
    if(Q_UNLIKELY((getInstance().m_writeBuffer.size() == InternetExplorers::Constants::Logger::WRITE_BUFFER_MAX_SIZE)
        || (severity == InternetExplorers::Constants::Logger::Severity::CRITICAL)))
    { // Buffer is full or message severity was critical
        getInstance().writeBuffer();
    }
}
