#include "mainwindow.h"

#include <QApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    /*
    Client c;
    c.show();*/
    return a.exec();
}