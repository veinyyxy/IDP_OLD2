/********************************************************************************
** Form generated from reading UI file 'AreaDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AREADIALOG_H
#define UI_AREADIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_AreaDialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QRadioButton *radioBtnAll;
    QRadioButton *radioBtnNoAll;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QRadioButton *radioBtnB;
    QRadioButton *radioBtnZ;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_8;
    QGridLayout *gridLayout_7;
    QLabel *label;
    QComboBox *cmbMin;
    QLabel *label_2;
    QComboBox *cmbMax;
    QPushButton *pushBtnOK;
    QPushButton *pushBtnExit;

    void setupUi(QDialog *AreaDialog)
    {
        if (AreaDialog->objectName().isEmpty())
            AreaDialog->setObjectName(QString::fromUtf8("AreaDialog"));
        AreaDialog->resize(246, 293);
        gridLayout_2 = new QGridLayout(AreaDialog);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(AreaDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        radioBtnAll = new QRadioButton(groupBox);
        radioBtnAll->setObjectName(QString::fromUtf8("radioBtnAll"));
        radioBtnAll->setChecked(true);

        gridLayout_3->addWidget(radioBtnAll, 0, 0, 1, 1);

        radioBtnNoAll = new QRadioButton(groupBox);
        radioBtnNoAll->setObjectName(QString::fromUtf8("radioBtnNoAll"));

        gridLayout_3->addWidget(radioBtnNoAll, 1, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(AreaDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_6 = new QGridLayout(groupBox_2);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        radioBtnB = new QRadioButton(groupBox_2);
        radioBtnB->setObjectName(QString::fromUtf8("radioBtnB"));
        radioBtnB->setChecked(true);

        gridLayout_5->addWidget(radioBtnB, 0, 0, 1, 1);

        radioBtnZ = new QRadioButton(groupBox_2);
        radioBtnZ->setObjectName(QString::fromUtf8("radioBtnZ"));

        gridLayout_5->addWidget(radioBtnZ, 1, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox_3 = new QGroupBox(AreaDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        gridLayout_8 = new QGridLayout(groupBox_3);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setSpacing(6);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_7->addWidget(label, 0, 0, 1, 1);

        cmbMin = new QComboBox(groupBox_3);
        cmbMin->setObjectName(QString::fromUtf8("cmbMin"));

        gridLayout_7->addWidget(cmbMin, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_7->addWidget(label_2, 0, 2, 1, 1);

        cmbMax = new QComboBox(groupBox_3);
        cmbMax->setObjectName(QString::fromUtf8("cmbMax"));

        gridLayout_7->addWidget(cmbMax, 0, 3, 1, 1);


        gridLayout_8->addLayout(gridLayout_7, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_3, 1, 0, 1, 2);

        pushBtnOK = new QPushButton(AreaDialog);
        pushBtnOK->setObjectName(QString::fromUtf8("pushBtnOK"));

        gridLayout->addWidget(pushBtnOK, 2, 0, 1, 1);

        pushBtnExit = new QPushButton(AreaDialog);
        pushBtnExit->setObjectName(QString::fromUtf8("pushBtnExit"));

        gridLayout->addWidget(pushBtnExit, 2, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(AreaDialog);

        QMetaObject::connectSlotsByName(AreaDialog);
    } // setupUi

    void retranslateUi(QDialog *AreaDialog)
    {
        AreaDialog->setWindowTitle(QApplication::translate("AreaDialog", "\351\235\242\347\247\257\347\273\237\350\256\241", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("AreaDialog", "\347\273\237\350\256\241\350\214\203\345\233\264", 0, QApplication::UnicodeUTF8));
        radioBtnAll->setText(QApplication::translate("AreaDialog", "\346\225\264\344\275\223\350\214\203\345\233\264", 0, QApplication::UnicodeUTF8));
        radioBtnNoAll->setText(QApplication::translate("AreaDialog", "\346\214\207\345\256\232\350\214\203\345\233\264", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("AreaDialog", "\347\273\237\350\256\241\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        radioBtnB->setText(QApplication::translate("AreaDialog", "\351\245\274\347\212\266\345\233\276", 0, QApplication::UnicodeUTF8));
        radioBtnZ->setText(QApplication::translate("AreaDialog", "\346\237\261\347\212\266\345\233\276", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("AreaDialog", "\347\273\237\350\256\241\346\235\241\344\273\266", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AreaDialog", "\345\244\247\344\272\216", 0, QApplication::UnicodeUTF8));
        cmbMin->clear();
        cmbMin->insertItems(0, QStringList()
         << QApplication::translate("AreaDialog", "-5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "15", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "20", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "25", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "30", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "35", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "40", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "45", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "50", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "55", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "60", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "65", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "70", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "75", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("AreaDialog", "\345\260\217\344\272\216", 0, QApplication::UnicodeUTF8));
        cmbMax->clear();
        cmbMax->insertItems(0, QStringList()
         << QApplication::translate("AreaDialog", "75", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "70", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "65", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "60", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "55", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "50", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "45", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "40", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "35", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "30", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "25", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "20", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "15", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AreaDialog", "-5", 0, QApplication::UnicodeUTF8)
        );
        pushBtnOK->setText(QApplication::translate("AreaDialog", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        pushBtnExit->setText(QApplication::translate("AreaDialog", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AreaDialog: public Ui_AreaDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AREADIALOG_H
