#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datahandler.h"
#include "UI/finlandia.h"
#include "UI/gamescene.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QGraphicsView>

namespace Ui {
class MainWindow;
}

/*!
 * \brief
 * Starting window for the application
 * \details
 * This class contains the implementation of the starting window.<br>
 * It contains a custom progress bar<br>
 * and a game to play while the data is loading.<br>
 * It also contains a start bullton, which will be<br>
 * enabled after data loading is ready.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /*!
     * \brief
     * Default constructor
     * \param finlandiaUI
     * Finlandia UI main window
     * \param dh
     * Backend data handler
     */
    explicit MainWindow(Finlandia* finlandiaUI,
                        InternetExplorers::DataHandler* dh,
                        QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    /*!
     * \brief
     * Backend has data ready to be used.
     */
    void dataReady();

    /*!
     * \brief
     * Loading progress has been changed in backend. Connects to progress bar.
     * * \param progress
     * The current progress of the loading
     */
    void progressChanged(const int progress);

    /*!
     * \brief
     * Begins the search -> opens the FinlandiaUI window
     */
    void on_haunAloitusNappi_clicked();

private:

    // Finlandia UI window
    Finlandia* m_finlandiaUI;

    // Backend data handler
    InternetExplorers::DataHandler *m_dataHandler;

    // Main window progress bar
    QProgressBar *m_progress;

    // View for custom progress bar
    QGraphicsView* m_view;

    // Scene for the custom progress bar
    InternetExplorers::GameScene* m_scene;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
