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
    std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> makefilter();

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
     * attributes the listView should show
     */
    std::vector<int> select_attributes();

    /**
     * @brief convert_time_to_seconds helper function
     * @param time
     * @return time in seconds as double
     */
    int convert_time_to_seconds(std::string time);

private slots:

    /**
     * @brief on_pushButtonNollaKaikki_clicked resets
     * everything to default position
     */
    void on_pushButtonNollaKaikki_clicked();

    /**
     * @brief on_pushButtoLisaaHaku_clicked adds the filters to
     * a queue for completing the searches
     */
    void on_pushButtoLisaaHaku_clicked();

    /**
     * @brief on_pushButton_clicked Makes the actual searches happen
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

    void predicterOpened();

    /**
     * @brief remove_cart deletes all charts added to the chartView
     */
    void remove_cart();

    /**
     * @brief save_chart is used to save the current view of the
     * chartView
     */
    void save_chart();

    /**
     * @brief make_search is called when pushbutton for making the searches
     * is pushed. It is called for all the filters in the m_search_queue and
     * it stores the data into allSearches vector
     * @param filter
     */
    void make_search(std::map<Filter_NS, QString> filter);

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
    std::vector<Filter_NS> previousSrc;

    InternetExplorers::DataHandler *m_DataHandler;

    std::vector<std::vector<std::vector<std::string>>> allSearches;
    std::vector<std::vector<std::string>> m_datalump;

    QChart *m_chart;
    QString curr_series_title;

    //this is used for getting the corresponing string with the enum index
    std::vector<std::string> attribute_enum = {"YEAR", "DISTANCE", "TIME",
                                               "PLACE", "PLACE_MEN",
                                               "PLACE_WOMEN", "SEX", "NAME",
                                               "CITY", "NATIONALITY",
                                               "BIRTH_YEAR", "TEAM"};
    std::vector<QMenu*> m_menus;

    //vector for saving filters after "lisää haku" pressed
    std::vector<std::map<Filter_NS, QString>> m_search_queue;

    // Settings window is stored here
    std::unique_ptr<EncryptionSettingsWindow> m_encryptionSettings;

    // Predicter window is stored here
    std::unique_ptr<PredicterWindow> m_predicter;

};

#endif // FINLANDIA_H
