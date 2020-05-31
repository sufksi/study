#include "mainwindow.h"

#include <QApplication>
#include "client.h"

#include  <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    return a.exec();
}
