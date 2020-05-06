#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <Qlabel>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void on_pushButton_pressed();

    void on_pushButtonSend_clicked();

    void on_pushButtonClose_clicked();

private:
    Ui::Client *ui;
    QTcpSocket *pClinet;
    QLabel *lab;
};

#endif // CLIENT_H
