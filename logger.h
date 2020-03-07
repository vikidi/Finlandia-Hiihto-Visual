#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <mutex>

namespace InternetExplorers
{

class Logger
{

public:
    enum Severity {
        INFO = 0,
        WARNING,
        CRITICAL
    };

    // Copy constructors are not allowed
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance();
    void log(QString& msg, Severity severity);
    void log(QString& msg, Severity severity, QString& sender);

private:
    Logger();

    void write(QString& msg);
    QString getTime();
    QString getSeverity(Severity);

    inline static const QString LOG_FOLDER_NAME = "logs";
    inline static const QString LOG_FILE_NAME = "log.txt";

    static std::mutex m_mtx;
};

}

#endif // LOGGER_H
