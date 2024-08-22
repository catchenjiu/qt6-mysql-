#ifndef CHARTS_H
#define CHARTS_H
#include <QChartView>
#include <QtCharts>

QT_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_END_NAMESPACE


class charts
{
public:
    charts();

    //绘制条形图
    static void drawBar(QChart *bar, QStringList name,QStringList num);
    //绘制饼图，接收参数
    static void drawPie(QChart *pie,QString name,qint8 sell,qint8 last);

signals:
    //信号，返回图片
    void barChart(QChart *bar);
    void pieChart(QChart *pie);
};

#endif // CHARTS_H
