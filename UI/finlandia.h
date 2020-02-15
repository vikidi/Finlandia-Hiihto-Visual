#ifndef FINLANDIA_H
#define FINLANDIA_H

#include <QMainWindow>

namespace Ui {
class Finlandia;
}

class Finlandia : public QMainWindow
{
    Q_OBJECT

public:
    explicit Finlandia(QWidget *parent = 0);
    ~Finlandia();

private slots:
    void on_pushButtonNollaKaikki_clicked();

    void on_pushButtoLisaaHaku_clicked();

private:
    Ui::Finlandia *ui;
};

#endif // FINLANDIA_H
