/********************************************************************************
** Form generated from reading UI file 'RadarToolBar.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RADARTOOLBAR_H
#define UI_RADARTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>

QT_BEGIN_NAMESPACE

class Ui_RadarToolBar
{
public:
    QAction *actionPlot;
    QAction *actionWin4;
    QAction *actionArea;
    QAction *actionFuzzy;
    QAction *actionDB2;
    QAction *action2D;
    QAction *action3D;
    QAction *actionVolumeRender;
    QAction *actionClipPlane;
    QAction *action_VTK;
    QAction *actionAnimatic;

    void setupUi(QToolBar *RadarToolBar)
    {
        if (RadarToolBar->objectName().isEmpty())
            RadarToolBar->setObjectName(QString::fromUtf8("RadarToolBar"));
        RadarToolBar->resize(706, 87);
        RadarToolBar->setIconSize(QSize(20, 20));
        RadarToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        actionPlot = new QAction(RadarToolBar);
        actionPlot->setObjectName(QString::fromUtf8("actionPlot"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/radarToolBar/pro.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlot->setIcon(icon);
        actionWin4 = new QAction(RadarToolBar);
        actionWin4->setObjectName(QString::fromUtf8("actionWin4"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/radarToolBar/win4.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWin4->setIcon(icon1);
        actionArea = new QAction(RadarToolBar);
        actionArea->setObjectName(QString::fromUtf8("actionArea"));
        actionArea->setEnabled(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/radarToolBar/area.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionArea->setIcon(icon2);
        actionFuzzy = new QAction(RadarToolBar);
        actionFuzzy->setObjectName(QString::fromUtf8("actionFuzzy"));
        actionFuzzy->setCheckable(true);
        actionFuzzy->setEnabled(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/radarToolBar/fuzzy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFuzzy->setIcon(icon3);
        actionDB2 = new QAction(RadarToolBar);
        actionDB2->setObjectName(QString::fromUtf8("actionDB2"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/defaultToolBar/config.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDB2->setIcon(icon4);
        action2D = new QAction(RadarToolBar);
        action2D->setObjectName(QString::fromUtf8("action2D"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/radarToolBar/_2d.png"), QSize(), QIcon::Normal, QIcon::Off);
        action2D->setIcon(icon5);
        action3D = new QAction(RadarToolBar);
        action3D->setObjectName(QString::fromUtf8("action3D"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/radarToolBar/_3d.png"), QSize(), QIcon::Normal, QIcon::Off);
        action3D->setIcon(icon6);
        actionVolumeRender = new QAction(RadarToolBar);
        actionVolumeRender->setObjectName(QString::fromUtf8("actionVolumeRender"));
        actionVolumeRender->setCheckable(true);
        actionVolumeRender->setIcon(icon3);
        actionClipPlane = new QAction(RadarToolBar);
        actionClipPlane->setObjectName(QString::fromUtf8("actionClipPlane"));
        actionClipPlane->setCheckable(true);
        actionClipPlane->setIcon(icon3);
        action_VTK = new QAction(RadarToolBar);
        action_VTK->setObjectName(QString::fromUtf8("action_VTK"));
        action_VTK->setIcon(icon);
        actionAnimatic = new QAction(RadarToolBar);
        actionAnimatic->setObjectName(QString::fromUtf8("actionAnimatic"));
        actionAnimatic->setIcon(icon3);

        retranslateUi(RadarToolBar);

        QMetaObject::connectSlotsByName(RadarToolBar);
    } // setupUi

    void retranslateUi(QToolBar *RadarToolBar)
    {
        RadarToolBar->setWindowTitle(QApplication::translate("RadarToolBar", "RadarToolBar", 0, QApplication::UnicodeUTF8));
        actionPlot->setText(QApplication::translate("RadarToolBar", "\345\211\226\351\235\242\345\233\276", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPlot->setToolTip(QApplication::translate("RadarToolBar", "\345\211\226\351\235\242", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionWin4->setText(QApplication::translate("RadarToolBar", "\345\233\233\350\201\224\345\261\217", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionWin4->setToolTip(QApplication::translate("RadarToolBar", "\345\233\233\350\201\224\345\261\217", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionArea->setText(QApplication::translate("RadarToolBar", "\351\235\242\347\247\257\347\273\237\350\256\241", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionArea->setToolTip(QApplication::translate("RadarToolBar", "\351\235\242\347\247\257\347\273\237\350\256\241", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFuzzy->setText(QApplication::translate("RadarToolBar", "\351\200\237\345\272\246\351\200\200\346\250\241\347\263\212", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFuzzy->setToolTip(QApplication::translate("RadarToolBar", "\351\200\237\345\272\246\351\200\200\346\250\241\347\263\212", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionDB2->setText(QApplication::translate("RadarToolBar", "\347\255\211\345\200\274\351\235\242\347\224\237\346\210\220", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionDB2->setToolTip(QApplication::translate("RadarToolBar", "\347\255\211\345\200\274\351\235\242\347\224\237\346\210\220", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action2D->setText(QApplication::translate("RadarToolBar", "\344\272\214\347\273\264", 0, QApplication::UnicodeUTF8));
        action3D->setText(QApplication::translate("RadarToolBar", "\344\270\211\347\273\264", 0, QApplication::UnicodeUTF8));
        actionVolumeRender->setText(QApplication::translate("RadarToolBar", "\344\275\223\347\273\230\345\210\266", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionVolumeRender->setToolTip(QApplication::translate("RadarToolBar", "\344\275\223\347\273\230\345\210\266", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionClipPlane->setText(QApplication::translate("RadarToolBar", "\345\234\260\347\220\203\345\211\226\351\235\242", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionClipPlane->setToolTip(QApplication::translate("RadarToolBar", "\346\267\273\345\212\240\345\234\260\347\220\203\345\211\226\351\235\242", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_VTK->setText(QApplication::translate("RadarToolBar", "\345\211\226\351\235\242\344\270\211\347\273\264", 0, QApplication::UnicodeUTF8));
        actionAnimatic->setText(QApplication::translate("RadarToolBar", "\345\212\250\347\224\273", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAnimatic->setToolTip(QApplication::translate("RadarToolBar", "\345\212\250\347\224\273", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class RadarToolBar: public Ui_RadarToolBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RADARTOOLBAR_H
