/********************************************************************************
** Form generated from reading UI file 'FieldDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIELDDIALOG_H
#define UI_FIELDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_FieldDialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;

    void setupUi(QDialog *FieldDialog)
    {
        if (FieldDialog->objectName().isEmpty())
            FieldDialog->setObjectName(QString::fromUtf8("FieldDialog"));
        FieldDialog->resize(455, 491);
        gridLayout_2 = new QGridLayout(FieldDialog);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(FieldDialog);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(FieldDialog);

        QMetaObject::connectSlotsByName(FieldDialog);
    } // setupUi

    void retranslateUi(QDialog *FieldDialog)
    {
        FieldDialog->setWindowTitle(QApplication::translate("FieldDialog", "\345\233\276\345\261\202\345\255\227\346\256\265\346\230\276\347\244\272\345\257\271\350\257\235\346\241\206", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FieldDialog: public Ui_FieldDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIELDDIALOG_H
