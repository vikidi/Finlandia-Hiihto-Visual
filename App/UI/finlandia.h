#ifndef FINLANDIA_H
#define FINLANDIA_H

#include <QtCharts>
#include <QMainWindow>
#include <vector>
#include "constants.h"
#include "datahandler.h"
#include "interfacefilter.h"


// Alias to make things easier
#ifndef FILTER_NS
#define FILTER_NS

using Filter_NS = InternetExplorers::Constants::Filter::ValueFilters;
#endif

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

    std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> makefilter();

    void make_listview();



    void make_listviweLabel();

    /**
     * @brief select_attributes
     * @return vector on integers pointin out what
     * attributes the listView should show
     */
    std::vector<int> select_attributes();

private slots:
    void on_pushButtonNollaKaikki_clicked();

    void on_pushButtoLisaaHaku_clicked();

    void on_pushButton_clicked();

    void make_chart();

    void remove_cart();

    void save_chart();

private:
    Ui::Finlandia *ui;
    std::vector<Filter_NS> previousSrc;

    InternetExplorers::DataHandler *m_DataHandler;

    std::vector<std::vector<std::vector<std::string>>> allSearches;

    QChart *m_chart;
    QString curr_series_title;
    std::vector<std::string> attribute_enum = {"YEAR", "DISTANCE", "TIME",
                                               "PLACE", "PLACE_MEN",
                                               "PLACE_WOMEN", "SEX", "NAME",
                                               "CITY", "NATIONALITY",
                                               "BIRTH_YEAR", "TEAM"};
};

#endif // FINLANDIA_H
