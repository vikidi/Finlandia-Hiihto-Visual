#ifndef FINLANDIA_H
#define FINLANDIA_H

#include <QtCharts>
#include <QMainWindow>
#include <QMenu>
#include <vector>
#include <memory>
#include "constants.h"
#include "datahandler.h"
#include "interfacefilter.h"
#include "encryptionsettingswindow.h"
#include "predicterwindow.h"

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
    /**
     * @brief Finlandia
     * @param datahandler
     * @param parent
     */
    explicit Finlandia(InternetExplorers::DataHandler* dh,
                       QWidget *parent = 0);

    ~Finlandia();

    /**
     * @brief makefilter takes the params given by user and
     * creates a filter based on them
     * @return map of filternames and their values. Contains
     * all the filters specified in the search made by user
     */
    std::map<Filter_NS, QString> makefilter();

    /**
     * @brief make_listview adds all search results to the listview
     *
     */
    void make_listview();


    /**
     * @brief make_listviweLabel checks what choises the user has made in the
     * UI and creates a label based on that
     */
    void make_listviweLabel();

    /**
     * @brief select_attributes
     * @return vector of integers pointing out what
     * attributes (year, distance, name etc.) the listView should show
     */
    std::vector<int> select_attributes();

    /**
     * @brief make_bar_chart
     * @param x indicates the attribute wanted on the x axis
     * @param y indicates the attribute wanted on y axis
     */
    void make_bar_chart(int x, int y);

    /**
     * @brief make_line_chart
     * @param x indicates the attribute wanted on the x axis
     * @param y indicates the attribute wanted on y axis
     */
    void make_line_chart(int x, int y);

    /**
     * @brief apply_special_filters does filtering for "Lisäehdot" -part
     * and stores the wanted information in the private section
     * @param filters
     */
    void apply_special_filters(std::map<Filter_NS, QString> filters);

    /**
     * @brief order_result is used if the user wants the data in a specific
     * order
     * @param filters
     */
    void order_result(std::map<Filter_NS,
                      QString> filters);

    /**
     * @brief check_for_special_filters returns 1 if special filters are
     * wanted, else 0
     * @return bool
     */
    bool check_for_special_filters();

    /**
     * @brief check_for_order_filter returns 1 if the user wants the data
     * in a specific order,else 0
     * @return  bool
     */
    bool check_for_order_filter();

    /**
     * @brief get_ordered_data is called by on_pushButtoLisaaHaku_clicked()
     * if check_for_special_filters returns 1.
     * @param filter
     * @return ordered and filtered data
     */
    std::vector<std::vector<std::string>> get_ordered_data(std::map<Filter_NS,
                                                           QString> filter);

    /**
     * @brief print_special_result is used to show thing like average times
     * in the listview
     * @param atr_vec pointing out what special filters were used
     */
    void print_special_result(std::vector<int> atr_vec);

private slots:

    /**
     * @brief on_pushButtonNollaKaikki_clicked resets
     * everything to default position
     */
    void on_pushButtonNollaKaikki_clicked();

    /**
     * @brief on_pushButtoLisaaHaku_clicked checks if special searches or
     * ordering is required and completes the search. Stores the data in private
     */
    void on_pushButtoLisaaHaku_clicked();

    /**
     * @brief on_pushButton_clicked Makes the search results appear, basically
     * just calls make_listview
     */
    void on_pushButton_clicked();

    /**
     * @brief make_chart creates the graphics of the chartView
     */
    void make_chart();

    /*!
     * \brief encryptionSettingsOpened opens a new settings window
     * for encryption settings
     */
    void encryptionSettingsOpened();

    /*!
     * \brief predicterOpened opens result predicter program in a new window
     */
    void predicterOpened();

    /*!
     * \brief openGame opens a small game in a new window
     */
    void openGame();

    /**
     * @brief remove_cart deletes all charts added to the chartView
     */
    void remove_cart();

    /**
     * @brief save_chart is used to save the current view of the
     * chartView
     */
    void save_chart();

    /*!
     * \brief Checks that range limits are valid and fixes them if not
     * \param newValue what number was spinbox changed to
     */
    void on_spinBoxSijoitusYla_valueChanged(int newValue);

    /*!
     * \brief Checks that range limits are valid and fixes them if not
     * \param newValue what number was spinbox changed to
     */
    void on_spinBoxSijoitusAla_valueChanged(int newValue);

private:
    Ui::Finlandia *ui;

    InternetExplorers::DataHandler *m_DataHandler;

    std::vector<std::vector<std::vector<std::string>>> allSearches;
    std::vector<std::vector<std::string>> m_datalump;

    QChart *m_chart;
    QString curr_series_title;
    std::vector<QString> all_titles;

    //this is used for getting the corresponing string with the enum index
    std::vector<std::string> attribute_enum = {"YEAR", "DISTANCE", "TIME",
                                               "PLACE", "PLACE_MEN",
                                               "PLACE_WOMEN", "SEX", "NAME",
                                               "CITY", "NATIONALITY",
                                               "BIRTH_YEAR", "TEAM",
                                               "NUMBER OF PARTICIPANTS",
                                              "FASTEST", "SLOWEST", "AVERAGE SPEED",
                                              "PARTICIPANTS NATION VICE",
                                              "BEST OF YEAR"};


    enum Atributes { year = 0, distance, time, place, place_men, place_wm,
                     sex, name, town, nationality, birth_yr, team,
                     nmbr_of_parts, fastest, slowest, avrg_speed,
                     nmbr_of_parts_nationvice, bestofx};

    std::vector<QMenu*> m_menus;

    // Settings window is stored here
    std::unique_ptr<EncryptionSettingsWindow> m_encryptionSettings;

    // Predicter window is stored here
    std::unique_ptr<PredicterWindow> m_predicter;


    //Place for storing all results of special searches
    //<year, nmr>
    std::map<std::string, int> m_nmbr_of_parts;
    //<year, row>
    std::map<std::string, std::vector<std::string>> m_fastest;
    // < year, row >
    std::map<std::string, std::vector<std::string>> m_slowest;
    // <year, time>
    std::map<std::string, std::string> m_avrg_time;
    //<nation, nmr>
    std::map<std::string, int> m_nmbr_of_parts_nationvice;
    //< team, average time >
    std::vector<std::pair<std::string, std::string>> m_best_of_year_X;

    // Game window is stored here
    std::unique_ptr<QMainWindow> m_gameWindow;


};

#endif // FINLANDIA_H
