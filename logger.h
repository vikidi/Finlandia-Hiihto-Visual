#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <mutex>

namespace InternetExplorers
{

/*!
 * \brief Appends log info to log file
 */
class Logger
{

public:
    enum Severity {
        INFO = 0,
        WARNING,
        CRITICAL
    };

    // Copy constructors are not allowed

    /*!
     * \brief Creator not allowed
     */
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

    /*!
     * \brief Name for the log folder
     */
    inline static const QString LOG_FOLDER_NAME = "logs";

    /*!
     * \brief Name for the log file
     */
    inline static const QString LOG_FILE_NAME = "log.txt";

    /*!
     * \brief Mutex for the file
     */
    static std::mutex m_mtx;
};

}

#endif // LOGGER_H
