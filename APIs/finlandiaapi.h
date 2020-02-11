#ifndef FINLANDIAAPI_H
#define FINLANDIAAPI_H

#include <QObject>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <mutex>
#include <atomic>


const std::vector<std::string> TRIPS = {
    "P50",
    "V50",
    "P100",
    "P32",
    "V20",
    "V32",
    "V20jun",
    "P42",
    "V42",
    "P20",
    "P30",
    "P44",
    "P60",
    "P62",
    "P25",
    "P35",
    "P45",
    "P52",
    "P53",
    "P75",
    "V30",
    "V45",
    "V53",
    "V75"
};


class FinlandiaAPI : public QObject
{
    Q_OBJECT

public:
    FinlandiaAPI();
    ~FinlandiaAPI();

    struct Parameters {
        QString year = "";
        QString trip = "kaikki";
        QString gender = "kaikki";
        QString age = "kaikki";
        QString firstName = "";
        QString lastName = "";
        QString city = "";
        QString nationality = "0";
        QString team = "";
    };


    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> loadAllData();


signals:
    void progressChanged(const int);

private slots:

private:
    void loadInThread(std::shared_ptr<std::vector<FinlandiaAPI::Parameters>>);
    void appendData(std::vector<std::vector<std::string> >, QString, QString);

    std::mutex m_mtx;
    std::map<QString, std::map<QString, std::vector<std::vector<std::string>>>> m_data;

    int m_ready;
    std::atomic_int m_runners;

    int m_totalCalls;
    int m_currentProgress;
};

#endif // FINLANDIAAPI_H
