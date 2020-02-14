#ifndef FINLANDIACONTROLLER_H
#define FINLANDIACONTROLLER_H

#include <QObject>
#include <memory>
#include "finlandiamodel.h"

class FinlandiaController : public QObject
{
    Q_OBJECT

public:
    explicit FinlandiaController(Finlandiamodel *CM, QObject *parent = nullptr);

    Q_INVOKABLE void generateListValue();
    Q_INVOKABLE void setListChoice();

signals:

private:
    Finlandiamodel* m_Model;
};


#endif // FINLANDIACONTROLLER_H
