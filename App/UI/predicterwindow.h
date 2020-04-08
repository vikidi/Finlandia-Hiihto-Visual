#ifndef PREDICTERWINDOW_H
#define PREDICTERWINDOW_H

#include <QWidget>
#include <QWebEngineView>

namespace Ui {
class PredicterWindow;
}

class PredicterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PredicterWindow(QWidget *parent = nullptr);
    ~PredicterWindow();

signals:
    void closeProgram();

private:
    Ui::PredicterWindow *ui;
    QWebEngineView *view;
};

#endif // PREDICTERWINDOW_H
