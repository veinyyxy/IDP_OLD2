/********************************************************************************
** Form generated from reading UI file 'DB2Dialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DB2DIALOG_H
#define UI_DB2DIALOG_H

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

QT_BEGIN_NAMESPACE

class Ui_DB2Dialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_0;
    QLabel *label;
    QPushButton *pushBtnDel_0;
    QPushButton *pushBtnCal_0;
    QPushButton *pushBtnAdd_0;

    void setupUi(QDialog *DB2Dialog)
    {
        if (DB2Dialog->objectName().isEmpty())
            DB2Dialog->setObjectName(QString::fromUtf8("DB2Dialog"));
        DB2Dialog->resize(548, 43);
        gridLayout_2 = new QGridLayout(DB2Dialog);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit_0 = new QLineEdit(DB2Dialog);
        lineEdit_0->setObjectName(QString::fromUtf8("lineEdit_0"));

        gridLayout->addWidget(lineEdit_0, 0, 0, 1, 1);

        label = new QLabel(DB2Dialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        pushBtnDel_0 = new QPushButton(DB2Dialog);
        pushBtnDel_0->setObjectName(QString::fromUtf8("pushBtnDel_0"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushBtnDel_0->sizePolicy().hasHeightForWidth());
        pushBtnDel_0->setSizePolicy(sizePolicy);

        gridLayout->addWidget(pushBtnDel_0, 0, 4, 1, 1);

        pushBtnCal_0 = new QPushButton(DB2Dialog);
        pushBtnCal_0->setObjectName(QString::fromUtf8("pushBtnCal_0"));
        sizePolicy.setHeightForWidth(pushBtnCal_0->sizePolicy().hasHeightForWidth());
        pushBtnCal_0->setSizePolicy(sizePolicy);

        gridLayout->addWidget(pushBtnCal_0, 0, 2, 1, 1);

        pushBtnAdd_0 = new QPushButton(DB2Dialog);
        pushBtnAdd_0->setObjectName(QString::fromUtf8("pushBtnAdd_0"));
        sizePolicy.setHeightForWidth(pushBtnAdd_0->sizePolicy().hasHeightForWidth());
        pushBtnAdd_0->setSizePolicy(sizePolicy);
        pushBtnAdd_0->setIconSize(QSize(16, 16));

        gridLayout->addWidget(pushBtnAdd_0, 0, 3, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(DB2Dialog);

        QMetaObject::connectSlotsByName(DB2Dialog);
    } // setupUi

    void retranslateUi(QDialog *DB2Dialog)
    {
        DB2Dialog->setWindowTitle(QApplication::translate("DB2Dialog", "\347\255\211\345\200\274\351\235\242\347\224\237\346\210\220\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DB2Dialog", "DB2", 0, QApplication::UnicodeUTF8));
        pushBtnDel_0->setText(QApplication::translate("DB2Dialog", "-", 0, QApplication::UnicodeUTF8));
        pushBtnCal_0->setText(QApplication::translate("DB2Dialog", "\350\256\241\347\256\227", 0, QApplication::UnicodeUTF8));
        pushBtnAdd_0->setText(QApplication::translate("DB2Dialog", "+", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DB2Dialog: public Ui_DB2Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DB2DIALOG_H
