#ifndef FINLANDIAAPI_H
#define FINLANDIAAPI_H

#include <QObject>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <mutex>
#include <atomic>

class FinlandiaAPI : public QObject
{
    Q_OBJECT

public:
    FinlandiaAPI();
    ~FinlandiaAPI();

    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> loadAllData();

private:
    void loadInThread(QString, QString);
    void appendData(std::vector<std::vector<std::string> >, QString, QString);

    std::mutex m_mtx;
    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> m_data;

    int ready;
    std::atomic_int m_runners;
};

#endif // FINLANDIAAPI_H
