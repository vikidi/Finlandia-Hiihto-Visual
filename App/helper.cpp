#include "helper.h"

#include <QTime>

long InternetExplorers::Helper::timeToMSecs(const QString time)
{
    long t;
    if (time.contains('.')) {
        t = static_cast<long>(QTime(0, 0, 0).msecsTo(QTime::fromString(time, "h:mm:ss.z")));
    } else {
        t = static_cast<long>(QTime(0, 0, 0).msecsTo(QTime::fromString(time, "h:mm:ss")));
    }

    if (t == 0) return -1; // Invalid time given

    return t;
}

long InternetExplorers::Helper::timeToMSecs(const std::string time)
{
    QString qtime = QString::fromStdString(time);

    long t;
    if (time.find('.') != time.npos) {
        t = static_cast<long>(QTime(0, 0, 0).msecsTo(QTime::fromString(qtime, "h:mm:ss.z")));
    } else {
        t = static_cast<long>(QTime(0, 0, 0).msecsTo(QTime::fromString(qtime, "h:mm:ss")));
    }

    if (t == 0) return -1; // Invalid time given

    return t;
}

QString InternetExplorers::Helper::mSecsToQString(const unsigned long long time)
{
    QString t = QDateTime::fromMSecsSinceEpoch(static_cast<long long>(time), Qt::UTC).toString("h:mm:ss.zzz");

    std::string stdTime{t.toStdString()}; // Chop result to 0.1s
    return QString::fromStdString(stdTime.substr(0, stdTime.size() - 2));
}

std::string InternetExplorers::Helper::mSecsToString(const unsigned long long time)
{
    QString t = QDateTime::fromMSecsSinceEpoch(static_cast<long long>(time), Qt::UTC).toString("h:mm:ss.zzz");

    std::string stdTime{t.toStdString()}; // Chop result to 0.1s
    return stdTime.substr(0, stdTime.size() - 2);
}
