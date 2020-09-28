/********************************************************************************
** Form generated from reading UI file 'DBDockWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBDOCKWIDGET_H
#define UI_DBDOCKWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DBDockWidget
{
public:
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *SingleTab;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QCheckBox *chb_day_s;
    QComboBox *cmb_day_s;
    QCheckBox *chb_hour_s;
    QComboBox *cmb_hour_s;
    QTableWidget *tableWidget_s;
    QComboBox *cmbName_s;
    QComboBox *cmbAre_s;
    QDateTimeEdit *dateTimeEdit_s;
    QWidget *MosaicTab;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QLabel *label_4;
    QLabel *label_5;
    QCheckBox *chb_day_m;
    QComboBox *cmb_day_m;
    QCheckBox *chb_hour_m;
    QComboBox *cmb_hour_m;
    QTableWidget *tableWidget_m;
    QComboBox *cmbName_m;
    QDateTimeEdit *dateTimeEdit_m;

    void setupUi(QDockWidget *DBDockWidget)
    {
        if (DBDockWidget->objectName().isEmpty())
            DBDockWidget->setObjectName(QString::fromUtf8("DBDockWidget"));
        DBDockWidget->resize(344, 568);
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        SingleTab = new QWidget();
        SingleTab->setObjectName(QString::fromUtf8("SingleTab"));
        gridLayout_4 = new QGridLayout(SingleTab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label = new QLabel(SingleTab);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(SingleTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(SingleTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 2, 0, 1, 1);

        chb_day_s = new QCheckBox(SingleTab);
        chb_day_s->setObjectName(QString::fromUtf8("chb_day_s"));

        gridLayout_3->addWidget(chb_day_s, 3, 0, 1, 1);

        cmb_day_s = new QComboBox(SingleTab);
        cmb_day_s->setObjectName(QString::fromUtf8("cmb_day_s"));

        gridLayout_3->addWidget(cmb_day_s, 3, 1, 1, 1);

        chb_hour_s = new QCheckBox(SingleTab);
        chb_hour_s->setObjectName(QString::fromUtf8("chb_hour_s"));

        gridLayout_3->addWidget(chb_hour_s, 3, 2, 1, 1);

        cmb_hour_s = new QComboBox(SingleTab);
        cmb_hour_s->setObjectName(QString::fromUtf8("cmb_hour_s"));

        gridLayout_3->addWidget(cmb_hour_s, 3, 3, 1, 1);

        tableWidget_s = new QTableWidget(SingleTab);
        if (tableWidget_s->columnCount() < 2)
            tableWidget_s->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_s->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_s->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget_s->setObjectName(QString::fromUtf8("tableWidget_s"));

        gridLayout_3->addWidget(tableWidget_s, 4, 0, 1, 4);

        cmbName_s = new QComboBox(SingleTab);
        cmbName_s->setObjectName(QString::fromUtf8("cmbName_s"));

        gridLayout_3->addWidget(cmbName_s, 0, 1, 1, 3);

        cmbAre_s = new QComboBox(SingleTab);
        cmbAre_s->setObjectName(QString::fromUtf8("cmbAre_s"));

        gridLayout_3->addWidget(cmbAre_s, 1, 1, 1, 3);

        dateTimeEdit_s = new QDateTimeEdit(SingleTab);
        dateTimeEdit_s->setObjectName(QString::fromUtf8("dateTimeEdit_s"));

        gridLayout_3->addWidget(dateTimeEdit_s, 2, 1, 1, 3);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);

        tabWidget->addTab(SingleTab, QString());
        MosaicTab = new QWidget();
        MosaicTab->setObjectName(QString::fromUtf8("MosaicTab"));
        gridLayout_6 = new QGridLayout(MosaicTab);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_4 = new QLabel(MosaicTab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_5->addWidget(label_4, 0, 0, 1, 1);

        label_5 = new QLabel(MosaicTab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_5->addWidget(label_5, 1, 0, 1, 1);

        chb_day_m = new QCheckBox(MosaicTab);
        chb_day_m->setObjectName(QString::fromUtf8("chb_day_m"));

        gridLayout_5->addWidget(chb_day_m, 2, 0, 1, 1);

        cmb_day_m = new QComboBox(MosaicTab);
        cmb_day_m->setObjectName(QString::fromUtf8("cmb_day_m"));

        gridLayout_5->addWidget(cmb_day_m, 2, 1, 1, 1);

        chb_hour_m = new QCheckBox(MosaicTab);
        chb_hour_m->setObjectName(QString::fromUtf8("chb_hour_m"));

        gridLayout_5->addWidget(chb_hour_m, 2, 2, 1, 1);

        cmb_hour_m = new QComboBox(MosaicTab);
        cmb_hour_m->setObjectName(QString::fromUtf8("cmb_hour_m"));

        gridLayout_5->addWidget(cmb_hour_m, 2, 3, 1, 1);

        tableWidget_m = new QTableWidget(MosaicTab);
        if (tableWidget_m->columnCount() < 2)
            tableWidget_m->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_m->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_m->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        tableWidget_m->setObjectName(QString::fromUtf8("tableWidget_m"));

        gridLayout_5->addWidget(tableWidget_m, 3, 0, 1, 4);

        cmbName_m = new QComboBox(MosaicTab);
        cmbName_m->setObjectName(QString::fromUtf8("cmbName_m"));

        gridLayout_5->addWidget(cmbName_m, 0, 1, 1, 3);

        dateTimeEdit_m = new QDateTimeEdit(MosaicTab);
        dateTimeEdit_m->setObjectName(QString::fromUtf8("dateTimeEdit_m"));

        gridLayout_5->addWidget(dateTimeEdit_m, 1, 1, 1, 3);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        tabWidget->addTab(MosaicTab, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        DBDockWidget->setWidget(widget);

        retranslateUi(DBDockWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DBDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *DBDockWidget)
    {
        DBDockWidget->setWindowTitle(QApplication::translate("DBDockWidget", "\346\225\260\346\215\256\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DBDockWidget", "\344\272\247\345\223\201\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DBDockWidget", "\344\272\247\345\223\201\345\214\272\345\237\237\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DBDockWidget", "\351\200\211\346\213\251\346\227\245\346\234\237\357\274\232", 0, QApplication::UnicodeUTF8));
        chb_day_s->setText(QApplication::translate("DBDockWidget", "\346\227\245", 0, QApplication::UnicodeUTF8));
        cmb_day_s->clear();
        cmb_day_s->insertItems(0, QStringList()
         << QApplication::translate("DBDockWidget", "1\345\244\251\344\273\245\345\206\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DBDockWidget", "2\345\244\251\344\273\245\345\206\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DBDockWidget", "3\345\244\251\344\273\245\345\206\205", 0, QApplication::UnicodeUTF8)
        );
        chb_hour_s->setText(QApplication::translate("DBDockWidget", "\346\227\266", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_s->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DBDockWidget", "\346\227\245\346\234\237", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_s->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("DBDockWidget", "\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(SingleTab), QApplication::translate("DBDockWidget", "\345\215\225\347\253\231\344\272\247\345\223\201", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DBDockWidget", "\344\272\247\345\223\201\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DBDockWidget", "\351\200\211\346\213\251\346\227\245\346\234\237\357\274\232", 0, QApplication::UnicodeUTF8));
        chb_day_m->setText(QApplication::translate("DBDockWidget", "\346\227\245", 0, QApplication::UnicodeUTF8));
        cmb_day_m->clear();
        cmb_day_m->insertItems(0, QStringList()
         << QApplication::translate("DBDockWidget", "1\345\244\251\344\273\245\345\206\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DBDockWidget", "2\345\244\251\344\273\245\345\206\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DBDockWidget", "3\345\244\251\344\273\245\345\206\205", 0, QApplication::UnicodeUTF8)
        );
        chb_hour_m->setText(QApplication::translate("DBDockWidget", "\346\227\266", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_m->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("DBDockWidget", "\346\227\245\346\234\237", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_m->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("DBDockWidget", "\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(MosaicTab), QApplication::translate("DBDockWidget", "\347\273\204\347\275\221\344\272\247\345\223\201", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DBDockWidget: public Ui_DBDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBDOCKWIDGET_H
