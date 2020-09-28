/********************************************************************************
** Form generated from reading UI file 'MapToolBar.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPTOOLBAR_H
#define UI_MAPTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>

QT_BEGIN_NAMESPACE

class Ui_MapToolBar
{
public:
    QAction *actionCenter;
    QAction *actionTop;
    QAction *actionBottom;
    QAction *actionLeft;
    QAction *actionRight;
    QAction *actionMZoomIn;
    QAction *actionMZoomout;

    void setupUi(QToolBar *MapToolBar)
    {
        if (MapToolBar->objectName().isEmpty())
            MapToolBar->setObjectName(QString::fromUtf8("MapToolBar"));
        MapToolBar->resize(563, 66);
        MapToolBar->setIconSize(QSize(20, 20));
        MapToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        actionCenter = new QAction(MapToolBar);
        actionCenter->setObjectName(QString::fromUtf8("actionCenter"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/mapToolBar/center.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCenter->setIcon(icon);
        actionTop = new QAction(MapToolBar);
        actionTop->setObjectName(QString::fromUtf8("actionTop"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/mapToolBar/top.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTop->setIcon(icon1);
        actionBottom = new QAction(MapToolBar);
        actionBottom->setObjectName(QString::fromUtf8("actionBottom"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/mapToolBar/bottom.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBottom->setIcon(icon2);
        actionLeft = new QAction(MapToolBar);
        actionLeft->setObjectName(QString::fromUtf8("actionLeft"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/mapToolBar/left.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLeft->setIcon(icon3);
        actionRight = new QAction(MapToolBar);
        actionRight->setObjectName(QString::fromUtf8("actionRight"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/mapToolBar/right.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRight->setIcon(icon4);
        actionMZoomIn = new QAction(MapToolBar);
        actionMZoomIn->setObjectName(QString::fromUtf8("actionMZoomIn"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/mapToolBar/Ex_ZoomIn.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMZoomIn->setIcon(icon5);
        actionMZoomout = new QAction(MapToolBar);
        actionMZoomout->setObjectName(QString::fromUtf8("actionMZoomout"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/mapToolBar/Ex_ZoomOut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMZoomout->setIcon(icon6);

        MapToolBar->addAction(actionTop);
        MapToolBar->addAction(actionBottom);
        MapToolBar->addAction(actionLeft);
        MapToolBar->addAction(actionRight);
        MapToolBar->addAction(actionMZoomout);
        MapToolBar->addAction(actionMZoomIn);
        MapToolBar->addAction(actionCenter);

        retranslateUi(MapToolBar);

        QMetaObject::connectSlotsByName(MapToolBar);
    } // setupUi

    void retranslateUi(QToolBar *MapToolBar)
    {
        MapToolBar->setWindowTitle(QApplication::translate("MapToolBar", "MapToolBar", 0, QApplication::UnicodeUTF8));
        actionCenter->setText(QApplication::translate("MapToolBar", "\350\277\224\345\233\236", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCenter->setToolTip(QApplication::translate("MapToolBar", "\350\277\224\345\233\236\345\216\237\347\202\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionTop->setText(QApplication::translate("MapToolBar", "\344\270\212\347\247\273", 0, QApplication::UnicodeUTF8));
        actionBottom->setText(QApplication::translate("MapToolBar", "\344\270\213\347\247\273", 0, QApplication::UnicodeUTF8));
        actionLeft->setText(QApplication::translate("MapToolBar", "\345\267\246\347\247\273", 0, QApplication::UnicodeUTF8));
        actionRight->setText(QApplication::translate("MapToolBar", "\345\217\263\347\247\273", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRight->setToolTip(QApplication::translate("MapToolBar", "\345\217\263\347\247\273", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionMZoomIn->setText(QApplication::translate("MapToolBar", "\346\224\276\345\244\247", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionMZoomIn->setToolTip(QApplication::translate("MapToolBar", "\346\214\211\346\257\224\344\276\213\346\224\276\345\244\247", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionMZoomout->setText(QApplication::translate("MapToolBar", "\347\274\251\345\260\217", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MapToolBar: public Ui_MapToolBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPTOOLBAR_H
