#include "finlandiamodel.h"
#include <QProgressBar>
#include <QObject>
#include <QDebug>

Finlandiamodel::Finlandiamodel(QObject *parent) :
    QObject(parent),
    m_dataHandler(new DataHandler),
    listIndex(0),
    lastOfListTrue(false)

{

    QObject::connect(m_dataHandler, &DataHandler::loadingReady, this, &Finlandiamodel::dataReady);
    m_dataHandler->Initialize();

    // For progress
    QObject::connect(m_dataHandler, &DataHandler::progressChanged, this, &Finlandiamodel::progressChanged);

}

Finlandiamodel::~Finlandiamodel()
{
    delete m_dataHandler;
    //delete m_progress;
}

int Finlandiamodel::progress()
{
    return progress_now;
}

bool Finlandiamodel::dataFin()
{
    if (progress_now == 100){
        qDebug() << "datadone";
        return true;
    }
    else{
        return false;
    }
}

void Finlandiamodel::updateListIndex()
{
    if(!isLastOfList()){

    } else {
        listIndex = 0;
        lastOfListTrue = false;
    }
}


bool Finlandiamodel::isLastOfList()
{
    return lastOfListTrue;
}


void Finlandiamodel::dataReady()
{
    //std::map<QString, int> test = m_dataHandler->amountOfSkiers();

    progress_now = 100;
    emit progressed();
    emit dataisReady();
}

void Finlandiamodel::progressChanged(const int progress)
{
    progress_now = progress;
    emit progressed();
}


//TESTING
QString Finlandiamodel::userName()
{
    return m_userName;
}

void Finlandiamodel::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged();

    qDebug() << "asdasd";
}

