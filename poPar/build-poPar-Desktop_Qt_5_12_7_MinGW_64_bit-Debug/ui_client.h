/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.12.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButtonSend;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEditIP;
    QTextEdit *textEditRead;
    QTextEdit *textEditWrite;
    QLabel *labelPort;
    QPushButton *pushButton;
    QLineEdit *lineEditPort;
    QLabel *labelIP;
    QPushButton *pushButtonClose;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName(QString::fromUtf8("Client"));
        Client->resize(734, 627);
        gridLayout = new QGridLayout(Client);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButtonSend = new QPushButton(Client);
        pushButtonSend->setObjectName(QString::fromUtf8("pushButtonSend"));

        gridLayout->addWidget(pushButtonSend, 6, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(515, 25, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 6, 1, 1, 2);

        lineEditIP = new QLineEdit(Client);
        lineEditIP->setObjectName(QString::fromUtf8("lineEditIP"));

        gridLayout->addWidget(lineEditIP, 1, 1, 1, 1);

        textEditRead = new QTextEdit(Client);
        textEditRead->setObjectName(QString::fromUtf8("textEditRead"));
        textEditRead->setReadOnly(true);

        gridLayout->addWidget(textEditRead, 4, 0, 1, 4);

        textEditWrite = new QTextEdit(Client);
        textEditWrite->setObjectName(QString::fromUtf8("textEditWrite"));

        gridLayout->addWidget(textEditWrite, 5, 0, 1, 4);

        labelPort = new QLabel(Client);
        labelPort->setObjectName(QString::fromUtf8("labelPort"));

        gridLayout->addWidget(labelPort, 2, 0, 1, 1);

        pushButton = new QPushButton(Client);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 1, 2, 2, 2);

        lineEditPort = new QLineEdit(Client);
        lineEditPort->setObjectName(QString::fromUtf8("lineEditPort"));

        gridLayout->addWidget(lineEditPort, 2, 1, 2, 1);

        labelIP = new QLabel(Client);
        labelIP->setObjectName(QString::fromUtf8("labelIP"));

        gridLayout->addWidget(labelIP, 1, 0, 1, 1);

        pushButtonClose = new QPushButton(Client);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));

        gridLayout->addWidget(pushButtonClose, 6, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 2, 1, 1);


        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QWidget *Client)
    {
        Client->setWindowTitle(QApplication::translate("Client", "Form", nullptr));
        pushButtonSend->setText(QApplication::translate("Client", "send", nullptr));
        labelPort->setText(QApplication::translate("Client", "PORT", nullptr));
        pushButton->setText(QApplication::translate("Client", "connect", nullptr));
        labelIP->setText(QApplication::translate("Client", "IP", nullptr));
        pushButtonClose->setText(QApplication::translate("Client", "close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
