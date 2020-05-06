#include "client.h"
#include "ui_client.h"
#include <QHostAddress>
#include <QStatusBar>

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{

    ui->setupUi(this);
   // ui->lineEditIP->setInputMask("");
    /*实现window socket编程*/
    pClinet = new QTcpSocket(this);
    QStatusBar *info = new QStatusBar(this);
    lab = new QLabel(this);
    info->addWidget(lab);

    connect(pClinet,&QTcpSocket::connected,this,
            [=](){
        lab->setText("connect success");
    });

    connect(pClinet,&QTcpSocket::readyRead,this,
            [=](){
        lab->setText("read success");
        QByteArray str = pClinet->readAll();
        ui->textEditRead->append(str);
    });

}

Client::~Client()
{
    delete ui;
}

void Client::on_pushButton_pressed()
{
    QString IP = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    pClinet->connectToHost(QHostAddress(IP),port);


}

void Client::on_pushButtonSend_clicked()
{
    QString str = ui->textEditWrite->toPlainText();
    pClinet->write(str.toUtf8().data());
    lab->setText("send success");

}

void Client::on_pushButtonClose_clicked()
{
    pClinet->close();
}
