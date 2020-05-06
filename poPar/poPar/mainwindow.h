#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "mstrgin.h"
#include <QDebug>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setStaInfo(const QString & str);

private slots:
    void on_auto_run_stateChanged(int arg1);

    void on_sn_auto_stateChanged(int arg1);

    void on_ex_iso_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    MStrgIn *strgIn;
    QLabel *info;
};
#endif // MAINWINDOW_H
