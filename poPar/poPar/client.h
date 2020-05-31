#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include "ui_client.h"
#include <QHostAddress>
#include <QStatusBar>
#include <QRegExp>
#include <QRegExpValidator>
#include <QPushButton>
#include <QString>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();

    void conntodevice();
    void writeData(const QString& str);
    bool readData(QString & str);
    bool disConnectServer();
    bool connectServer();



private slots:
    void on_pushButtonCancle_clicked();

    void on_pushButton_clicked();
signals:
        void mysignal();
        void emitReadSignal(QString);
//       void mysignal(int,QString);
private:
    Ui::Client *ui;
    QTcpSocket *pClinet;
    quint16 port;
    QString ip;

};

#endif // CLIENT_H
