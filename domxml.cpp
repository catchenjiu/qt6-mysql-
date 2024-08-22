#include "domxml.h"
#include <QTextStream> //文件流
#include <QDebug>
#include <QDateTime>
#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"

DomXML::DomXML() {}

//根据文件路径创建文件
void DomXML::creatXML(QString filePath)
{
    QFile file(filePath);
    if(true == file.exists())//判断是否存在，存在则不创建
    {
        cout<<"已存在，不创建";
        return;
    }
    bool ret = file.open(QIODevice::WriteOnly);
    if(ret == true)
    {
        //打开成功开始写入
        //创建xml文档
        QDomDocument doc;
        //创建xml头部格式
        QDomProcessingInstruction ins;
        ins = doc.createProcessingInstruction("xml","version=\'1.0\' encoding=\'utf-8\'");
        //追加元素
        doc.appendChild(ins);

        //创建根节点
        QDomElement root = doc.createElement("日交易清单");
        doc.appendChild(root);

        //保存
        QTextStream stream(&file);//文本流关联文件
        doc.save(stream,4);//4是缩进字符

        file.close();
    }
    else
    {
        cout<<"DomXML::creatXML 文件打开失败";
        return;
    }
}

//根据文件路径读取文件，并添加交易信息
void DomXML::appendXML(QString filePath,QStringList list)
{
    //打开文件
    QFile file(filePath);
    bool ret = file.open(QIODevice::ReadOnly);
    if(true == ret)
    {
        //关联xml文件对象
        QDomDocument doc;
        QString error =0;
        int row = 0;
        int column = 0;
        ret = doc.setContent(&file,false,&error,&row,&column);
        if(true == ret)
        {
            //关联成功则关闭文件，同时获取根节点和时间，判断根节点下有无子节点
            file.close();
            QDomElement root = doc.documentElement();
            QDateTime date = QDateTime::currentDateTime();
            QString dateStr = date.toString("yyyy-MM-dd");

            if(root.hasChildNodes())//判断是否有子节点
            {
                //查找最后一个子节点进行判断是否是当前日期
                QDomElement lastemt = root.lastChildElement();
                if(lastemt.attribute("date") == dateStr)
                {
                    //是当前日期，在当前日期追加
                    writerXML(doc,lastemt,list);
                }
                else
                {
                    //不是当前日期，先创建当前日期节点，再对新创建的节点进行添加
                    //添加节点，添加属性，给属性赋值，给节点设置属性，将日期节点绑定到根节点
                    //调用write函数，添加当日详细时间和交易信息

                    //添加日期子节点
                    QDomElement dateEmt = doc.createElement("日期");
                    //添加日期属性
                    QDomAttr dateAttr = doc.createAttribute("date");
                    //给属性赋值
                    dateAttr.setNodeValue(dateStr);
                    //将属性绑定到日期节点
                    dateEmt.setAttributeNode(dateAttr);
                    //添加到根节点
                    root.appendChild(dateEmt);
                    //写入详细信息
                    writerXML(doc,dateEmt,list);
                }
            }
            else//无子节点则直接添加
            {
                //添加节点，添加属性，给属性赋值，给节点设置属性，将日期节点绑定到根节点
                //调用write函数，添加当日详细时间和交易信息

                //添加日期子节点
                QDomElement dateEmt = doc.createElement("日期");
                //添加日期属性
                QDomAttr dateAttr = doc.createAttribute("date");
                //给属性赋值
                dateAttr.setNodeValue(dateStr);
                //将属性绑定到日期节点
                dateEmt.setAttributeNode(dateAttr);
                //添加到根节点
                root.appendChild(dateEmt);
                //写入详细信息
                writerXML(doc,dateEmt,list);
            }

            //重新打开文件，将内容写入到文件
            ret = file.open(QIODevice::WriteOnly);
            if(true == ret)
            {
                QTextStream stream(&file);
                doc.save(stream,4);
                file.close();
                cout<<"DOM写入成功";
            }
            else
            {
                cout<<"DomXML::appendXML写入失败";
            }
        }
        else
        {
            cout<<"DomXML::appendXML 绑定失败";
            return;
        }
    }
}

//根据文件和节点信息，添加具体细节
void DomXML::writerXML(QDomDocument &doc,QDomElement &root,QStringList &list)
{
    //获取当前时间时分秒
    //创建时间节点和属性，对属性赋值，将属性绑定到节点
    //给时间节点创建子节点，将list内容添加到对应子节点，然后给时间节点添加子节点
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("hh-mm-ss");
    QDomElement timeEmt = doc.createElement("时间");
    QDomAttr timeAttr = doc.createAttribute("time");
    timeAttr.setNodeValue(timeStr);
    timeEmt.setAttributeNode(timeAttr);
    root.appendChild(timeEmt);

    QDomElement factory = doc.createElement("厂家");
    QDomElement brand = doc.createElement("品牌");
    QDomElement price = doc.createElement("报价");
    QDomElement num = doc.createElement("数量");
    QDomElement total = doc.createElement("金额");

    QDomText text = doc.createTextNode(list.at(0));
    factory.appendChild(text);
    timeEmt.appendChild(factory);

    text = doc.createTextNode(list.at(1));
    brand.appendChild(text);
    timeEmt.appendChild(brand);

    text = doc.createTextNode(list.at(2));
    price.appendChild(text);
    timeEmt.appendChild(price);

    text = doc.createTextNode(list.at(3));
    num.appendChild(text);
    timeEmt.appendChild(num);

    text = doc.createTextNode(list.at(4));
    total.appendChild(text);
    timeEmt.appendChild(total);

}

//从XML文件中读取信息，到后面五个引用参数中
void DomXML::readXML(QString filePath, QStringList &flist,QStringList &blist,QStringList &plist,QStringList &nlist,QStringList &tlist)
{
    //只读打开文件，绑定后获取根节点，查询根节点是否有子节点，没有则说明为空，返回
    //如果有子节点，则取出最后一个子节点，判断日期是否是当前日期，如果不是，则返回不读取
    //如果是当前日期则获取但前日期下全部交易记录
    QFile file(filePath);
    bool ret = file.open(QIODevice::ReadOnly);
    if(true == ret)
    {
        QDomDocument doc;
        QString error =0;
        int row = 0;
        int column = 0;
        ret = doc.setContent(&file,false,&error,&row,&column);
        if(true == ret)
        {
            //
            QDomElement root = doc.documentElement();
            file.close();
            QDateTime date = QDateTime::currentDateTime();
            QString dataStr = date.toString("yyyy-MM-dd");
            if(root.hasChildNodes())
            {
                //有子节点
                QDomElement lastEmt = root.lastChildElement();
                //查看最后一个子结点是否是当前日期
                if(lastEmt.attribute("date") == dataStr)
                {
                    //
                    QDomNodeList Nodelist = lastEmt.childNodes();
                    for(int i = 0;i< Nodelist.size();i++)
                    {
                        QDomNodeList listemt = Nodelist.at(i).toElement().childNodes();
                        QString str = listemt.at(0).toElement().text();
                        flist.append(str);
                        str = listemt.at(1).toElement().text();
                        blist.append(str);
                        str = listemt.at(2).toElement().text();
                        plist.append(str);
                        str = listemt.at(3).toElement().text();
                        nlist.append(str);
                        str = listemt.at(4).toElement().text();
                        tlist.append(str);

                    }
                }
                else
                {
                    cout<<"DomXML::readXML有子节点但不是当前日期";
                }
            }
            else
            {
                cout<<"DomXML::readXML没有子节点";
            }
        }
        else
        {
            cout<<"DomXML::readXML绑定失败";
        }
    }
    else
    {
        cout<<"DomXML::readXML 打开失败";
    }
}




