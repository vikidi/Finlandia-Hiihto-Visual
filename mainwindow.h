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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Finlandia* finlandiaUI,
                        InternetExplorers::DataHandler* dh,
                        QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void dataReady();
    void progressChanged(const int);

    void on_haunAloitusNappi_clicked();

private:
    Finlandia* m_finlandiaUI;
    InternetExplorers::DataHandler *m_dataHandler;
    QProgressBar *m_progress;
    QGraphicsView* m_view;
    InternetExplorers::GameScene* m_scene;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
