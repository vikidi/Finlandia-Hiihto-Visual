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
    Q_PROPERTY(bool dataDone READ dataFin NOTIFY dataisReady)

  //  Q_PROPERTY(type name READ name WRITE setName NOTIFY nameChanged)

 //   Q_PROPERTY(READ getCurrentSkier WRITE updateListIndex NOTIFY skierChanged)
   // Q_PROPERTY(bool lastOfListTrue READ isLastOfList NOTIFY lastOfListSetTrue)


public:
    explicit Finlandiamodel(QObject *parent = nullptr);
    ~Finlandiamodel();

    int progress();
    bool dataFin();

    // TODO: Get the data from DataHandler

    // TODO: Processing data


    // Means to send data to list
   // Q_INVOKABLE
    void generateListValue();
    //Q_INVOKABLE
    void setListChoice();

    void updateListIndex();
    bool isLastOfList();

    //Testing
    QString userName();
    void setUserName(const QString &userName);

private slots:
    void dataReady();
    void progressChanged(const int);


signals:
    //TEsting
    void userNameChanged();
    void dataisReady();
    void progressed();

    // Listvalues
    /*
    void skierChanged();
    void lastOfListSetTrue();
    */

private:
    //Testing
    QString m_userName;
    //Testing

    DataHandler *m_dataHandler;
    //QProgressBar *m_progress;

    int listIndex; // index of current searchlist
    bool lastOfListTrue; // if setListValue is at the last object = true

    int progress_now;

};

#endif // FINLANDIAMODEL_HH
