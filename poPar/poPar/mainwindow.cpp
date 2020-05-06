#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QButtonGroup>
#include <QCheckBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#if 1
    this->setWindowTitle("SNPQTOOL");
    ui->ISO->SetLableName("ISO");
    ui->dayStrg->SetLableName("dayStrg");
    ui->wdrStrg->SetLableName("wdrStrg");
    ui->nighStrg->SetLableName("nighStrg");

    QStatusBar *pstatBara = statusBar();
    QLabel *info = new QLabel();
    info->setText("normal text");
    pstatBara->addWidget(info);

    /* audio 分组*/
   /* QButtonGroup *m_btnGroupAuto = new QButtonGroup(this);
    QButtonGroup *m_btnGroupSnAtuo = new QButtonGroup(this);
    QButtonGroup *m_btnGroupExIso = new QButtonGroup(this);

*/



    connect(ui->qPBExit,&QPushButton::pressed,this,&QWidget::close);
    connect(ui->printStrg,&QPushButton::pressed,
            [=](){
        ui->dayStrg->printStrg();
    });
    void (MStrgIn:: *p)(QString) = &MStrgIn::mysignal;
    connect(ui->wdrStrg,p,this,
            [=](QString str){
        info->setText(str);
    });

    connect(ui->nighStrg,p,this,
            [=](QString str){
        info->setText(str);
    });
#endif







}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStaInfo(const QString & str)
{
    info->setText(str);
}

void MainWindow::on_auto_run_stateChanged(int arg1)
{
    if(Qt::Unchecked == arg1)
    {
        ui->sn_auto->setEnabled(false);
        ui->ex_iso->setEnabled(false);
    }
    else if(Qt::Checked == arg1)
    {
        ui->sn_auto->setEnabled(true);
        ui->ex_iso->setEnabled(true);
    }
}

void MainWindow::on_sn_auto_stateChanged(int arg1)
{
    qDebug()<<arg1;
}

void MainWindow::on_ex_iso_stateChanged(int arg1)
{
    qDebug()<<arg1;
}
