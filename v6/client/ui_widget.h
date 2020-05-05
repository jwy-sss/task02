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
    QPushButton *openBtn;
    QPushButton *sendBtn;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QProgressBar *clientProgressBar;
    QLabel *clientStatusLabel;
    QPushButton *continueBtn;
    QPushButton *zipbutton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(709, 467);
        openBtn = new QPushButton(Widget);
        openBtn->setObjectName(QString::fromUtf8("openBtn"));
        openBtn->setGeometry(QRect(170, 280, 75, 23));
        sendBtn = new QPushButton(Widget);
        sendBtn->setObjectName(QString::fromUtf8("sendBtn"));
        sendBtn->setGeometry(QRect(310, 280, 75, 23));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(180, 170, 54, 12));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(180, 200, 54, 12));
        hostLineEdit = new QLineEdit(Widget);
        hostLineEdit->setObjectName(QString::fromUtf8("hostLineEdit"));
        hostLineEdit->setGeometry(QRect(220, 170, 113, 20));
        portLineEdit = new QLineEdit(Widget);
        portLineEdit->setObjectName(QString::fromUtf8("portLineEdit"));
        portLineEdit->setGeometry(QRect(220, 200, 113, 20));
        clientProgressBar = new QProgressBar(Widget);
        clientProgressBar->setObjectName(QString::fromUtf8("clientProgressBar"));
        clientProgressBar->setGeometry(QRect(180, 230, 201, 23));
        clientProgressBar->setValue(0);
        clientStatusLabel = new QLabel(Widget);
        clientStatusLabel->setObjectName(QString::fromUtf8("clientStatusLabel"));
        clientStatusLabel->setGeometry(QRect(180, 260, 201, 16));
        continueBtn = new QPushButton(Widget);
        continueBtn->setObjectName(QString::fromUtf8("continueBtn"));
        continueBtn->setGeometry(QRect(310, 320, 75, 23));
        zipbutton = new QPushButton(Widget);
        zipbutton->setObjectName(QString::fromUtf8("zipbutton"));
        zipbutton->setGeometry(QRect(170, 320, 75, 23));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        openBtn->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200", nullptr));
        sendBtn->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\344\270\273\346\234\272\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\347\253\257\345\217\243\357\274\232", nullptr));
        hostLineEdit->setText(QString());
        portLineEdit->setText(QString());
        clientStatusLabel->setText(QString());
        continueBtn->setText(QCoreApplication::translate("Widget", "\347\273\247\347\273\255\345\217\221\351\200\201", nullptr));
        zipbutton->setText(QCoreApplication::translate("Widget", "\345\216\213\347\274\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
