#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <mutex>

namespace InternetExplorers
{

/*!
 * \brief
 * Singleton class that handles the logging to a text file.
 * \details
 * Class provides a thread safe way of<br>
 * logging the ongoing events in program.<br>
 * It provides logging messages with following informations:<br>
 * timestamp, message, severity and sender.<br>
 * The logfile created will be saved in own folder<br>
 * in the programs execution folder.<br>
 * The file will not have backup.<br>
 * The created file will be overwritten each time the<br>
 * program is started.
 */
class Logger
{

public:
    /*!
     * \brief
     * Severity levels for logged messages.
     */
    enum Severity {
        INFO = 0,   /*!< Basic info logging */
        WARNING,    /*!< Unwanted behavior, possibly dangerous */
        CRITICAL    /*!< Critical error in program */
    };

    /*!
     * \brief
     * Copy constructor not allowed
     */
    Logger(const Logger&) = delete;

    /*!
     * \brief
     * Copy constructor not allowed
     */
    Logger& operator=(const Logger&) = delete;

    /*!
     * \brief
     * Get the singleton instance of the logger.
     * \details
     * The singleton instance is created only the first<br>
     * time calling this method.<br>
     * With later calls the same instance is being returned.
     * \return
     * The logger instance
     */
    static Logger& getInstance();

    /*!
     * \brief
     * Creates new log to the log file.
     * \details
     * Creates the string from given parameters.<br>
     * Adds timestamp to the log.<br>
     * This method is thread safe.
     * \param
     * Message to be written
     * \param
     * Severity of the message
     */
    void log(QString& msg, Severity severity);

    /*!
     * \brief
     * Creates new log to the log file.
     * \details
     * Creates the string from given parameters.<br>
     * Adds timestamp to the log.<br>
     * This method is thread safe.
     * \param
     * Message to be written
     * \param
     * Severity of the message
     * \param
     * Sender of the message
     */
    void log(QString& msg, Severity severity, QString& sender);

private:
    /*!
     * \brief
     * Constructor called first time an instace is called.
     * Clears or creates the log file.
     */
    Logger();

    /*!
     * \brief
     * Writes the message to the log files end.
     * \param
     * Message that contains everything for the row.
     */
    void write(QString& msg);

    /*!
     * \brief
     * Provides the current time
     * \return
     * Current time in format dd:mm:yyyy h:mm:ss.z
     */
    QString getTime();

    /*!
     * \brief
     * Handles the severity parameter
     * \param
     * The severity as enum value
     * \return
     * Severity as string
     */
    QString getSeverity(Severity severity);

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
