/********************************************************************************
** Form generated from reading UI file 'GridSetting.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDSETTING_H
#define UI_GRIDSETTING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_GridSettingDialog
{
public:
    QGridLayout *gridLayout_3;
    QCheckBox *checkBox;
    QGridLayout *gridLayout_14;
    QGridLayout *gridLayout_11;
    QCheckBox *checkBoxText;
    QCheckBox *checkBoxFrame;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditUP;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_4;
    QLabel *label_3;
    QLineEdit *lineEditLeft;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout_5;
    QLineEdit *lineEditRight;
    QLabel *label_4;
    QGridLayout *gridLayout_6;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_2;
    QLineEdit *lineEditDown;
    QSpacerItem *horizontalSpacer_7;
    QGridLayout *gridLayout_12;
    QGridLayout *gridLayout_9;
    QLabel *label_6;
    QLineEdit *lineEditLatResolution;
    QGridLayout *gridLayout_8;
    QLabel *label_5;
    QLineEdit *lineEditLonResolution;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_addGrid;
    QPushButton *pushButton_quit;

    void setupUi(QDialog *GridSettingDialog)
    {
        if (GridSettingDialog->objectName().isEmpty())
            GridSettingDialog->setObjectName(QString::fromUtf8("GridSettingDialog"));
        GridSettingDialog->resize(450, 280);
        GridSettingDialog->setMinimumSize(QSize(280, 250));
        GridSettingDialog->setMaximumSize(QSize(450, 280));
        gridLayout_3 = new QGridLayout(GridSettingDialog);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        checkBox = new QCheckBox(GridSettingDialog);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setEnabled(false);

        gridLayout_3->addWidget(checkBox, 0, 0, 1, 1);

        gridLayout_14 = new QGridLayout();
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        gridLayout_11 = new QGridLayout();
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        checkBoxText = new QCheckBox(GridSettingDialog);
        checkBoxText->setObjectName(QString::fromUtf8("checkBoxText"));
        checkBoxText->setEnabled(false);
        checkBoxText->setChecked(true);

        gridLayout_11->addWidget(checkBoxText, 0, 0, 1, 1);

        checkBoxFrame = new QCheckBox(GridSettingDialog);
        checkBoxFrame->setObjectName(QString::fromUtf8("checkBoxFrame"));
        checkBoxFrame->setEnabled(false);
        checkBoxFrame->setChecked(true);

        gridLayout_11->addWidget(checkBoxFrame, 1, 0, 1, 1);


        gridLayout_14->addLayout(gridLayout_11, 2, 0, 1, 1);

        gridLayout_13 = new QGridLayout();
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(GridSettingDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        lineEditUP = new QLineEdit(GridSettingDialog);
        lineEditUP->setObjectName(QString::fromUtf8("lineEditUP"));

        gridLayout->addWidget(lineEditUP, 1, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 1, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 1, 2, 1, 1);


        gridLayout_13->addLayout(gridLayout, 0, 0, 1, 1);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_3 = new QLabel(GridSettingDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_4->addWidget(label_3, 0, 0, 1, 1);

        lineEditLeft = new QLineEdit(GridSettingDialog);
        lineEditLeft->setObjectName(QString::fromUtf8("lineEditLeft"));

        gridLayout_4->addWidget(lineEditLeft, 0, 1, 1, 1);


        gridLayout_7->addLayout(gridLayout_4, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer, 0, 1, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        lineEditRight = new QLineEdit(GridSettingDialog);
        lineEditRight->setObjectName(QString::fromUtf8("lineEditRight"));

        gridLayout_5->addWidget(lineEditRight, 0, 1, 1, 1);

        label_4 = new QLabel(GridSettingDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_5->addWidget(label_4, 0, 2, 1, 1);


        gridLayout_7->addLayout(gridLayout_5, 0, 2, 1, 1);


        gridLayout_13->addLayout(gridLayout_7, 1, 0, 1, 1);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_6, 1, 0, 1, 1);

        label_2 = new QLabel(GridSettingDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_6->addWidget(label_2, 2, 1, 1, 1);

        lineEditDown = new QLineEdit(GridSettingDialog);
        lineEditDown->setObjectName(QString::fromUtf8("lineEditDown"));

        gridLayout_6->addWidget(lineEditDown, 1, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_7, 1, 2, 1, 1);


        gridLayout_13->addLayout(gridLayout_6, 2, 0, 1, 1);


        gridLayout_14->addLayout(gridLayout_13, 0, 0, 1, 1);

        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_6 = new QLabel(GridSettingDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_9->addWidget(label_6, 0, 0, 1, 1);

        lineEditLatResolution = new QLineEdit(GridSettingDialog);
        lineEditLatResolution->setObjectName(QString::fromUtf8("lineEditLatResolution"));

        gridLayout_9->addWidget(lineEditLatResolution, 0, 1, 1, 1);


        gridLayout_12->addLayout(gridLayout_9, 0, 2, 1, 1);

        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        label_5 = new QLabel(GridSettingDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_8->addWidget(label_5, 0, 0, 1, 1);

        lineEditLonResolution = new QLineEdit(GridSettingDialog);
        lineEditLonResolution->setObjectName(QString::fromUtf8("lineEditLonResolution"));

        gridLayout_8->addWidget(lineEditLonResolution, 0, 1, 1, 1);


        gridLayout_12->addLayout(gridLayout_8, 0, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_12->addItem(horizontalSpacer_3, 0, 1, 1, 1);


        gridLayout_14->addLayout(gridLayout_12, 1, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_14, 1, 0, 1, 2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pushButton_addGrid = new QPushButton(GridSettingDialog);
        pushButton_addGrid->setObjectName(QString::fromUtf8("pushButton_addGrid"));

        gridLayout_2->addWidget(pushButton_addGrid, 0, 0, 1, 1);

        pushButton_quit = new QPushButton(GridSettingDialog);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));

        gridLayout_2->addWidget(pushButton_quit, 0, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 2, 1, 1, 1);


        retranslateUi(GridSettingDialog);

        QMetaObject::connectSlotsByName(GridSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *GridSettingDialog)
    {
        GridSettingDialog->setWindowTitle(QApplication::translate("GridSettingDialog", "\346\267\273\345\212\240\347\273\217\347\272\254\347\275\221\346\240\274", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("GridSettingDialog", "\345\205\250\347\220\203\350\214\203\345\233\264", 0, QApplication::UnicodeUTF8));
        checkBoxText->setText(QApplication::translate("GridSettingDialog", "\345\210\273\345\272\246\346\240\207\346\263\250", 0, QApplication::UnicodeUTF8));
        checkBoxFrame->setText(QApplication::translate("GridSettingDialog", "\346\267\273\345\212\240\347\275\221\346\240\274\350\276\271\346\241\206", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GridSettingDialog", "          \344\270\212", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("GridSettingDialog", "\345\267\246", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("GridSettingDialog", "\345\217\263", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("GridSettingDialog", "           \344\270\213", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("GridSettingDialog", "\347\272\254\345\272\246\345\210\206\350\276\250\347\216\207\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("GridSettingDialog", "\347\273\217\345\272\246\345\210\206\350\276\250\347\216\207\357\274\232", 0, QApplication::UnicodeUTF8));
        pushButton_addGrid->setText(QApplication::translate("GridSettingDialog", "\346\267\273\345\212\240\347\275\221\346\240\274", 0, QApplication::UnicodeUTF8));
        pushButton_quit->setText(QApplication::translate("GridSettingDialog", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GridSettingDialog: public Ui_GridSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDSETTING_H
