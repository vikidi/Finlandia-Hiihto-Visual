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

class FinlandiaCaller : public QObject
{
    Q_OBJECT

public:
    FinlandiaCaller();
    ~FinlandiaCaller();

    ///
    /// \brief Gets all data.
    /// \return All data: < < year, distance >, < rows< row< value > > > >
    ///
    std::vector<std::vector<std::string>> loadAllData(QString, QString);

    std::vector<std::vector<std::string>> loadData(std::shared_ptr<std::vector<InternetExplorers::FinlandiaAPI::Parameters>>);

signals:

private:

    bool isTooMuchData(const std::string&);

    std::string escapeAmp(const std::string&);
    void escapeBR(std::string&);
    void escapeSpace(std::string&);

    ///
    /// \brief Parses the data from response with QXmlStreamReader.
    /// \param Formatted response data.
    /// \return Rows of data in vector.
    ///
    std::vector<std::vector<std::string>> parseData(const std::string&);

    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    static std::mutex m_mtx;
};

}

#endif // FINLANDIACALLER_H
