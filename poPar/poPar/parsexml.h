#ifndef PARSEXML_H
#define PARSEXML_H
#include <QString>
#include <QFile>
#include <QDomDocument> //文件
#include <QDomProcessingInstruction> //格式头部
#include <QDomElement>  //元素
#include <QTextStream> //文件流
#include <QDebug>
#include "modvector.h"
#include "pomodule.h"


class parseXml
{
public:
    parseXml();
    static bool writeXml(const QString& path,modvector<pomodule>& module);
};

#endif // PARSEXML_H
