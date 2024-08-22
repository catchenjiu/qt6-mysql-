#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "domxml.h"
#include "charts.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlTableModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_action_triggered();
    connectDB();
    initData();

    DomXML::creatXML("../sell.xml");
    //connect(this,&MainWindow::setbrand,this,&MainWindow::setbrandname);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pageCar);
    ui->labelTitle->setText("车辆管理");
}


void MainWindow::on_action_2_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pageOther);
    ui->labelTitle->setText("销售统计");
}
//连接数据库
void MainWindow::connectDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("car");
    if(!db.open())
    {
        QMessageBox::warning(this,"open err",db.lastError().text());
        return;
    }
    model = new QSqlTableModel(this);
    model->setTable("brand");
    ui->tableView->setModel(model);
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
}
//初始化
void MainWindow::initData()
{
    QSqlQueryModel * queryModel = new QSqlQueryModel(this);
    queryModel->setQuery("select name from factory");
    ui->comboBoxfactory->setModel(queryModel);
    ui->comboBoxOtherFactory->setModel(queryModel);
    ui->labelLast->setText("0");
    ui->lineEditTotal->setEnabled(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void MainWindow::setbrandname(QString name)
{
    //ui->comboBoxfactory->addItem(name,Qt::DisplayRole);
    //ui->comboBoxbrand->addItem(name,Qt::DisplayRole);
}


void MainWindow::on_comboBoxfactory_currentIndexChanged(int index)
{
    if("请选择厂家" == ui->comboBoxfactory->currentText())
    {

        //清空当前页面
        ui->comboBoxbrand->clear();
        ui->lineEditPrice->clear();
        ui->lineEditTotal->clear();
        // if("0" != ui->labelLast->text())
        // {
        ui->labelLast->setText("0");
        // }
        ui->spinBox->setValue(0);
        ui->spinBox->setEnabled(false);
        ui->buttonSure->setEnabled(false);
    }
    else
    {
        ui->comboBoxbrand->clear();
        QString sql = QString("select name from brand where factory = '%1'").arg(ui->comboBoxfactory->currentText());
        QSqlQuery query;
        query.exec(sql);
        while(query.next())
        {
            QString name = query.value("name").toString();
            qDebug()<<name;
            ui->comboBoxbrand->addItem(name,Qt::DisplayRole);
        }
        ui->spinBox->setEnabled(true);
    }
}


void MainWindow::on_comboBoxbrand_currentIndexChanged(int index)
{
    QString sql = QString("select price,last from brand where factory = '%1' and name = '%2'").arg(ui->comboBoxfactory->currentText()).arg(ui->comboBoxbrand->currentText());
    QSqlQuery query;
    bool ret = query.exec(sql);
    //qDebug()<<ret;
    while(query.next())
    {
        int price = query.value("price").toInt();
        int last = query.value("last").toInt();
        //qDebug()<<price<<last;
        ui->lineEditPrice->setText(QString::number(price));
        ui->labelLast->setText(QString::number(last));
    }
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    QString sql = QString("select last from brand where factory = '%1' and name = '%2'").arg(ui->comboBoxfactory->currentText()).arg(ui->comboBoxbrand->currentText());
    QSqlQuery query;
    query.exec(sql);
    while(query.next())
    {
        int last = query.value(0).toInt();
        ui->spinBox->setRange(0,last);//设置范围
        int setlast = last - arg1;
        ui->labelLast->setText(QString::number(setlast));
    }
    int price = ui->lineEditPrice->text().toInt();
    int sum = price * arg1;
    ui->lineEditTotal->setText(QString::number(sum));
    if(arg1 == 0)
    {
        ui->buttonSure->setEnabled(false);
    }
    else
    {
        ui->buttonSure->setEnabled(true);
    }
}


void MainWindow::on_buttonCancle_clicked()
{
    ui->comboBoxfactory->setCurrentIndex(0);
}



void MainWindow::on_buttonSure_clicked()
{
    int num = ui->spinBox->value();
    int last = ui->labelLast->text().toInt();
    QString sql = QString("select sell from brand where factory = '%1' and name = '%2'").arg(ui->comboBoxfactory->currentText()).arg(ui->comboBoxbrand->currentText());
    QSqlQuery query;
    query.exec(sql);
    int sell;
    while(query.next())
    {
        sell = query.value(0).toInt();
    }
    sell += num;
    sql = QString("update brand set sell = %1 ,last = %2 where factory = '%3' and name = '%4'").arg(sell).arg(last).arg(ui->comboBoxfactory->currentText()).arg(ui->comboBoxbrand->currentText());
    bool ret = query.exec(sql);
    qDebug()<<ret;

    //数据更新到xml
    QStringList list;
    list<<ui->comboBoxfactory->currentText()
         <<ui->comboBoxbrand->currentText()
         <<ui->lineEditPrice->text()
         <<QString::number(num)
         <<ui->lineEditTotal->text();
    DomXML::appendXML("../sell.xml",list);

    //
    QStringList flist;
    QStringList blist;
    QStringList plist;
    QStringList nlist;
    QStringList tlist;
    DomXML::readXML("../sell.xml",flist,blist,plist,nlist,tlist);
    ui->textEdit->clear();
    for(int i = 0;i<flist.size();i++)
    {
        QString str = QString("%1.%2售出单价:%3,售出数量:%4,总金额:%5").arg(flist.at(i)).arg(blist.at(i)).arg(plist.at(i)).arg(nlist.at(i)).arg(tlist.at(i));
        ui->textEdit->append(str);
    }
    ui->comboBoxfactory->setCurrentIndex(0);
}


void MainWindow::on_toolBox_currentChanged(int index)
{
    model->select();
}


void MainWindow::on_buttonAdd_clicked()
{
    int id = ui->lineID->text().toInt();
    QString factory = ui->lineFactory->text();
    QString name = ui->lineName->text();
    int price = ui->linePrice->text().toInt();
    int num = ui->lineNum->text().toInt();

    QSqlQuery query;
    QString sqlsel;//查看数据库内有无
    QString sqlins;//添加语句
    QString sqlupd;//更新语句
    //开启事务
    QSqlDatabase::database().transaction();

    sqlsel = QString("select count(*) from brand where factory = '%1' and name = '%2'").arg(factory).arg(name);
    bool ret = query.exec(sqlsel);
    //qDebug()<<"新车入库，查询语句"<<ret;
    int val;
    while(query.next())
    {
        val = query.value(0).toInt();
    }
    if(0 == val)
    {
        //添加
        qDebug()<<"不存在";
        query.prepare("insert into brand (id,factory,name,price,sum,sell,last) values(?,?,?,?,?,?,?)");
        query.addBindValue(id);
        query.addBindValue(factory);
        query.addBindValue(name);
        query.addBindValue(price);
        query.addBindValue(num);
        query.addBindValue(0);
        query.addBindValue(num);
        bool ret = query.exec();
        qDebug()<<ret;
    }
    else
    {
        //更新
        //qDebug()<<"存在";
        if(num > 0)
        {
            QString sqlsum = QString("select sum from brand where factory = '%1' and name = '%2'").arg(factory).arg(name);
            query.exec(sqlsum);
            while(query.next())
            {
                num += query.value(0).toInt();
            }
            sqlupd = QString("update brand set price = %1 , sum = %2 where factory = '%3' and name = '%4';").arg(price).arg(num).arg(factory).arg(name);
            query.exec(sqlupd);
        }

    }
    on_buttonDel_clicked();

    //给tableview设置上要添加的数据
    model->select();
    ui->buttonAdd->setEnabled(false);

}


void MainWindow::on_buttonSub_clicked()
{
    QSqlDatabase::database().commit();
    ui->buttonAdd->setEnabled(true);
}


void MainWindow::on_buttonRev_clicked()
{
    QSqlDatabase::database().rollback();
    ui->buttonAdd->setEnabled(true);
}


void MainWindow::on_buttonDel_clicked()
{
    ui->lineID->clear();
    ui->lineFactory->clear();
    ui->lineName->clear();
    ui->linePrice->clear();
    ui->lineNum->clear();
}


void MainWindow::on_comboBoxOtherFactory_currentIndexChanged(int index)
{
    QChart *bar = ui->barCharts->chart();
    QChart *pie = ui->pieCharts->chart();

    if(ui->comboBoxOtherFactory->currentText() == "请选择厂家")
    {
        ui->comboBoxOtherBrand->clear();
        //清除绘制的元素
        bar->removeAllSeries();
        for (QAbstractAxis *axis : bar->axes()) {
            bar->removeAxis(axis);
        }
        pie->removeAllSeries();
    }
    else
    {
        //清除绘制的元素
        bar->removeAllSeries();
        for (QAbstractAxis *axis : bar->axes()) {
            bar->removeAxis(axis);
        }
        pie->removeAllSeries();
        QStringList namelist;
        QStringList selllist;
        ui->comboBoxOtherBrand->clear();
        QString sql = QString("select name,sell from brand where factory = '%1'").arg(ui->comboBoxOtherFactory->currentText());
        QSqlQuery query;
        query.exec(sql);
        while(query.next())
        {
            QString name = query.value("name").toString();
            QString sell = query.value("sell").toString();
            namelist<<name;
            selllist<<sell;
            ui->comboBoxOtherBrand->addItem(name,Qt::DisplayRole);
        }
        charts::drawBar(bar,namelist,selllist);
    }
}


void MainWindow::on_comboBoxOtherBrand_currentIndexChanged(int index)
{
    //
    QChart *pie = ui->pieCharts->chart();
    pie->removeAllSeries();
    QString sql = QString("select sell,last from brand where factory = '%1' and name = '%2'").arg(ui->comboBoxOtherFactory->currentText()).arg(ui->comboBoxOtherBrand->currentText());
    QSqlQuery query;
    query.exec(sql);
    qint8 piesell;
    qint8 pielast;

    QString name = ui->comboBoxOtherBrand->currentText();
    while(query.next())
    {
        //    static void drawPie(QChart *pie,QString name,qint8 sell,qint8 last);
        piesell = query.value("sell").toInt();
        pielast = query.value("last").toInt();
    }
    charts::drawPie(pie,name,piesell,pielast);
}

