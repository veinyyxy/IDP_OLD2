/********************************************************************************
** Form generated from reading UI file 'RadarControlDWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RADARCONTROLDWIDGET_H
#define UI_RADARCONTROLDWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RadarControlDockWidget
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout_11;
    QPushButton *pushButton_display_setting;
    QLabel *label_13;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_9;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QComboBox *comboBox_section1_hight;
    QLabel *label_3;
    QComboBox *comboBox_section1_reso;
    QGridLayout *gridLayout_8;
    QLabel *label_iso2_value;
    QSlider *horizontalSlider_iso2;
    QGridLayout *gridLayout_4;
    QLabel *label_iso1_value;
    QSlider *horizontalSlider_iso1;
    QGridLayout *gridLayout_3;
    QLabel *label_4;
    QComboBox *comboBox_section2_hight;
    QLabel *label_5;
    QComboBox *comboBox_section2_reso;
    QCheckBox *checkBox_isosurface2;
    QCheckBox *checkBox_section2;
    QCheckBox *checkBox_section1;
    QCheckBox *checkBox_isosurface1;
    QCheckBox *checkBox_section3;
    QGridLayout *gridLayout_14;
    QLabel *label_6;
    QComboBox *comboBox_section3_hight;
    QLabel *label_12;
    QComboBox *comboBox_section3_reso;
    QComboBox *cmblayerID;
    QSpacerItem *verticalSpacer;

    void setupUi(QDockWidget *RadarControlDockWidget)
    {
        if (RadarControlDockWidget->objectName().isEmpty())
            RadarControlDockWidget->setObjectName(QString::fromUtf8("RadarControlDockWidget"));
        RadarControlDockWidget->resize(282, 601);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout_13 = new QGridLayout(dockWidgetContents);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        gridLayout_11 = new QGridLayout();
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        pushButton_display_setting = new QPushButton(dockWidgetContents);
        pushButton_display_setting->setObjectName(QString::fromUtf8("pushButton_display_setting"));
        pushButton_display_setting->setEnabled(false);

        gridLayout_11->addWidget(pushButton_display_setting, 1, 2, 1, 1);

        label_13 = new QLabel(dockWidgetContents);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMaximumSize(QSize(55, 16777215));

        gridLayout_11->addWidget(label_13, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(dockWidgetContents);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_9 = new QGridLayout(groupBox_2);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setEnabled(false);
        label_2->setMinimumSize(QSize(36, 20));
        label_2->setMaximumSize(QSize(36, 20));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        comboBox_section1_hight = new QComboBox(groupBox_2);
        comboBox_section1_hight->setObjectName(QString::fromUtf8("comboBox_section1_hight"));
        comboBox_section1_hight->setEnabled(false);

        gridLayout_2->addWidget(comboBox_section1_hight, 0, 1, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setEnabled(false);
        label_3->setMaximumSize(QSize(48, 20));

        gridLayout_2->addWidget(label_3, 0, 2, 1, 1);

        comboBox_section1_reso = new QComboBox(groupBox_2);
        comboBox_section1_reso->setObjectName(QString::fromUtf8("comboBox_section1_reso"));
        comboBox_section1_reso->setEnabled(false);

        gridLayout_2->addWidget(comboBox_section1_reso, 0, 3, 1, 1);


        gridLayout_9->addLayout(gridLayout_2, 1, 0, 1, 1);

        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        label_iso2_value = new QLabel(groupBox_2);
        label_iso2_value->setObjectName(QString::fromUtf8("label_iso2_value"));

        gridLayout_8->addWidget(label_iso2_value, 0, 1, 1, 1);

        horizontalSlider_iso2 = new QSlider(groupBox_2);
        horizontalSlider_iso2->setObjectName(QString::fromUtf8("horizontalSlider_iso2"));
        horizontalSlider_iso2->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSlider_iso2, 0, 0, 1, 1);


        gridLayout_9->addLayout(gridLayout_8, 9, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_iso1_value = new QLabel(groupBox_2);
        label_iso1_value->setObjectName(QString::fromUtf8("label_iso1_value"));

        gridLayout_4->addWidget(label_iso1_value, 0, 1, 1, 1);

        horizontalSlider_iso1 = new QSlider(groupBox_2);
        horizontalSlider_iso1->setObjectName(QString::fromUtf8("horizontalSlider_iso1"));
        horizontalSlider_iso1->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_iso1, 0, 0, 1, 1);


        gridLayout_9->addLayout(gridLayout_4, 7, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setEnabled(false);
        label_4->setMaximumSize(QSize(36, 20));

        gridLayout_3->addWidget(label_4, 0, 0, 1, 1);

        comboBox_section2_hight = new QComboBox(groupBox_2);
        comboBox_section2_hight->setObjectName(QString::fromUtf8("comboBox_section2_hight"));
        comboBox_section2_hight->setEnabled(false);

        gridLayout_3->addWidget(comboBox_section2_hight, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setEnabled(false);
        label_5->setMaximumSize(QSize(48, 20));

        gridLayout_3->addWidget(label_5, 0, 2, 1, 1);

        comboBox_section2_reso = new QComboBox(groupBox_2);
        comboBox_section2_reso->setObjectName(QString::fromUtf8("comboBox_section2_reso"));
        comboBox_section2_reso->setEnabled(false);

        gridLayout_3->addWidget(comboBox_section2_reso, 0, 3, 1, 1);


        gridLayout_9->addLayout(gridLayout_3, 3, 0, 1, 1);

        checkBox_isosurface2 = new QCheckBox(groupBox_2);
        checkBox_isosurface2->setObjectName(QString::fromUtf8("checkBox_isosurface2"));

        gridLayout_9->addWidget(checkBox_isosurface2, 8, 0, 1, 1);

        checkBox_section2 = new QCheckBox(groupBox_2);
        checkBox_section2->setObjectName(QString::fromUtf8("checkBox_section2"));

        gridLayout_9->addWidget(checkBox_section2, 2, 0, 1, 1);

        checkBox_section1 = new QCheckBox(groupBox_2);
        checkBox_section1->setObjectName(QString::fromUtf8("checkBox_section1"));

        gridLayout_9->addWidget(checkBox_section1, 0, 0, 1, 1);

        checkBox_isosurface1 = new QCheckBox(groupBox_2);
        checkBox_isosurface1->setObjectName(QString::fromUtf8("checkBox_isosurface1"));

        gridLayout_9->addWidget(checkBox_isosurface1, 6, 0, 1, 1);

        checkBox_section3 = new QCheckBox(groupBox_2);
        checkBox_section3->setObjectName(QString::fromUtf8("checkBox_section3"));

        gridLayout_9->addWidget(checkBox_section3, 4, 0, 1, 1);

        gridLayout_14 = new QGridLayout();
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setEnabled(false);
        label_6->setMaximumSize(QSize(36, 20));

        gridLayout_14->addWidget(label_6, 0, 0, 1, 1);

        comboBox_section3_hight = new QComboBox(groupBox_2);
        comboBox_section3_hight->setObjectName(QString::fromUtf8("comboBox_section3_hight"));
        comboBox_section3_hight->setEnabled(false);

        gridLayout_14->addWidget(comboBox_section3_hight, 0, 1, 1, 1);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setEnabled(false);
        label_12->setMaximumSize(QSize(48, 20));

        gridLayout_14->addWidget(label_12, 0, 2, 1, 1);

        comboBox_section3_reso = new QComboBox(groupBox_2);
        comboBox_section3_reso->setObjectName(QString::fromUtf8("comboBox_section3_reso"));
        comboBox_section3_reso->setEnabled(false);

        gridLayout_14->addWidget(comboBox_section3_reso, 0, 3, 1, 1);


        gridLayout_9->addLayout(gridLayout_14, 5, 0, 1, 1);


        gridLayout_11->addWidget(groupBox_2, 2, 0, 1, 3);

        cmblayerID = new QComboBox(dockWidgetContents);
        cmblayerID->setObjectName(QString::fromUtf8("cmblayerID"));

        gridLayout_11->addWidget(cmblayerID, 1, 1, 1, 1);


        gridLayout_13->addLayout(gridLayout_11, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_13->addItem(verticalSpacer, 1, 0, 1, 1);

        RadarControlDockWidget->setWidget(dockWidgetContents);

        retranslateUi(RadarControlDockWidget);

        QMetaObject::connectSlotsByName(RadarControlDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *RadarControlDockWidget)
    {
        RadarControlDockWidget->setWindowTitle(QApplication::translate("RadarControlDockWidget", "\351\233\267\350\276\276\346\216\247\345\210\266", 0, QApplication::UnicodeUTF8));
        pushButton_display_setting->setText(QApplication::translate("RadarControlDockWidget", "\346\230\276\347\244\272\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("RadarControlDockWidget", "\344\273\260\350\247\222\345\261\202\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("RadarControlDockWidget", "\345\211\226\351\235\242\343\200\201\347\255\211\345\200\274\351\235\242", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RadarControlDockWidget", "\351\253\230\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("RadarControlDockWidget", "\345\210\206\350\276\250\347\216\207\357\274\232", 0, QApplication::UnicodeUTF8));
        label_iso2_value->setText(QApplication::translate("RadarControlDockWidget", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_iso1_value->setText(QApplication::translate("RadarControlDockWidget", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("RadarControlDockWidget", "\351\253\230\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("RadarControlDockWidget", "\345\210\206\350\276\250\347\216\207\357\274\232", 0, QApplication::UnicodeUTF8));
        checkBox_isosurface2->setText(QApplication::translate("RadarControlDockWidget", "\347\255\211\345\200\274\351\235\2422", 0, QApplication::UnicodeUTF8));
        checkBox_section2->setText(QApplication::translate("RadarControlDockWidget", "\346\267\273\345\212\240\345\211\226\351\235\2422", 0, QApplication::UnicodeUTF8));
        checkBox_section1->setText(QApplication::translate("RadarControlDockWidget", "\346\267\273\345\212\240\345\211\226\351\235\2421", 0, QApplication::UnicodeUTF8));
        checkBox_isosurface1->setText(QApplication::translate("RadarControlDockWidget", "\347\255\211\345\200\274\351\235\2421", 0, QApplication::UnicodeUTF8));
        checkBox_section3->setText(QApplication::translate("RadarControlDockWidget", "\346\267\273\345\212\240\345\211\226\351\235\2423", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("RadarControlDockWidget", "\351\253\230\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("RadarControlDockWidget", "\345\210\206\350\276\250\347\216\207\357\274\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RadarControlDockWidget: public Ui_RadarControlDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RADARCONTROLDWIDGET_H
