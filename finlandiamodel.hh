#ifndef FINLANDIAMODEL_HH
#define FINLANDIAMODEL_HH

#include "datahandler.h"
#include <QProgressBar>
#include <QObject>


class Finlandiamodel : public QObject
{
    Q_OBJECT
    //Testing
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(int progress READ progress NOTIFY progressed)

public:
    explicit Finlandiamodel(QObject *parent = nullptr);
    ~Finlandiamodel();

    int progress();

    //Testing
    QString userName();
    void setUserName(const QString &userName);

private slots:
    void dataReady();
    void progressChanged(const int);

    //void on_haunAloitusNappi_clicked();

signals:
    //TEsting
    void userNameChanged();

    void progressed();

private:
    //Testing
    QString m_userName;
    //Testing

    DataHandler *m_dataHandler;
    //QProgressBar *m_progress;

    int progress_now;

};

#endif // FINLANDIAMODEL_HH
