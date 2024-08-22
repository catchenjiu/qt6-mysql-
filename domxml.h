#ifndef DOMXML_H
#define DOMXML_H
#include <QString>
#include <QStringList>
#include <QDomDocument>//xml文件
#include <QFile>
#include <QDomProcessingInstruction>//格式头
#include <QDomElement>//元素

class DomXML
{
public:
    DomXML();

    //根据文件路径创建文件
    static void creatXML(QString filePath);
    //根据文件路径读取文件，并添加交易信息
    static void appendXML(QString filePath,QStringList list);
    //根据文件和节点信息，添加具体细节
    static void writerXML(QDomDocument &doc,QDomElement &root,QStringList &list);
    //从XML文件中读取信息，到后面五个引用参数中
    static void readXML(QString filePath, QStringList &flist,QStringList &blist,QStringList &plist,QStringList &nlist,QStringList &tlist);

};

#endif // DOMXML_H
