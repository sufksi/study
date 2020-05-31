#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QButtonGroup>
#include <QCheckBox>

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),modIndex(0)
{
    ui->setupUi(this);

    this->setWindowTitle("SNPQTOOL");
    ui->ISO->SetLableName("ISO");
    ui->dayStrg->SetLableName("dayStrg");
    ui->wdrStrg->SetLableName("wdrStrg");
    ui->nighStrg->SetLableName("nighStrg");

    /*状态栏，可以用作更新信息*/
    QStatusBar *pstatBara = statusBar();
    QLabel *info = new QLabel();
    info->setText("normal text");
    pstatBara->addWidget(info);


    login = new Client();
    login->show();

    /*收到socket连接成功信号隐藏client窗口，显示Mainwindow*/
    connect(login,&Client::mysignal,
            [=](){
            login->hide();
            this->show();
    }
    );
    /*选择打开xml文件*/
    connect(ui->actionopen,&QAction::triggered,this,
            [=](){
        QString path =  QFileDialog::getOpenFileName(this,"open","../",
                        "xml(*.xml);;all(*.*)");
        /*读xml*/
        readXmltoModule(path);
        ui->listWidgetmode ->setFocus();
    });

    /*退出*/
    connect(ui->qPBExit,&QPushButton::pressed,this,&QWidget::close);

    /*打印调试信息*/
    connect(ui->printStrg,&QPushButton::pressed,
            [=](){
        ui->dayStrg->printStrg();
    });
    /*当有值改动是设置相关值*/
    void (MStrgIn:: *p)(int,int) = &MStrgIn::mysignal;
    connect(ui->dayStrg,p,this,
            [=](int index,int val){
        module[modIndex].setStrgByIndex(0,index,val);
        comPacket(WRITE_TO_SEVER_ONE_VALUE,modIndex,0,index);
        info->setText("day "+QString::number(index));
    });
    connect(ui->wdrStrg,p,this,
            [=](int index,int val){
        module[modIndex].setStrgByIndex(1,index,val);
        comPacket(WRITE_TO_SEVER_ONE_VALUE,modIndex,1,index);
        info->setText("wdr "+QString::number(index));
    });

    connect(ui->nighStrg,p,this,
            [=](int index,int val){
        module[modIndex].setStrgByIndex(2,index,val);
        comPacket(WRITE_TO_SEVER_ONE_VALUE,modIndex,2,index);
        info->setText("night "+QString::number(index));
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::readXmltoModule(const QString &name)
{
     QFile file(name);

     if (!file.open(QFile::ReadOnly | QFile::Text))
      {
           return false;
      }

      reader.setDevice(&file);
      reader.readNext();
      while(!reader.atEnd())
      {
          if(reader.isStartElement())
          {
              if(reader.name() == "ROOTNODE")
              {
                  readXmlSaveData();
              }
          }
          else
          {
              reader.readNext();
          }

      }
      return true;

}
bool MainWindow::readXmlMod()
{
    reader.readNext();
    while(!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        qDebug();
        if(reader.isStartElement())
        {
            if(reader.name() == "module")
            {
                pomodule tmp;
                QString name(reader.attributes().value("MOD").toString());
                ui->listWidgetmode->addItem(name);
                tmp.setName(name.toStdString());
                tmp.setAtuo(reader.attributes().value("IS_ATUO").toInt());
                tmp.setSnAuto(reader.attributes().value("SN_ATUO").toInt());
                tmp.setExIso(reader.attributes().value("EX_ISO").toInt());
                tmp.setStrgByStr(0,(char *)reader.attributes().value("DAY").toString().toStdString().c_str());
                tmp.setStrgByStr(1,(char *)reader.attributes().value("WDR").toString().toStdString().c_str());
                tmp.setStrgByStr(2,(char *)reader.attributes().value("NIGHT").toString().toStdString().c_str());
                module.push_back(tmp);

                reader.readElementText();
                if(reader.isEndElement())
                {
                    reader.readNext();
                }

                //std::cout<<tmp;
            }
            else
            {
                reader.readNext();
            }
        }
        else
        {
            reader.readNext();
        }


    }
    return true;
}
bool MainWindow::readXmlSaveData()
{
    reader.readNext();
    while(!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        qDebug()<<"aaaaaaaaaaaaaaa";
        if(reader.isStartElement())
        {
            if(reader.name() == "SAVEDATA")
            {
                readXmlMod();
            }
            else
            {
                reader.readNext();
            }

//            reader.readElementText();
//            if(reader.isEndElement())
//            {
//                reader.readNext();
//            }
        }
        else
        {
            reader.readNext();
        }

    }
    return true;
}

void MainWindow::setStaInfo(const QString & str)
{
    info->setText(str);
}

void MainWindow::on_auto_run_stateChanged(int arg1)
{
    if(Qt::Unchecked == arg1)
    {
        module[modIndex].setAtuo(false);
        ui->sn_auto->setEnabled(false);
        ui->ex_iso->setEnabled(false);
    }
    else if(Qt::Checked == arg1)
    {
        module[modIndex].setAtuo(true);
        ui->sn_auto->setEnabled(true);
        ui->ex_iso->setEnabled(true);
    }
     comPacket(WRITE_TO_SEVER_RUN_MODE,modIndex,0,0);
}

void MainWindow::on_sn_auto_stateChanged(int arg1)
{
    if(Qt::Unchecked == arg1)
        module[modIndex].setSnAuto(false);
    else if(Qt::Checked == arg1)
        module[modIndex].setSnAuto(true);

    comPacket(WRITE_TO_SEVER_RUN_MODE,modIndex,0,0);

}

void MainWindow::on_ex_iso_stateChanged(int arg1)
{
    if(Qt::Unchecked == arg1)
        module[modIndex].setExIso(false);
    else if(Qt::Checked == arg1)
        module[modIndex].setExIso(true);

    comPacket(WRITE_TO_SEVER_RUN_MODE,modIndex,0,0);
}

void MainWindow::setComBox(bool isAtuo,bool isSnAuto,bool isExIso)
{
    if(isAtuo)
    {
        ui->auto_run->setEnabled(true);
        ui->auto_run->setCheckState(Qt::Checked);
        ui->sn_auto->setEnabled(true);
        ui->ex_iso->setEnabled(true);
        if(isSnAuto)
        {
            ui->sn_auto->setCheckState(Qt::Checked);
        }
        else
        {
            ui->sn_auto->setCheckState(Qt::Unchecked);
        }

        if(isExIso)
        {
            ui->ex_iso->setCheckState(Qt::Checked);
        }
        else
        {
            ui->ex_iso->setCheckState(Qt::Unchecked);
        }

    }
    else
    {
        ui->auto_run->setEnabled(true);
        ui->auto_run->setCheckState(Qt::Unchecked);
        ui->sn_auto->setEnabled(false);
        ui->ex_iso->setEnabled(false);
    }

}

void MainWindow::on_listWidgetmode_itemSelectionChanged()
{
    int index = ui->listWidgetmode->currentRow();
    modIndex = index;   
    setComBox(module[index].getIsAtuo(),module[index].getSnAuto(),module[index].getExIso());

    ui->dayStrg->setStrgByArry(module[index].getStrg(0),PO_MODULES_STRGS_NUM);
    ui->wdrStrg->setStrgByArry(module[index].getStrg(1),PO_MODULES_STRGS_NUM);
    ui->nighStrg->setStrgByArry(module[index].getStrg(2),PO_MODULES_STRGS_NUM);
}


void MainWindow::readAll()
{

}
void MainWindow::readPage()
{

}
void MainWindow::writePage()
{

}

void MainWindow::socketWriteToSever(const QString& str)
{
    this->login->writeData(str);
    qDebug()<<str;
}
void MainWindow::saveXml()
{
    QString path =  QFileDialog::getSaveFileName();
    parseXml::writeXml(path,module);
}

void MainWindow::on_printStrg_clicked()
{
    saveXml();
}

void MainWindow::writeModule(int mod,QString& str)
{
    str.append(MOULE_START);
    writeRunMode(mod,str);
    for(int i = 0;i<pomodule::PARAM_ROW;i++)
    {
        str.append(MODE_START);
        for(int j = 0 ; j < pomodule::PARAM_COL;j++)
        {
            str.append(QString::number(module[mod].getValueByIndex(i,j)));
            if(j == pomodule::PARAM_COL-1)
                break;
            str.append(VALUE_SEP);
        }
        str.append(MODE_END);
    }
    str.append(MOULE_END);
}
void MainWindow::writeAll(QString& str)
{
    str.append(ALL_START);
    for(int i = 0;i< module.size();i++)
    {
        writeModule(i,str);
    }
    str.append(ALL_END);
}
void MainWindow::writeOneValue(int mod,int row,int col,QString& str)
{
    str.append(QString::number(mod));
    str.append(VALUE_SEP);
    str.append(QString::number(row));
    str.append(VALUE_SEP);
    str.append(QString::number(col));
    str.append(VALUE_SEP);
    str.append(QString::number(module[mod].getValueByIndex(row,col)));
}
void MainWindow::writeRunMode(int mod,QString& str)
{
    str.append(MODE_START);
    str.append(QString::number(module[mod].getIsAtuo()? 1:0));
    str.append(VALUE_SEP);
    str.append(QString::number(module[mod].getSnAuto()? 1:0));
    str.append(VALUE_SEP);
    str.append(QString::number(module[mod].getExIso()? 1:0));
    str.append(MODE_END);
}
QString MainWindow::comPacket(socket_cmd cmd,int mod,int row,int col)
{
    /*数据格式：###+cmd+'#'+data+;*/
    /*
        ###+cmd:
        {}：整个模块的数据。
        []:一个模块的数据。
        ():一个模块中一模式的数据，如day/night/wdr.
    */

    QString str;
    str.clear();
    str.append("###");
    str.append(QString::number(cmd));
    str.append("#");

    switch (cmd) {
        case READ_FORM_SEVER_ALL:/*全读*/
            str.append('#');
            str.append(";");
            break;
        case READ_FORM_SEVER_ONE_MODULE:/*读一个模块*/
            str.append('#');
            str.append(QString::number(mod));
            str.append('#');
            str.append(';');
            break;
        case READ_FORM_SEVER_ONE_VALUE:/*读一个模块中的一个值*/
            str.append('#');
            str.append(QString::number(mod));
            str.append('#');
            str.append(QString::number(col));
            str.append('#');
            str.append(';');
            break;
        case READ_FORM_SEVER_RUN_MODE:

            break;
        case WRITE_TO_SEVER_ALL:/*全写*/
            writeAll(str);
            str.append(';');
            break;
        case WRITE_TO_SEVER_ONE_MODULE:/*写一个模块*/
            str.append(QString::number(mod));
            str.append(VALUE_SEP);
            writeModule(mod,str);
            str.append(';');
            break;
        case WRITE_TO_SEVER_ONE_VALUE:/*写一个模块中的一个值*/
            writeOneValue(mod,row,col,str);
            str.append(';');
            break;
        case WRITE_TO_SEVER_RUN_MODE:
            writeRunMode(mod,str);
            break;
        default:
            qDebug()<<"invalid cmd";
            str.clear();
         break;

    }
    //str.append('\0');
    socketWriteToSever(str);
    return str;
}

void MainWindow::on_pushButton_2_clicked()
{
    comPacket(WRITE_TO_SEVER_ONE_MODULE,ui->listWidgetmode->currentRow(),0,0);
}

void MainWindow::on_pushButton_6_clicked()
{
    comPacket(WRITE_TO_SEVER_ALL,0,0,0);

}

void MainWindow::on_pushButton_8_clicked()
{
    this->login->disConnectServer();
    this->login->connectServer();
}
