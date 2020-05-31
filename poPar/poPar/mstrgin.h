#ifndef MSTRGIN_H
#define MSTRGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QLabel>

class MStrgIn : public QWidget
{
    Q_OBJECT
   public:
#define PO_MODULES_STRGS_NUM	(16)

typedef unsigned int Uint32;
typedef enum PARAM_MODE{
        parDay = 0,
        parWdr = 1,
        parNight =2,
    }enParMode;
public:
    explicit MStrgIn(QWidget *parent,const QString& name = "name");
    MStrgIn(QWidget* parent,const QString& labStr,bool isw, Uint32 *ptr,const QRect& coordinate);
    ~MStrgIn();

    void MStrgInInit(const QString& labStr,bool isw,Uint32 *ptr,const QRect& coordinate);
    void ParamInit();
    void SetLableName(const QString& name);
    void printStrg();
    void setStrg(int index);
    void setStrgByArry(int *arry,int size);
    void reLoadLineExit();

signals:
        void mysignal(int index,int val);
private:
    QLineEdit *qle[PO_MODULES_STRGS_NUM];
    QWidget *qwidg;
    QSpacerItem *qspace[PO_MODULES_STRGS_NUM];
    QHBoxLayout *hor;
    QLabel *lab;

    QRect *coordinate;
    bool isWrite;   /** 是否使能*/
    Uint32 data[PO_MODULES_STRGS_NUM];
    QString labName;
};

#endif // MSTRGIN_H
