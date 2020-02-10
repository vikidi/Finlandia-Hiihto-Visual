#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datahandler.h"

#include <QMainWindow>
#include <QProgressBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void dataReady();
    void progressChanged(const int);

private:

    DataHandler *m_dataHandler;
    QProgressBar *m_progress;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
