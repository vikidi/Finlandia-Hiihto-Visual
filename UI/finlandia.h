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

private:
    Ui::Finlandia *ui;
};

#endif // FINLANDIA_H
