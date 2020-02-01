#ifndef FINLANDIAAPI_H
#define FINLANDIAAPI_H

#include <QObject>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>

class FinlandiaAPI : public QObject
{
    Q_OBJECT

public:
    FinlandiaAPI();

    ///
    /// \brief Initializes the component. Sets static attributes for POST-requests.
    ///     Call before loading data.
    ///
    void Initialize();

    ///
    /// \brief Loads all data from Finlandia-hiihto archive.
    ///
    void loadAllData() const;

    ///
    /// \brief Gets all data that was loaded in "loadAllData()". Call after dataReady-signal.
    /// \return All data: < < year, distance >, < rows< row< value > > > >
    ///
    std::unordered_map<std::pair<std::string, std::string>, std::vector<std::vector<std::string>>> getAllData();

public slots:

private slots:
    ///
    /// \brief Reply is ready, response is ready to be read.
    /// TODO: Find out which reply is ready, needs params??
    ///
    void replyReady();

signals:
    ///
    /// \brief Initialization is ready.
    ///
    void initReady();

    ///
    /// \brief Loading and parsing data ready. Data is available to get.
    ///
    void dataReady();

private:
    ///
    /// \brief Performs one request, parsing and adding of data to memory.
    /// \param Year
    /// \param Distance
    ///
    void fetchData(const QString, const QString);

    std::string escapeAmp(const std::string&);
    void escapeBR(std::string&);
    void escapeSpace(std::string&);

    ///
    /// \brief Parses the data from response with QXmlStreamReader.
    /// \param Formatted response data.
    /// \return Rows of data in vector.
    ///
    std::vector<std::vector<std::string>> parseData(const std::string&);

    ///
    /// \brief Adds data concurrently to the common data structure.
    /// \param Data to add. Represents one table parsed from Finlandia-hiihto.
    ///
    void addDataToMap(const std::vector<std::vector<std::string>>&);
};

#endif // FINLANDIAAPI_H
