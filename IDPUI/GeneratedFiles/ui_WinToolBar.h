/********************************************************************************
** Form generated from reading UI file 'WinToolBar.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINTOOLBAR_H
#define UI_WINTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>

QT_BEGIN_NAMESPACE

class Ui_WinToolBar
{
public:
    QAction *actionCascad;
    QAction *actionTile;
    QAction *actionClose;
    QAction *actionCloseAll;
    QAction *actionFullScreen;

    void setupUi(QToolBar *WinToolBar)
    {
        if (WinToolBar->objectName().isEmpty())
            WinToolBar->setObjectName(QString::fromUtf8("WinToolBar"));
        WinToolBar->resize(665, 89);
        WinToolBar->setIconSize(QSize(20, 20));
        WinToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        actionCascad = new QAction(WinToolBar);
        actionCascad->setObjectName(QString::fromUtf8("actionCascad"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/winToolBar/cascad.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCascad->setIcon(icon);
        actionTile = new QAction(WinToolBar);
        actionTile->setObjectName(QString::fromUtf8("actionTile"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/winToolBar/tile.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTile->setIcon(icon1);
        actionClose = new QAction(WinToolBar);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/winToolBar/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose->setIcon(icon2);
        actionCloseAll = new QAction(WinToolBar);
        actionCloseAll->setObjectName(QString::fromUtf8("actionCloseAll"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/winToolBar/closeAll.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCloseAll->setIcon(icon3);
        actionFullScreen = new QAction(WinToolBar);
        actionFullScreen->setObjectName(QString::fromUtf8("actionFullScreen"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/winToolBar/fullscreen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFullScreen->setIcon(icon4);

        WinToolBar->addAction(actionFullScreen);
        WinToolBar->addAction(actionCascad);
        WinToolBar->addAction(actionTile);
        WinToolBar->addAction(actionClose);
        WinToolBar->addAction(actionCloseAll);

        retranslateUi(WinToolBar);

        QMetaObject::connectSlotsByName(WinToolBar);
    } // setupUi

    void retranslateUi(QToolBar *WinToolBar)
    {
        WinToolBar->setWindowTitle(QApplication::translate("WinToolBar", "WinToolBar", 0, QApplication::UnicodeUTF8));
        actionCascad->setText(QApplication::translate("WinToolBar", "\345\261\202\345\217\240", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCascad->setToolTip(QApplication::translate("WinToolBar", "\347\252\227\345\217\243\345\261\202\345\217\240", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionTile->setText(QApplication::translate("WinToolBar", "\345\271\263\351\223\272", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionTile->setToolTip(QApplication::translate("WinToolBar", "\347\252\227\345\217\243\345\271\263\351\223\272", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionClose->setText(QApplication::translate("WinToolBar", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionClose->setToolTip(QApplication::translate("WinToolBar", "\345\205\263\351\227\255\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionCloseAll->setText(QApplication::translate("WinToolBar", "\345\205\263\351\227\255\346\211\200\346\234\211", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCloseAll->setToolTip(QApplication::translate("WinToolBar", "\345\205\263\351\227\255\346\211\200\346\234\211", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFullScreen->setText(QApplication::translate("WinToolBar", "\345\205\250\345\261\217", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFullScreen->setToolTip(QApplication::translate("WinToolBar", "\345\205\250\345\261\217", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFullScreen->setShortcut(QApplication::translate("WinToolBar", "Alt+Return", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WinToolBar: public Ui_WinToolBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINTOOLBAR_H
