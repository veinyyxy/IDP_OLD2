/********************************************************************************
** Form generated from reading UI file 'DisplaySettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYSETTINGSDIALOG_H
#define UI_DISPLAYSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DisplaySettingsDialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QStackedWidget *stackedWidget;
    QWidget *page1;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_4;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QCheckBox *checkBox_2;
    QWidget *page2;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_6;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout_12;
    QLabel *label_2;
    QFrame *frame_2;
    QPushButton *bgPushButton;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QPushButton *pushButton_3;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_4;
    QLabel *label_5;
    QListWidget *listWidget_2;
    QWidget *page3;
    QGridLayout *gridLayout_9;
    QGridLayout *gridLayout_8;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_15;
    QGridLayout *gridLayout_14;
    QLabel *label_6;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_7;
    QCheckBox *checkBox;
    QLineEdit *lineEdit_3;
    QWidget *page4;
    QGridLayout *gridLayout_11;
    QGridLayout *gridLayout_10;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_17;
    QGridLayout *gridLayout_16;
    QLabel *label_8;
    QSpacerItem *verticalSpacer_4;
    QLineEdit *lineEdit_4;
    QLabel *label_9;
    QLineEdit *lineEdit_5;
    QWidget *page5;
    QGridLayout *gridLayout_19;
    QGridLayout *gridLayout_18;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_21;
    QGridLayout *gridLayout_20;
    QLabel *label_10;
    QSpinBox *spinBox;
    QSpacerItem *verticalSpacer_5;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DisplaySettingsDialog)
    {
        if (DisplaySettingsDialog->objectName().isEmpty())
            DisplaySettingsDialog->setObjectName(QString::fromUtf8("DisplaySettingsDialog"));
        DisplaySettingsDialog->resize(587, 417);
        DisplaySettingsDialog->setMinimumSize(QSize(587, 417));
        DisplaySettingsDialog->setMaximumSize(QSize(587, 417));
        gridLayout_2 = new QGridLayout(DisplaySettingsDialog);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listWidget = new QListWidget(DisplaySettingsDialog);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/defaultToolBar/map.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(listWidget);
        __qlistwidgetitem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qlistwidgetitem->setIcon(icon);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(listWidget);
        __qlistwidgetitem1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qlistwidgetitem1->setIcon(icon);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(listWidget);
        __qlistwidgetitem2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qlistwidgetitem2->setIcon(icon);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(listWidget);
        __qlistwidgetitem3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qlistwidgetitem3->setIcon(icon);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(listWidget);
        __qlistwidgetitem4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qlistwidgetitem4->setIcon(icon);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setMinimumSize(QSize(128, 0));
        listWidget->setMaximumSize(QSize(128, 16777215));
        listWidget->setIconSize(QSize(96, 84));
        listWidget->setTextElideMode(Qt::ElideMiddle);
        listWidget->setMovement(QListView::Static);
        listWidget->setFlow(QListView::LeftToRight);
        listWidget->setSpacing(12);
        listWidget->setViewMode(QListView::IconMode);

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);

        stackedWidget = new QStackedWidget(DisplaySettingsDialog);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page1 = new QWidget();
        page1->setObjectName(QString::fromUtf8("page1"));
        gridLayout_3 = new QGridLayout(page1);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(page1);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_5 = new QGridLayout(groupBox);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_4->addWidget(label, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 1, 1, 1, 1);

        checkBox_2 = new QCheckBox(groupBox);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        gridLayout_4->addWidget(checkBox_2, 0, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        stackedWidget->addWidget(page1);
        page2 = new QWidget();
        page2->setObjectName(QString::fromUtf8("page2"));
        gridLayout_7 = new QGridLayout(page2);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        groupBox_2 = new QGroupBox(page2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_13 = new QGridLayout(groupBox_2);
        gridLayout_13->setSpacing(6);
        gridLayout_13->setContentsMargins(11, 11, 11, 11);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        gridLayout_12 = new QGridLayout();
        gridLayout_12->setSpacing(6);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_12->addWidget(label_2, 0, 0, 1, 1);

        frame_2 = new QFrame(groupBox_2);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush2(QColor(255, 255, 220, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
        frame_2->setPalette(palette);
        frame_2->setStyleSheet(QString::fromUtf8(""));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);

        gridLayout_12->addWidget(frame_2, 0, 1, 1, 1);

        bgPushButton = new QPushButton(groupBox_2);
        bgPushButton->setObjectName(QString::fromUtf8("bgPushButton"));

        gridLayout_12->addWidget(bgPushButton, 0, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_12->addItem(verticalSpacer_2, 4, 0, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_12->addWidget(label_3, 2, 0, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_12->addWidget(label_4, 1, 0, 1, 1);

        lineEdit = new QLineEdit(groupBox_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout_12->addWidget(lineEdit, 1, 1, 1, 1);

        pushButton_3 = new QPushButton(groupBox_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        gridLayout_12->addWidget(pushButton_3, 1, 2, 1, 1);

        lineEdit_2 = new QLineEdit(groupBox_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout_12->addWidget(lineEdit_2, 2, 1, 1, 1);

        pushButton_4 = new QPushButton(groupBox_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        gridLayout_12->addWidget(pushButton_4, 2, 2, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_12->addWidget(label_5, 3, 0, 1, 1);

        listWidget_2 = new QListWidget(groupBox_2);
        QListWidgetItem *__qlistwidgetitem5 = new QListWidgetItem(listWidget_2);
        __qlistwidgetitem5->setCheckState(Qt::Checked);
        QListWidgetItem *__qlistwidgetitem6 = new QListWidgetItem(listWidget_2);
        __qlistwidgetitem6->setCheckState(Qt::Checked);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));
        listWidget_2->setViewMode(QListView::IconMode);

        gridLayout_12->addWidget(listWidget_2, 3, 1, 2, 2);


        gridLayout_13->addLayout(gridLayout_12, 0, 0, 1, 1);


        gridLayout_6->addWidget(groupBox_2, 0, 0, 1, 1);


        gridLayout_7->addLayout(gridLayout_6, 0, 0, 1, 1);

        stackedWidget->addWidget(page2);
        page3 = new QWidget();
        page3->setObjectName(QString::fromUtf8("page3"));
        gridLayout_9 = new QGridLayout(page3);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setSpacing(6);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        groupBox_3 = new QGroupBox(page3);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_15 = new QGridLayout(groupBox_3);
        gridLayout_15->setSpacing(6);
        gridLayout_15->setContentsMargins(11, 11, 11, 11);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        gridLayout_14 = new QGridLayout();
        gridLayout_14->setSpacing(6);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_14->addWidget(label_6, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_14->addItem(verticalSpacer_3, 2, 0, 1, 1);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_14->addWidget(label_7, 1, 0, 1, 1);

        checkBox = new QCheckBox(groupBox_3);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout_14->addWidget(checkBox, 0, 1, 1, 1);

        lineEdit_3 = new QLineEdit(groupBox_3);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        gridLayout_14->addWidget(lineEdit_3, 1, 1, 1, 1);


        gridLayout_15->addLayout(gridLayout_14, 0, 0, 1, 1);


        gridLayout_8->addWidget(groupBox_3, 0, 0, 1, 1);


        gridLayout_9->addLayout(gridLayout_8, 0, 0, 1, 1);

        stackedWidget->addWidget(page3);
        page4 = new QWidget();
        page4->setObjectName(QString::fromUtf8("page4"));
        gridLayout_11 = new QGridLayout(page4);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_10 = new QGridLayout();
        gridLayout_10->setSpacing(6);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        groupBox_4 = new QGroupBox(page4);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_17 = new QGridLayout(groupBox_4);
        gridLayout_17->setSpacing(6);
        gridLayout_17->setContentsMargins(11, 11, 11, 11);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        gridLayout_16 = new QGridLayout();
        gridLayout_16->setSpacing(6);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_16->addWidget(label_8, 0, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_16->addItem(verticalSpacer_4, 2, 0, 1, 1);

        lineEdit_4 = new QLineEdit(groupBox_4);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        gridLayout_16->addWidget(lineEdit_4, 0, 1, 1, 1);

        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_16->addWidget(label_9, 1, 0, 1, 1);

        lineEdit_5 = new QLineEdit(groupBox_4);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        gridLayout_16->addWidget(lineEdit_5, 1, 1, 1, 1);


        gridLayout_17->addLayout(gridLayout_16, 0, 0, 1, 1);


        gridLayout_10->addWidget(groupBox_4, 0, 0, 1, 1);


        gridLayout_11->addLayout(gridLayout_10, 0, 0, 1, 1);

        stackedWidget->addWidget(page4);
        page5 = new QWidget();
        page5->setObjectName(QString::fromUtf8("page5"));
        gridLayout_19 = new QGridLayout(page5);
        gridLayout_19->setSpacing(6);
        gridLayout_19->setContentsMargins(11, 11, 11, 11);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        gridLayout_18 = new QGridLayout();
        gridLayout_18->setSpacing(6);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        groupBox_5 = new QGroupBox(page5);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_21 = new QGridLayout(groupBox_5);
        gridLayout_21->setSpacing(6);
        gridLayout_21->setContentsMargins(11, 11, 11, 11);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        gridLayout_20 = new QGridLayout();
        gridLayout_20->setSpacing(6);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        label_10 = new QLabel(groupBox_5);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_20->addWidget(label_10, 0, 0, 1, 1);

        spinBox = new QSpinBox(groupBox_5);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        gridLayout_20->addWidget(spinBox, 0, 1, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_20->addItem(verticalSpacer_5, 1, 0, 1, 1);


        gridLayout_21->addLayout(gridLayout_20, 0, 0, 1, 1);


        gridLayout_18->addWidget(groupBox_5, 0, 0, 1, 1);


        gridLayout_19->addLayout(gridLayout_18, 0, 0, 1, 1);

        stackedWidget->addWidget(page5);

        gridLayout->addWidget(stackedWidget, 0, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(DisplaySettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(DisplaySettingsDialog);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DisplaySettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *DisplaySettingsDialog)
    {
        DisplaySettingsDialog->setWindowTitle(QApplication::translate("DisplaySettingsDialog", "\346\230\276\347\244\272\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("DisplaySettingsDialog", "\350\247\206\345\217\243\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("DisplaySettingsDialog", "\345\234\260\345\233\276\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("DisplaySettingsDialog", "\347\273\217\347\272\254\347\275\221\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = listWidget->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("DisplaySettingsDialog", "\346\212\225\345\275\261\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem4 = listWidget->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("DisplaySettingsDialog", "\345\212\250\347\224\273\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        listWidget->setSortingEnabled(__sortingEnabled);

        groupBox->setTitle(QApplication::translate("DisplaySettingsDialog", "\350\247\206\345\217\243\345\237\272\347\241\200\350\256\276\347\275\256\357\274\232", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DisplaySettingsDialog", "\346\230\257\345\220\246\345\220\257\345\212\250\346\211\223\345\274\200\357\274\232", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("DisplaySettingsDialog", "\346\230\257", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("DisplaySettingsDialog", "\345\234\260\345\233\276\350\256\276\347\275\256\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DisplaySettingsDialog", "\350\203\214\346\231\257\357\274\232", 0, QApplication::UnicodeUTF8));
        bgPushButton->setText(QApplication::translate("DisplaySettingsDialog", "\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DisplaySettingsDialog", "\344\270\255\345\277\203\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DisplaySettingsDialog", "\350\214\203\345\233\264\357\274\232", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QApplication::translate("DisplaySettingsDialog", "90,180,180,90", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("DisplaySettingsDialog", "\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setText(QApplication::translate("DisplaySettingsDialog", "120.01,98.123", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("DisplaySettingsDialog", "\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DisplaySettingsDialog", "\345\214\272\345\237\237\357\274\232", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled1 = listWidget_2->isSortingEnabled();
        listWidget_2->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem5 = listWidget_2->item(0);
        ___qlistwidgetitem5->setText(QApplication::translate("DisplaySettingsDialog", "\345\214\227\344\272\254", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem6 = listWidget_2->item(1);
        ___qlistwidgetitem6->setText(QApplication::translate("DisplaySettingsDialog", "\346\255\246\346\261\211", 0, QApplication::UnicodeUTF8));
        listWidget_2->setSortingEnabled(__sortingEnabled1);

        groupBox_3->setTitle(QApplication::translate("DisplaySettingsDialog", "\347\273\217\347\272\254\347\275\221\350\256\276\347\275\256\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DisplaySettingsDialog", "\346\230\257\345\220\246\346\230\276\347\244\272\347\273\217\347\272\254\347\275\221\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DisplaySettingsDialog", "\347\273\217\347\272\254\347\275\221\350\214\203\345\233\264\357\274\232", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("DisplaySettingsDialog", "\346\230\257", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("DisplaySettingsDialog", "\346\212\225\345\275\261\350\256\276\347\275\256\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DisplaySettingsDialog", "\345\272\225\345\233\276\351\273\230\350\256\244\346\212\225\345\275\261\357\274\232", 0, QApplication::UnicodeUTF8));
        lineEdit_4->setText(QApplication::translate("DisplaySettingsDialog", "\345\205\260\344\274\257\347\211\271", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("DisplaySettingsDialog", "\346\212\225\345\275\261\344\270\255\345\277\203\357\274\232", 0, QApplication::UnicodeUTF8));
        lineEdit_5->setText(QApplication::translate("DisplaySettingsDialog", "123.0123,32.12", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("DisplaySettingsDialog", "\345\212\250\347\224\273\350\256\276\347\275\256\357\274\232", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("DisplaySettingsDialog", "\346\227\266\351\227\264\351\227\264\351\232\224\357\274\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DisplaySettingsDialog: public Ui_DisplaySettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYSETTINGSDIALOG_H
