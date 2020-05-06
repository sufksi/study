#include "mstrgin.h"
#include <QDebug>

MStrgIn::MStrgIn(QWidget *parent,const QString& name) : QWidget(parent),labName(name)
{   
   ParamInit();
   qDebug()<<this->objectName();
   MStrgInInit(labName,isWrite, data,*coordinate);
}

MStrgIn::MStrgIn(QWidget* parent,const QString& labStr,bool isw, Uint32 *ptr,const QRect& coordinate): QWidget(parent)
{
    MStrgInInit(labStr,isw, ptr,coordinate);
}

MStrgIn:: ~MStrgIn()
{

}

void MStrgIn::MStrgInInit(const QString& labStr,bool isw, Uint32 *ptr,const QRect& coordinate)
{
    this->setGeometry(coordinate);

    hor = new QHBoxLayout(this);

    lab = new QLabel(this);

    lab->setMaximumSize(QSize(100, 16777215));
    lab->setText(labStr);

    lab->setFixedWidth(80);

    qspace[0] =  new QSpacerItem(40, 20, QSizePolicy::Maximum, QSizePolicy::Maximum);
    hor->addItem(qspace[0]);
    hor->addWidget(lab);

    for(int i = 0;i<PO_MODULES_STRGS_NUM;i++)
    {
        qle[i] = new QLineEdit(this);
        //qle[i]->setInputMask("9999");
        if(true == isw)
        {
            qle[i]->setEnabled(true);
        }
        else
        {
            qle[i]->setEnabled(false);
        }

        qle[i]->setMaximumSize(QSize(100, 16777215));
        qle[i]->setAlignment(Qt::AlignCenter);
        qle[i]->setText(QString::number(ptr[i]));
        connect(qle[i],&QLineEdit::editingFinished,this,
                [=](){
            data[i] = qle[i]->text().toInt();
            //qDebug()<<" i = "<<i;
            setStrg(i);
        });

        hor->addWidget(qle[i]);
        qspace[i+1] =  new QSpacerItem(40, 20, QSizePolicy::Maximum, QSizePolicy::Maximum);
        hor->addItem(qspace[i+1]);
    }
}
void MStrgIn::ParamInit()
{
    coordinate = new QRect(this->geometry());
    isWrite = true;
    Uint32 tmp = 100;
    for(int i = 0;i<16;i++)
    {
        data[i] = tmp;
        tmp *= 2;
    }
}
void MStrgIn::SetLableName(const QString& name)
{
    lab->setText(name);
}

void MStrgIn::printStrg()
{
    for(int i = 0;i<PO_MODULES_STRGS_NUM;i++)
    {
        qDebug()<<"strg["<<i<<"]= "<<data[i];
    }
}
void MStrgIn::setStrg(int index)
{
    qDebug()<<"set day param";
    emit mysignal(QString("set success"));
    //mysignal();
    /*if(parDay == mode)
    {
        qDebug()<<"set day param";
    }
    else if(parWdr == mode)
    {
        qDebug()<<"set wdr param";
    }
    else if(parNight == mode)
    {
        qDebug()<<"set night param";
    }*/
}

