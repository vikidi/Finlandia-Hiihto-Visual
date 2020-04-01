#ifndef FINLANDIACALLER_H
#define FINLANDIACALLER_H

#include <QObject>
#include <memory>
#include <unordered_map>
#include <map>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <mutex>
#include "finlandiaapi.h"

namespace InternetExplorers
{

/*!
 * \brief
 * Worker class that handles data loading and screen scraping from
 * finlandia hiihto website.
 */
class FinlandiaCaller : public QObject
{
    Q_OBJECT

public:
    FinlandiaCaller();
    ~FinlandiaCaller();

    /*!
     * \brief
     * Loads data from finlandia hiihto website with given parameters.
     * \param parameters
     * Parameters to use in the data loading
     * \return
     * The loaded data in rows
     */
    std::vector<std::vector<std::string>> loadData(std::shared_ptr<std::vector<InternetExplorers::FinlandiaAPI::Parameters>> parameters);

signals:

private:

    bool isTooMuchData(const std::string&);

    std::string escapeAmp(const std::string&);
    void escapeBR(std::string&);
    void escapeSpace(std::string&);

    /*!
     * \brief
     * Parses the data from response with QXmlStreamReader.
     * \param data
     * Formatted response data.
     * \return
     * Rows of data in vector.
     */
    std::vector<std::vector<std::string>> parseData(const std::string& data);

    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    static std::mutex m_mtx;
};

}

#endif // FINLANDIACALLER_H
