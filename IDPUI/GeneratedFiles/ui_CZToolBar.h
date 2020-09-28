/********************************************************************************
** Form generated from reading UI file 'CZToolBar.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CZTOOLBAR_H
#define UI_CZTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>

QT_BEGIN_NAMESPACE

class Ui_CZToolBar
{
public:
    QAction *actionDIY;

    void setupUi(QToolBar *CZToolBar)
    {
        if (CZToolBar->objectName().isEmpty())
            CZToolBar->setObjectName(QString::fromUtf8("CZToolBar"));
        CZToolBar->resize(569, 78);
        CZToolBar->setIconSize(QSize(20, 20));
        CZToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        actionDIY = new QAction(CZToolBar);
        actionDIY->setObjectName(QString::fromUtf8("actionDIY"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/czToolBar/window_new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDIY->setIcon(icon);

        CZToolBar->addAction(actionDIY);

        retranslateUi(CZToolBar);

        QMetaObject::connectSlotsByName(CZToolBar);
    } // setupUi

    void retranslateUi(QToolBar *CZToolBar)
    {
        CZToolBar->setWindowTitle(QApplication::translate("CZToolBar", "CZToolBar", 0, QApplication::UnicodeUTF8));
        actionDIY->setText(QApplication::translate("CZToolBar", "\350\207\252\351\200\211\345\214\272\345\237\237", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionDIY->setToolTip(QApplication::translate("CZToolBar", "\350\207\252\351\200\211\345\214\272\345\237\237", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class CZToolBar: public Ui_CZToolBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CZTOOLBAR_H
