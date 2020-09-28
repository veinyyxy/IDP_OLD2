/********************************************************************************
** Form generated from reading UI file 'AnimaticWidow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANIMATICWIDOW_H
#define UI_ANIMATICWIDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AnimaticWidow
{
public:
    QAction *actionAsdfasd;
    QWidget *centralWidget;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_10;
    QGridLayout *gridLayout;
    QComboBox *comboBox_fileforplay;
    QLabel *label_13;
    QComboBox *comboBox_10;
    QLabel *label;
    QTableWidget *tableWidget_filelist;
    QPushButton *pushButtonSelectDir;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonClearList;

    void setupUi(QMainWindow *AnimaticWidow)
    {
        if (AnimaticWidow->objectName().isEmpty())
            AnimaticWidow->setObjectName(QString::fromUtf8("AnimaticWidow"));
        AnimaticWidow->resize(731, 501);
        actionAsdfasd = new QAction(AnimaticWidow);
        actionAsdfasd->setObjectName(QString::fromUtf8("actionAsdfasd"));
        centralWidget = new QWidget(AnimaticWidow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        AnimaticWidow->setCentralWidget(centralWidget);
        dockWidget = new QDockWidget(AnimaticWidow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setFloating(false);
        dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dockWidget->setAllowedAreas(Qt::NoDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout_2 = new QGridLayout(dockWidgetContents);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_10 = new QGridLayout(groupBox);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_10->setContentsMargins(-1, -1, -1, 7);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        comboBox_fileforplay = new QComboBox(groupBox);
        comboBox_fileforplay->setObjectName(QString::fromUtf8("comboBox_fileforplay"));

        gridLayout->addWidget(comboBox_fileforplay, 1, 2, 1, 2);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 0, 0, 1, 2);

        comboBox_10 = new QComboBox(groupBox);
        comboBox_10->setObjectName(QString::fromUtf8("comboBox_10"));

        gridLayout->addWidget(comboBox_10, 0, 2, 1, 2);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 2);


        gridLayout_10->addLayout(gridLayout, 2, 0, 1, 3);

        tableWidget_filelist = new QTableWidget(groupBox);
        if (tableWidget_filelist->columnCount() < 1)
            tableWidget_filelist->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_filelist->setHorizontalHeaderItem(0, __qtablewidgetitem);
        tableWidget_filelist->setObjectName(QString::fromUtf8("tableWidget_filelist"));

        gridLayout_10->addWidget(tableWidget_filelist, 3, 0, 1, 3);

        pushButtonSelectDir = new QPushButton(groupBox);
        pushButtonSelectDir->setObjectName(QString::fromUtf8("pushButtonSelectDir"));

        gridLayout_10->addWidget(pushButtonSelectDir, 4, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_10->addItem(horizontalSpacer, 4, 0, 1, 1);

        pushButtonClearList = new QPushButton(groupBox);
        pushButtonClearList->setObjectName(QString::fromUtf8("pushButtonClearList"));

        gridLayout_10->addWidget(pushButtonClearList, 4, 1, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        dockWidget->setWidget(dockWidgetContents);
        AnimaticWidow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);

        retranslateUi(AnimaticWidow);

        QMetaObject::connectSlotsByName(AnimaticWidow);
    } // setupUi

    void retranslateUi(QMainWindow *AnimaticWidow)
    {
        AnimaticWidow->setWindowTitle(QApplication::translate("AnimaticWidow", "AnimaticWidow", 0, QApplication::UnicodeUTF8));
        actionAsdfasd->setText(QApplication::translate("AnimaticWidow", "asdfasd", 0, QApplication::UnicodeUTF8));
        dockWidget->setWindowTitle(QApplication::translate("AnimaticWidow", "\345\212\250\347\224\273\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("AnimaticWidow", "\345\212\250\347\224\273", 0, QApplication::UnicodeUTF8));
        comboBox_fileforplay->clear();
        comboBox_fileforplay->insertItems(0, QStringList()
         << QApplication::translate("AnimaticWidow", "\346\211\200\346\234\211\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AnimaticWidow", "\346\234\200\350\277\2215\344\270\252\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AnimaticWidow", "\346\234\200\350\277\22110\344\270\252\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AnimaticWidow", "\346\234\200\350\277\22115\344\270\252\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AnimaticWidow", "\346\234\200\350\277\22120\344\270\252\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8)
        );
        label_13->setText(QApplication::translate("AnimaticWidow", "\344\273\260\350\247\222\345\261\202\357\274\232", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AnimaticWidow", "\345\276\252\347\216\257\346\222\255\346\224\276\357\274\232", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_filelist->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AnimaticWidow", "\346\226\207\344\273\266\345\220\215", 0, QApplication::UnicodeUTF8));
        pushButtonSelectDir->setText(QApplication::translate("AnimaticWidow", "\346\265\217\350\247\210", 0, QApplication::UnicodeUTF8));
        pushButtonClearList->setText(QApplication::translate("AnimaticWidow", "\346\270\205\347\251\272", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AnimaticWidow: public Ui_AnimaticWidow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANIMATICWIDOW_H
