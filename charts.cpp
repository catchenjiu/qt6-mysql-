#include "charts.h"

charts::charts() {}

//绘制条形图
void charts::drawBar(QChart *bar,QStringList name,QStringList num)
{
    //
    QBarSet * set0 = new QBarSet("0");
    for(int i = 0;i < num.size();i++)
    {
        set0->append(num.at(i).toInt());
        //qDebug()<<set0->at(i);
    }

    QBarSeries * series = new QBarSeries();
    series->append(set0);
    bar->addSeries(series);

    QBarCategoryAxis * axisX = new QBarCategoryAxis();
    axisX->setTitleText("x轴");
    axisX->append(name);

    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTickCount(11);
    axisY->setTitleText("Y轴");

    bar->addAxis(axisX,Qt::AlignBottom);
    bar->addAxis(axisY,Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    bar->legend()->setVisible(true);
    //delete set0;
    //delete series;
}
//绘制饼图，接收参数
void charts::drawPie(QChart *pie,QString name,qint8 sell,qint8 last)
{
    //
    QPieSeries* series = new QPieSeries();
    series->append("已售",sell);
    series->append("剩余",last);
    series->setPieSize(0.7);
    QPieSlice *slice = series->slices().at(0);
    slice->setExploded();
    series->setLabelsVisible(true);

    pie->addSeries(series);
    pie->legend()->setAlignment(Qt::AlignRight);
    pie->setTitle(name);

    //delete series;
    //delete slice;
}
