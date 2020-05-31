#include "client.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    pClinet = new QTcpSocket(this);
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    ui->lineEditIP->setValidator(new QRegExpValidator(rx, this));
    ui->lineEditPort->setInputMask("9999");

    connect(this->pClinet,&QTcpSocket::connected,this,&Client::conntodevice);

    connect(this->pClinet,&QTcpSocket::readyRead,this,
            [=]()
            {
                QString str;
                if(readData(str))
                    emitReadSignal(str);
            }
    );
}

Client::~Client()
{
    delete ui;
}
void Client::conntodevice()
{
    emit mysignal();
}
bool Client::disConnectServer()
{
    this->pClinet->disconnectFromHost();

    qDebug()<<"disConnect";

    return  true;
}
bool Client::connectServer()
{
    qDebug()<<ip;
    qDebug()<<port;

    pClinet->connectToHost(ip,port);
    qDebug()<<"connectServer";

    return  true;
}
void Client::on_pushButtonCancle_clicked()
{
    this->close();

}

void Client::on_pushButton_clicked()
{
    port = ui->lineEditPort->text().toUInt();
    ip = ui->lineEditIP->text();
    connectServer();


}

void Client::writeData(const QString& str)
{
    qDebug()<<pClinet->write(str.toUtf8().data(),str.size());
}
bool Client::readData(QString & str)
{
    str = pClinet->readAll();
    if(str.isEmpty())
    {
        return false;
    }
    else
    {
        return true;
    }
}
