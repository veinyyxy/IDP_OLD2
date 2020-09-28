/********************************************************************************
** Form generated from reading UI file 'ConfigDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGDIALOG_H
#define UI_CONFIGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigDialog
{
public:
    QGridLayout *gridLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *skinWidget;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout;
    QListWidget *skinListWidget;
    QLabel *label;
    QLabel *label_3;
    QListWidget *qssListWidget;
    QWidget *dataWidget;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_6;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_5;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QSpacerItem *verticalSpacer;
    QWidget *dispWidget;
    QGridLayout *gridLayout_9;
    QGridLayout *gridLayout_8;
    QLabel *label_9;
    QListWidget *listWidget_2;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *exitButton;
    QListWidget *leftListWidget;
    QPushButton *useButton;

    void setupUi(QDialog *ConfigDialog)
    {
        if (ConfigDialog->objectName().isEmpty())
            ConfigDialog->setObjectName(QString::fromUtf8("ConfigDialog"));
        ConfigDialog->resize(432, 452);
        ConfigDialog->setMinimumSize(QSize(432, 452));
        ConfigDialog->setMaximumSize(QSize(432, 452));
        gridLayout_2 = new QGridLayout(ConfigDialog);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        stackedWidget = new QStackedWidget(ConfigDialog);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        skinWidget = new QWidget();
        skinWidget->setObjectName(QString::fromUtf8("skinWidget"));
        gridLayout_3 = new QGridLayout(skinWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        skinListWidget = new QListWidget(skinWidget);
        skinListWidget->setObjectName(QString::fromUtf8("skinListWidget"));

        gridLayout->addWidget(skinListWidget, 0, 1, 1, 1);

        label = new QLabel(skinWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(skinWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        qssListWidget = new QListWidget(skinWidget);
        qssListWidget->setObjectName(QString::fromUtf8("qssListWidget"));

        gridLayout->addWidget(qssListWidget, 1, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 1);

        stackedWidget->addWidget(skinWidget);
        dataWidget = new QWidget();
        dataWidget->setObjectName(QString::fromUtf8("dataWidget"));
        gridLayout_5 = new QGridLayout(dataWidget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        groupBox = new QGroupBox(dataWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_7 = new QGridLayout(groupBox);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_6->addWidget(label_4, 0, 0, 1, 1);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout_6->addWidget(lineEdit, 0, 1, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_6->addWidget(label_6, 1, 0, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_6->addWidget(label_7, 2, 0, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_6->addWidget(label_8, 3, 0, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_6->addWidget(label_5, 4, 0, 1, 1);

        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout_6->addWidget(lineEdit_2, 1, 1, 1, 1);

        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        gridLayout_6->addWidget(lineEdit_3, 2, 1, 1, 1);

        lineEdit_4 = new QLineEdit(groupBox);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        gridLayout_6->addWidget(lineEdit_4, 3, 1, 1, 1);

        lineEdit_5 = new QLineEdit(groupBox);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        gridLayout_6->addWidget(lineEdit_5, 4, 1, 1, 1);


        gridLayout_7->addLayout(gridLayout_6, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupBox, 5, 1, 1, 2);


        gridLayout_5->addLayout(gridLayout_4, 0, 0, 1, 1);

        stackedWidget->addWidget(dataWidget);
        dispWidget = new QWidget();
        dispWidget->setObjectName(QString::fromUtf8("dispWidget"));
        gridLayout_9 = new QGridLayout(dispWidget);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setSpacing(6);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        label_9 = new QLabel(dispWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_8->addWidget(label_9, 0, 0, 1, 1);

        listWidget_2 = new QListWidget(dispWidget);
        new QListWidgetItem(listWidget_2);
        new QListWidgetItem(listWidget_2);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));

        gridLayout_8->addWidget(listWidget_2, 0, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_2, 1, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_8, 0, 0, 1, 1);

        stackedWidget->addWidget(dispWidget);

        gridLayout_2->addWidget(stackedWidget, 0, 1, 1, 5);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 1, 1, 1);

        exitButton = new QPushButton(ConfigDialog);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));

        gridLayout_2->addWidget(exitButton, 1, 3, 1, 1);

        leftListWidget = new QListWidget(ConfigDialog);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/defaultToolBar/skin.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(leftListWidget);
        __qlistwidgetitem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qlistwidgetitem->setIcon(icon);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/defaultToolBar/data.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(leftListWidget);
        __qlistwidgetitem1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qlistwidgetitem1->setIcon(icon1);
        leftListWidget->setObjectName(QString::fromUtf8("leftListWidget"));
        leftListWidget->setMinimumSize(QSize(128, 0));
        leftListWidget->setMaximumSize(QSize(128, 16777215));
        leftListWidget->setIconSize(QSize(96, 84));
        leftListWidget->setTextElideMode(Qt::ElideMiddle);
        leftListWidget->setMovement(QListView::Static);
        leftListWidget->setFlow(QListView::LeftToRight);
        leftListWidget->setSpacing(12);
        leftListWidget->setViewMode(QListView::IconMode);

        gridLayout_2->addWidget(leftListWidget, 0, 0, 2, 1);

        useButton = new QPushButton(ConfigDialog);
        useButton->setObjectName(QString::fromUtf8("useButton"));

        gridLayout_2->addWidget(useButton, 1, 2, 1, 1);


        retranslateUi(ConfigDialog);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigDialog)
    {
        ConfigDialog->setWindowTitle(QApplication::translate("ConfigDialog", "\347\263\273\347\273\237\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ConfigDialog", "\347\263\273\347\273\237\346\240\267\345\274\217\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ConfigDialog", "\347\263\273\347\273\237\347\232\256\350\202\244\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ConfigDialog", "\346\225\260\346\215\256\345\272\223\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ConfigDialog", "\346\225\260\346\215\256\345\272\223\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ConfigDialog", "\346\225\260\346\215\256\345\272\223IP\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ConfigDialog", "\346\225\260\346\215\256\345\272\223\347\253\257\345\217\243\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("ConfigDialog", "\346\225\260\346\215\256\345\272\223\347\224\250\346\210\267\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ConfigDialog", "\346\225\260\346\215\256\345\272\223\345\257\206\347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("ConfigDialog", "\345\220\257\345\212\250\346\230\276\347\244\272\357\274\232", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = listWidget_2->isSortingEnabled();
        listWidget_2->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget_2->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("ConfigDialog", "\350\257\204\344\273\267", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = listWidget_2->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("ConfigDialog", "\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        listWidget_2->setSortingEnabled(__sortingEnabled);

        exitButton->setText(QApplication::translate("ConfigDialog", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled1 = leftListWidget->isSortingEnabled();
        leftListWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem2 = leftListWidget->item(0);
        ___qlistwidgetitem2->setText(QApplication::translate("ConfigDialog", "\347\263\273\347\273\237\351\243\216\346\240\274", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = leftListWidget->item(1);
        ___qlistwidgetitem3->setText(QApplication::translate("ConfigDialog", "\346\225\260\346\215\256\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        leftListWidget->setSortingEnabled(__sortingEnabled1);

        useButton->setText(QApplication::translate("ConfigDialog", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConfigDialog: public Ui_ConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGDIALOG_H
