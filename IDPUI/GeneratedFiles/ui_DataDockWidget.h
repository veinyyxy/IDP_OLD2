/********************************************************************************
** Form generated from reading UI file 'DataDockWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATADOCKWIDGET_H
#define UI_DATADOCKWIDGET_H

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
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataDockWidget
{
public:
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *SingleTab;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QLineEdit *editSavePath;
    QLineEdit *editBrowserPath;
    QLabel *label;
    QPushButton *btnBrowser;
    QListView *listView;
    QPushButton *btnSave;
    QLabel *label_3;
    QLabel *label_6;
    QComboBox *RoseArithmsBox;
    QRadioButton *SelQcRadioBtn;
    QPushButton *GenerateButton;
    QComboBox *RadSiteNameComBox;
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

    void setupUi(QDockWidget *DataDockWidget)
    {
        if (DataDockWidget->objectName().isEmpty())
            DataDockWidget->setObjectName(QString::fromUtf8("DataDockWidget"));
        DataDockWidget->resize(300, 568);
        DataDockWidget->setMinimumSize(QSize(300, 310));
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
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
        gridLayout_4->setContentsMargins(3, 3, 3, 3);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        editSavePath = new QLineEdit(SingleTab);
        editSavePath->setObjectName(QString::fromUtf8("editSavePath"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(editSavePath->sizePolicy().hasHeightForWidth());
        editSavePath->setSizePolicy(sizePolicy);
        editSavePath->setReadOnly(true);

        gridLayout_3->addWidget(editSavePath, 5, 1, 1, 3);

        editBrowserPath = new QLineEdit(SingleTab);
        editBrowserPath->setObjectName(QString::fromUtf8("editBrowserPath"));
        sizePolicy.setHeightForWidth(editBrowserPath->sizePolicy().hasHeightForWidth());
        editBrowserPath->setSizePolicy(sizePolicy);
        editBrowserPath->setReadOnly(true);

        gridLayout_3->addWidget(editBrowserPath, 6, 1, 1, 3);

        label = new QLabel(SingleTab);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        btnBrowser = new QPushButton(SingleTab);
        btnBrowser->setObjectName(QString::fromUtf8("btnBrowser"));

        gridLayout_3->addWidget(btnBrowser, 6, 0, 1, 1);

        listView = new QListView(SingleTab);
        listView->setObjectName(QString::fromUtf8("listView"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(listView, 7, 0, 1, 4);

        btnSave = new QPushButton(SingleTab);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));

        gridLayout_3->addWidget(btnSave, 5, 0, 1, 1);

        label_3 = new QLabel(SingleTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(label_3, 3, 0, 1, 1);

        label_6 = new QLabel(SingleTab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(label_6, 1, 0, 1, 1);

        RoseArithmsBox = new QComboBox(SingleTab);
        RoseArithmsBox->setObjectName(QString::fromUtf8("RoseArithmsBox"));
        sizePolicy.setHeightForWidth(RoseArithmsBox->sizePolicy().hasHeightForWidth());
        RoseArithmsBox->setSizePolicy(sizePolicy);
        RoseArithmsBox->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_3->addWidget(RoseArithmsBox, 1, 1, 1, 3);

        SelQcRadioBtn = new QRadioButton(SingleTab);
        SelQcRadioBtn->setObjectName(QString::fromUtf8("SelQcRadioBtn"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(SelQcRadioBtn->sizePolicy().hasHeightForWidth());
        SelQcRadioBtn->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(SelQcRadioBtn, 3, 1, 1, 3);

        GenerateButton = new QPushButton(SingleTab);
        GenerateButton->setObjectName(QString::fromUtf8("GenerateButton"));

        gridLayout_3->addWidget(GenerateButton, 8, 3, 1, 1);

        RadSiteNameComBox = new QComboBox(SingleTab);
        RadSiteNameComBox->setObjectName(QString::fromUtf8("RadSiteNameComBox"));
        sizePolicy.setHeightForWidth(RadSiteNameComBox->sizePolicy().hasHeightForWidth());
        RadSiteNameComBox->setSizePolicy(sizePolicy);
        RadSiteNameComBox->setMaximumSize(QSize(16777215, 16777215));
        RadSiteNameComBox->setEditable(true);

        gridLayout_3->addWidget(RadSiteNameComBox, 0, 1, 1, 3);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);

        tabWidget->addTab(SingleTab, QString());
        MosaicTab = new QWidget();
        MosaicTab->setObjectName(QString::fromUtf8("MosaicTab"));
        gridLayout_6 = new QGridLayout(MosaicTab);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(3, 3, 3, 3);
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
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_m->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_m->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget_m->setObjectName(QString::fromUtf8("tableWidget_m"));
        sizePolicy2.setHeightForWidth(tableWidget_m->sizePolicy().hasHeightForWidth());
        tableWidget_m->setSizePolicy(sizePolicy2);

        gridLayout_5->addWidget(tableWidget_m, 3, 0, 1, 4);

        cmbName_m = new QComboBox(MosaicTab);
        cmbName_m->setObjectName(QString::fromUtf8("cmbName_m"));

        gridLayout_5->addWidget(cmbName_m, 0, 1, 1, 3);

        dateTimeEdit_m = new QDateTimeEdit(MosaicTab);
        dateTimeEdit_m->setObjectName(QString::fromUtf8("dateTimeEdit_m"));
        sizePolicy.setHeightForWidth(dateTimeEdit_m->sizePolicy().hasHeightForWidth());
        dateTimeEdit_m->setSizePolicy(sizePolicy);

        gridLayout_5->addWidget(dateTimeEdit_m, 1, 1, 1, 3);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        tabWidget->addTab(MosaicTab, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        DataDockWidget->setWidget(widget);

        retranslateUi(DataDockWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DataDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *DataDockWidget)
    {
        DataDockWidget->setWindowTitle(QApplication::translate("DataDockWidget", "\345\237\272\346\225\260\346\215\256\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DataDockWidget", "\351\233\267\350\276\276\347\253\231\347\202\271\357\274\232", 0, QApplication::UnicodeUTF8));
        btnBrowser->setText(QApplication::translate("DataDockWidget", "\346\226\207\344\273\266\350\267\257\345\276\204", 0, QApplication::UnicodeUTF8));
        btnSave->setText(QApplication::translate("DataDockWidget", "\347\224\237\346\210\220\350\267\257\345\276\204", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DataDockWidget", "\346\230\257\345\220\246\350\264\250\346\216\247\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DataDockWidget", "\344\272\247\345\223\201\347\261\273\345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        RoseArithmsBox->clear();
        RoseArithmsBox->insertItems(0, QStringList()
         << QApplication::translate("DataDockWidget", "Arith_251_R", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_252_QR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_254_V", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_255_QV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_256_W", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_257_QW", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_258_CAP", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_262_HBR_270_QPR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_263_CR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_264_CRH", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_265_ET", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_266_VIL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_267_VILD", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_271_OHP", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_273_THP", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_274_SHP", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_277_SCI", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_285_EVP_269_VWP_268_VAD", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_291_HI", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Arith_292_M", 0, QApplication::UnicodeUTF8)
        );
        SelQcRadioBtn->setText(QApplication::translate("DataDockWidget", "\350\264\250\351\207\217\346\216\247\345\210\266", 0, QApplication::UnicodeUTF8));
        GenerateButton->setText(QApplication::translate("DataDockWidget", "\347\224\237\346\210\220", 0, QApplication::UnicodeUTF8));
        RadSiteNameComBox->clear();
        RadSiteNameComBox->insertItems(0, QStringList()
         << QApplication::translate("DataDockWidget", "Z9431", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Z9436", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Z9439", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "Z9591", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(SingleTab), QApplication::translate("DataDockWidget", "\345\215\225\347\253\231\344\272\247\345\223\201", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DataDockWidget", "\344\272\247\345\223\201\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DataDockWidget", "\351\200\211\346\213\251\346\227\245\346\234\237\357\274\232", 0, QApplication::UnicodeUTF8));
        chb_day_m->setText(QApplication::translate("DataDockWidget", "\346\227\245", 0, QApplication::UnicodeUTF8));
        cmb_day_m->clear();
        cmb_day_m->insertItems(0, QStringList()
         << QApplication::translate("DataDockWidget", "1\345\244\251\344\273\245\345\206\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "2\345\244\251\344\273\245\345\206\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataDockWidget", "3\345\244\251\344\273\245\345\206\205", 0, QApplication::UnicodeUTF8)
        );
        chb_hour_m->setText(QApplication::translate("DataDockWidget", "\346\227\266", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_m->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DataDockWidget", "\346\227\245\346\234\237", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_m->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("DataDockWidget", "\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(MosaicTab), QApplication::translate("DataDockWidget", "\347\273\204\347\275\221\344\272\247\345\223\201", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DataDockWidget: public Ui_DataDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATADOCKWIDGET_H
