/********************************************************************************
** Form generated from reading UI file 'DefaultToolBar.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEFAULTTOOLBAR_H
#define UI_DEFAULTTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>

QT_BEGIN_NAMESPACE

class Ui_DefaultToolBar
{
public:
    QAction *actionOpen;
    QAction *actionNew;
    QAction *actionConfig;
    QAction *actionHelp;
    QAction *actionExit;

    void setupUi(QToolBar *DefaultToolBar)
    {
        if (DefaultToolBar->objectName().isEmpty())
            DefaultToolBar->setObjectName(QString::fromUtf8("DefaultToolBar"));
        DefaultToolBar->resize(509, 83);
        DefaultToolBar->setIconSize(QSize(20, 20));
        DefaultToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        actionOpen = new QAction(DefaultToolBar);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/defaultToolBar/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionNew = new QAction(DefaultToolBar);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/defaultToolBar/newproject.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon1);
        actionConfig = new QAction(DefaultToolBar);
        actionConfig->setObjectName(QString::fromUtf8("actionConfig"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/defaultToolBar/config.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConfig->setIcon(icon2);
        actionHelp = new QAction(DefaultToolBar);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/defaultToolBar/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHelp->setIcon(icon3);
        actionExit = new QAction(DefaultToolBar);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/defaultToolBar/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon4);

        DefaultToolBar->addAction(actionOpen);
        DefaultToolBar->addSeparator();
        DefaultToolBar->addAction(actionExit);

        retranslateUi(DefaultToolBar);

        QMetaObject::connectSlotsByName(DefaultToolBar);
    } // setupUi

    void retranslateUi(QToolBar *DefaultToolBar)
    {
        DefaultToolBar->setWindowTitle(QApplication::translate("DefaultToolBar", "DefaultToolBar", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("DefaultToolBar", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("DefaultToolBar", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionNew->setText(QApplication::translate("DefaultToolBar", "\346\226\260\345\273\272\345\267\245\347\250\213", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("DefaultToolBar", "\346\226\260\345\273\272\345\267\245\347\250\213", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionConfig->setText(QApplication::translate("DefaultToolBar", "\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionConfig->setToolTip(QApplication::translate("DefaultToolBar", "\347\263\273\347\273\237\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionHelp->setText(QApplication::translate("DefaultToolBar", "\347\263\273\347\273\237\345\270\256\345\212\251", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionHelp->setToolTip(QApplication::translate("DefaultToolBar", "\347\263\273\347\273\237\345\270\256\345\212\251", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("DefaultToolBar", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("DefaultToolBar", "\351\200\200\345\207\272\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class DefaultToolBar: public Ui_DefaultToolBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEFAULTTOOLBAR_H
