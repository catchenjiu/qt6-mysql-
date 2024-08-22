#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QChartView>
#include <QtCharts>

QT_BEGIN_NAMESPACE
class QChartView;
class QChart;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectDB();
    void initData();

private slots:
    void on_action_triggered();

    void on_action_2_triggered();

    //void on_comboBoxfactory_currentTextChanged(const QString &arg1);

    //void on_comboBoxbrand_currentTextChanged(const QString &arg1);

    void setbrandname(QString name);

    void on_comboBoxfactory_currentIndexChanged(int index);

    void on_comboBoxbrand_currentIndexChanged(int index);

    void on_spinBox_valueChanged(int arg1);

    void on_buttonCancle_clicked();

    void on_buttonSure_clicked();

    void on_toolBox_currentChanged(int index);

    void on_buttonAdd_clicked();

    void on_buttonSub_clicked();

    void on_buttonRev_clicked();

    void on_buttonDel_clicked();

    void on_comboBoxOtherFactory_currentIndexChanged(int index);

    void on_comboBoxOtherBrand_currentIndexChanged(int index);

signals:
    void setbrand(QString name);
private:
    Ui::MainWindow *ui;
    QSqlTableModel * model;
};
#endif // MAINWINDOW_H
