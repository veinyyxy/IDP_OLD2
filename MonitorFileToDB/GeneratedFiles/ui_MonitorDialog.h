/********************************************************************************
** Form generated from reading UI file 'MonitorDialog.ui'
**
** Created: Thu Nov 7 11:30:58 2013
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONITORDIALOG_H
#define UI_MONITORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_MonitorDialogClass
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;

    void setupUi(QDialog *MonitorDialogClass)
    {
        if (MonitorDialogClass->objectName().isEmpty())
            MonitorDialogClass->setObjectName(QString::fromUtf8("MonitorDialogClass"));
        MonitorDialogClass->resize(505, 158);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MonitorDialogClass->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(MonitorDialogClass);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(MonitorDialogClass);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(MonitorDialogClass);

        QMetaObject::connectSlotsByName(MonitorDialogClass);
    } // setupUi

    void retranslateUi(QDialog *MonitorDialogClass)
    {
        MonitorDialogClass->setWindowTitle(QApplication::translate("MonitorDialogClass", "\351\233\267\350\276\276\346\226\207\344\273\266\344\276\246\345\220\254\345\205\245\345\272\223\346\234\215\345\212\241\347\250\213\345\272\217", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MonitorDialogClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; color:#ff0000;\">\351\233\267\350\276\276\346\226\207\344\273\266\344\276\246\345\220\254\345\205\245\345\272\223\346\234\215\345\212\241\347\250\213\345\272\217</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MonitorDialogClass: public Ui_MonitorDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONITORDIALOG_H
