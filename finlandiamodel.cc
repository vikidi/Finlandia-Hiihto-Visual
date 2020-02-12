#include "finlandiamodel.hh"
#include <QProgressBar>
#include <QObject>
#include <QDebug>

Finlandiamodel::Finlandiamodel(QObject *parent) :
    QObject(parent),
    m_dataHandler(new DataHandler)
    //m_progress(new QProgressBar(nullptr))

{

    QObject::connect(m_dataHandler, &DataHandler::loadingReady, this, &Finlandiamodel::dataReady);
    m_dataHandler->Initialize();

    // For progress
    QObject::connect(m_dataHandler, &DataHandler::progressChanged, this, &Finlandiamodel::progressChanged);

    // Remove toolbar
    //QList<QToolBar *> allToolBars = this->findChildren<QToolBar *>();
    //foreach(QToolBar *tb, allToolBars) {
    //    this->removeToolBar(tb);
    //}

    //ui->haunAloitusNappi->setDisabled(true);
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

void Finlandiamodel::dataReady()
{
    std::map<QString, int> test = m_dataHandler->amountOfSkiers();
    //ui->haunAloitusNappi->setDisabled(false);
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
