/********************************************************************************
** Form generated from reading UI file 'ProjCenterDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJCENTERDIALOG_H
#define UI_PROJCENTERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ProjCenterDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit_Lon;
    QLabel *label_2;
    QLineEdit *lineEdit_Lat;
    QPushButton *pushButton_OK;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *ProjCenterDialog)
    {
        if (ProjCenterDialog->objectName().isEmpty())
            ProjCenterDialog->setObjectName(QString::fromUtf8("ProjCenterDialog"));
        ProjCenterDialog->resize(500, 70);
        ProjCenterDialog->setMinimumSize(QSize(300, 40));
        ProjCenterDialog->setMaximumSize(QSize(500, 70));
        gridLayout = new QGridLayout(ProjCenterDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ProjCenterDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit_Lon = new QLineEdit(ProjCenterDialog);
        lineEdit_Lon->setObjectName(QString::fromUtf8("lineEdit_Lon"));

        gridLayout->addWidget(lineEdit_Lon, 0, 1, 1, 1);

        label_2 = new QLabel(ProjCenterDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        lineEdit_Lat = new QLineEdit(ProjCenterDialog);
        lineEdit_Lat->setObjectName(QString::fromUtf8("lineEdit_Lat"));

        gridLayout->addWidget(lineEdit_Lat, 0, 3, 1, 1);

        pushButton_OK = new QPushButton(ProjCenterDialog);
        pushButton_OK->setObjectName(QString::fromUtf8("pushButton_OK"));

        gridLayout->addWidget(pushButton_OK, 0, 5, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 4, 1, 1);


        retranslateUi(ProjCenterDialog);

        QMetaObject::connectSlotsByName(ProjCenterDialog);
    } // setupUi

    void retranslateUi(QDialog *ProjCenterDialog)
    {
        ProjCenterDialog->setWindowTitle(QApplication::translate("ProjCenterDialog", "\350\256\276\347\275\256\346\212\225\345\275\261\344\270\255\345\277\203", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ProjCenterDialog", "\347\273\217\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ProjCenterDialog", "\347\272\254\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        pushButton_OK->setText(QApplication::translate("ProjCenterDialog", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProjCenterDialog: public Ui_ProjCenterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJCENTERDIALOG_H
