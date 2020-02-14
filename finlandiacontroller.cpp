#include "finlandiacontroller.h"

FinlandiaController::FinlandiaController(Finlandiamodel *CM, QObject *parent) :
    QObject(parent),
    m_Model(CM)
{
}

void FinlandiaController::generateListValue()
{
    //m_Model->generateListValue();
}

void FinlandiaController::setListChoice()
{
    //m_Model->setListChoice();
}
