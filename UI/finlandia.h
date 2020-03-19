#ifndef FINLANDIA_H
#define FINLANDIA_H

#include <QtCharts>
#include <QMainWindow>
#include <vector>
#include "datahandler.h"
#include "interfacefilter.h"
#include "constants.h"

namespace Ui {
class Finlandia;
}

class Finlandia : public QMainWindow
{
    Q_OBJECT

public:
    explicit Finlandia(InternetExplorers::DataHandler* dh,
                       QWidget *parent = 0);
    ~Finlandia();

private slots:
    void on_pushButtonNollaKaikki_clicked();

    void on_pushButtoLisaaHaku_clicked();

    void on_pushButton_clicked();

private:
    Ui::Finlandia *ui;
    std::vector<InternetExplorers::Constants::Filter::ValueFilters> previousSrc;

    InternetExplorers::DataHandler *m_DataHandler;

    std::vector<std::vector<std::vector<std::string>>> allSearches;

    QChart *m_chart;
};

#endif // FINLANDIA_H
