#ifndef FINLANDIACALLER_H
#define FINLANDIACALLER_H

#include <QObject>
#include <unordered_map>
#include <map>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <mutex>

class FinlandiaCaller : QObject
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

    std::mutex m_mtx;
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
};

#endif // FINLANDIACALLER_H
