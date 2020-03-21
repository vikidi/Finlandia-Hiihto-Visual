#ifndef FINLANDIA_H
#define FINLANDIA_H

#include <QtCharts>
#include <QMainWindow>
#include <vector>
#include "datahandler.h"
#include "interfacefilter.h"

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

    std::map<InternetExplorers::InterfaceFilter::ValueFilters, QString> makefilter();

    void make_listview();

    void make_chart();

    void make_listviweLabel();

private slots:
    void on_pushButtonNollaKaikki_clicked();

    void on_pushButtoLisaaHaku_clicked();

    void on_pushButton_clicked();

private:
    Ui::Finlandia *ui;
    std::vector<InternetExplorers::InterfaceFilter::ValueFilters> previousSrc;

    InternetExplorers::DataHandler *m_DataHandler;

    std::vector<std::vector<std::vector<std::string>>> allSearches;

    QChart *m_chart;
    QString curr_series_title;
};

#endif // FINLANDIA_H
