#include "parsexml.h"
#include "mstrgin.h"

parseXml::parseXml()
{

}
bool parseXml::writeXml(const QString& path,modvector<pomodule>& data)
{
    QFile file(path);
    QVector<QString> modAttrlist={"MOD","IS_ATUO","SN_ATUO","EX_ISO","DAY","WDR","NIGHT"};

    bool isOK = file.open(QIODevice::WriteOnly);
    if(isOK)
    {
        QDomDocument doc;
        QDomProcessingInstruction ins;
        ins = doc.createProcessingInstruction("xml", "version=\'1.0\' encoding=\'utf-8\'");
        //追加元素
        doc.appendChild(ins);
        QDomElement root = doc.createElement("ROOTNODE");
        doc.appendChild(root);
        QDomElement savedata = doc.createElement("SAVEDATA");
        for(int i =0;i< data.size() ;i++)
        {
            QDomElement module = doc.createElement("module");
            QDomAttr modAttr;
            QString tmpStr;

            modAttr = doc.createAttribute(modAttrlist[0]);
            modAttr.setValue(QString::fromStdString(data[i].getName()));
            module.setAttributeNode(modAttr);

            modAttr = doc.createAttribute(modAttrlist[1]);
            tmpStr = data[i].getIsAtuo() ? "1" : "0";
            modAttr.setValue(tmpStr);
            module.setAttributeNode(modAttr);

            modAttr = doc.createAttribute(modAttrlist[2]);
            tmpStr = data[i].getSnAuto() ? "1" : "0";
            modAttr.setValue(tmpStr);
            module.setAttributeNode(modAttr);

            modAttr = doc.createAttribute(modAttrlist[3]);
            tmpStr = data[i].getExIso() ? "1" : "0";
            modAttr.setValue(tmpStr);
            module.setAttributeNode(modAttr);

            /*DAY strg*/
            modAttr = doc.createAttribute(modAttrlist[4]);
            tmpStr.clear();
            int *strg = data[i].getDayStrg();
            for(int j = 0;j < PO_MODULES_STRGS_NUM;j++)
            {
                QString tmp = QString::number((*strg++),16);
                tmpStr.append("0x");
                tmpStr.append(tmp);
                if(j == PO_MODULES_STRGS_NUM - 1)
                    break;
                tmpStr.append(',');
            }
            modAttr.setValue(tmpStr);
            module.setAttributeNode(modAttr);

            /*WDR strg*/
            modAttr = doc.createAttribute(modAttrlist[5]);
            tmpStr.clear();
            strg = data[i].getWdrStrg();
            for(int j = 0;j < PO_MODULES_STRGS_NUM;j++)
            {
                QString tmp = QString::number((*strg++),16);
                tmpStr.append("0x");
                tmpStr.append(tmp);
                if(j == PO_MODULES_STRGS_NUM - 1)
                    break;
                tmpStr.append(',');
            }
            modAttr.setValue(tmpStr);
            module.setAttributeNode(modAttr);

            /*NIGHT strg*/
            modAttr = doc.createAttribute(modAttrlist[6]);
            tmpStr.clear();
            strg = data[i].getNightStrg();
            for(int j = 0;j < PO_MODULES_STRGS_NUM;j++)
            {
                QString tmp = QString::number((*strg++),16);
                tmpStr.append("0x");
                tmpStr.append(tmp);
                if(j == PO_MODULES_STRGS_NUM - 1)
                    break;
                tmpStr.append(',');
            }
            modAttr.setValue(tmpStr);
            module.setAttributeNode(modAttr);

            savedata.appendChild(module);
        }
        root.appendChild(savedata);
        //保存
        QTextStream stream(&file); //文本流关联文件
        doc.save(stream, 4); //4 缩进字符
        file.close(); //关闭文件
        qDebug()<<"aaaaaaaaaaaaaaaaaaaa";
    }
    else
    {
        return false;
    }


    return true;
}
