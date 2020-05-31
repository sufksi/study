#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "mstrgin.h"
#include <QDebug>
#include <QTcpSocket>
#include <QXmlStreamReader>
#include "client.h"
#include "modvector.h"
#include "pomodule.h"
#include "parsexml.h"
#include <QFileDialog>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
#define ALL_START '{'
#define ALL_END '}'
#define MOULE_START '['
#define MOULE_END ']'
#define MODE_START '('
#define MODE_END ')'
#define VALUE_SEP ','

    typedef enum  SCOKET_CMD{
        READ_FORM_SEVER_ALL = 0,
        READ_FORM_SEVER_ONE_MODULE,
        READ_FORM_SEVER_ONE_VALUE,
        READ_FORM_SEVER_RUN_MODE,

        WRITE_TO_SEVER_ALL,
        WRITE_TO_SEVER_ONE_MODULE,
        WRITE_TO_SEVER_ONE_VALUE,
        WRITE_TO_SEVER_RUN_MODE,

    }socket_cmd;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setStaInfo(const QString & str);
    bool readXmltoModule(const QString &name);
    bool readXmlSaveData();
    bool readXmlMod();

    void setComBox(bool isAtuo,bool isSnAuto,bool isExIso);
    void socketWriteToSever(const QString& str);
    void scoketReadFromSever();

    void readAll();
    void readPage();
    void writePage();
    void saveXml();


    void writeAll(QString& str);
    void writeModule(int mod,QString& strr);
    void writeOneValue(int mod,int stat,int index,QString& str);
    void writeRunMode(int mod,QString& str);

    QString comPacket(socket_cmd cmd,int mod,int row,int col);

private slots:
    void on_auto_run_stateChanged(int arg1);

    void on_sn_auto_stateChanged(int arg1);

    void on_ex_iso_stateChanged(int arg1);

    void on_listWidgetmode_itemSelectionChanged();

    void on_printStrg_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;   
    QLabel *info;
    Client *login;
    modvector<pomodule> module;
    QXmlStreamReader reader;
    int modIndex;
};
#endif // MAINWINDOW_H
