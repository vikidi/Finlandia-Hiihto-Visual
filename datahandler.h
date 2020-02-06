#ifndef DATAHANDLER_H
#define DATAHANDLER_H

/*
 * This module is supposed to handle
 * most of the data handling and returning
 * it in right format for UI.
 */

#include <QObject>

class DataHandler : QObject
{
    Q_OBJECT

public:
    DataHandler();
    ~DataHandler();

    void Initialize();

signals:
    void progressChanged(const int&);

private:
    void loadData();
    void loadInThread();

    bool m_loadOngoing;

    std::map<QString, std::map<QString,
                std::vector<std::vector<std::string>>>> m_data;
};

#endif // DATAHANDLER_H
