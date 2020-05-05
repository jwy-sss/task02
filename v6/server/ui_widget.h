/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *startBtn;
    QProgressBar *ServerProgressBar;
    QLabel *serverStatusLabel;
    QLineEdit *ipaddr;
    QLineEdit *portnum;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(477, 393);
        startBtn = new QPushButton(Widget);
        startBtn->setObjectName(QString::fromUtf8("startBtn"));
        startBtn->setGeometry(QRect(240, 220, 75, 23));
        ServerProgressBar = new QProgressBar(Widget);
        ServerProgressBar->setObjectName(QString::fromUtf8("ServerProgressBar"));
        ServerProgressBar->setGeometry(QRect(130, 180, 211, 23));
        ServerProgressBar->setValue(0);
        serverStatusLabel = new QLabel(Widget);
        serverStatusLabel->setObjectName(QString::fromUtf8("serverStatusLabel"));
        serverStatusLabel->setGeometry(QRect(130, 160, 201, 16));
        ipaddr = new QLineEdit(Widget);
        ipaddr->setObjectName(QString::fromUtf8("ipaddr"));
        ipaddr->setGeometry(QRect(180, 110, 113, 20));
        portnum = new QLineEdit(Widget);
        portnum->setObjectName(QString::fromUtf8("portnum"));
        portnum->setGeometry(QRect(180, 140, 113, 20));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(103, 110, 71, 20));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 140, 71, 20));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        startBtn->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213\347\233\221\345\220\254", nullptr));
        serverStatusLabel->setText(QString());
        ipaddr->setText(QString());
        label->setText(QCoreApplication::translate("Widget", "\346\234\215\345\212\241\345\231\250IP\345\234\260\345\235\200", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "    \347\253\257\345\217\243\345\217\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
