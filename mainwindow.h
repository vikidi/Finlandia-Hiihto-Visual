#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private:
    std::string escape(const std::string&);
    void escapeBR(std::string&);
    void escapeSpaces(std::string& src);
    void printResults(std::vector<std::vector<std::string>>&) const;
    void sumSize(std::vector<std::vector<std::string>>);

    Ui::MainWindow *ui;

    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
};

#endif // MAINWINDOW_H
