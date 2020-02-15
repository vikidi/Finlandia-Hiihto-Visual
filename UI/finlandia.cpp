#include <QWidget>
#include "finlandia.h"
#include "UI/finlandia.h"
#include "ui_finlandia.h"
#include <QtCharts>

Finlandia::Finlandia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Finlandia)
{
    ui->setupUi(this);


    QBarSet *set0 = new QBarSet("Jane");
    QBarSet *set1 = new QBarSet("John");
    QBarSet *set2 = new QBarSet("Axel");
    QBarSet *set3 = new QBarSet("Mary");
    QBarSet *set4 = new QBarSet("Sam");

    *set0 << 1 << 2 << 3 << 4 << 5 << 6;
    *set1 << 5 << 0 << 0 << 4 << 0 << 7;
    *set2 << 3 << 5 << 8 << 13 << 8 << 5;
    *set3 << 5 << 6 << 7 << 3 << 4 << 5;
    *set4 << 9 << 7 << 5 << 3 << 1 << 2;
    QBarSeries *barseries = new QBarSeries();
    barseries->append(set0);
    barseries->append(set1);
    barseries->append(set2);
    barseries->append(set3);
    barseries->append(set4);

    QLineSeries *lineseries = new QLineSeries();
    lineseries->setName("trend");
    lineseries->append(QPoint(0, 4));
    lineseries->append(QPoint(1, 15));
    lineseries->append(QPoint(2, 20));
    lineseries->append(QPoint(3, 4));
    lineseries->append(QPoint(4, 12));
    lineseries->append(QPoint(5, 17));

    QChart *chart = new QChart();
    chart->addSeries(barseries);
    chart->addSeries(lineseries);
    chart->setTitle("Line and barchart example");

    ui->graafiWiev->setChart(chart);


}

Finlandia::~Finlandia()
{
    delete ui;
}

void Finlandia::on_pushButtonNollaKaikki_clicked()
{

}

void Finlandia::on_pushButtoLisaaHaku_clicked()
{

}
