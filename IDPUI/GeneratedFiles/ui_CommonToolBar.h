/********************************************************************************
** Form generated from reading UI file 'CommonToolBar.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMONTOOLBAR_H
#define UI_COMMONTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>

QT_BEGIN_NAMESPACE

class Ui_CommonToolBar
{
public:
    QAction *actionThematic;
    QAction *actionText;
    QAction *actionVLine;
    QAction *actionSLine;
    QAction *actionFLine;
    QAction *actionRect;
    QAction *actionRound;
    QAction *actionMouse;
    QAction *actionDelete;
    QAction *actionMagnifier;
    QAction *actionEdit;

    void setupUi(QToolBar *CommonToolBar)
    {
        if (CommonToolBar->objectName().isEmpty())
            CommonToolBar->setObjectName(QString::fromUtf8("CommonToolBar"));
        CommonToolBar->setEnabled(false);
        CommonToolBar->resize(643, 85);
        CommonToolBar->setIconSize(QSize(20, 20));
        CommonToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        actionThematic = new QAction(CommonToolBar);
        actionThematic->setObjectName(QString::fromUtf8("actionThematic"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/commonToolBar/thematic.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionThematic->setIcon(icon);
        actionText = new QAction(CommonToolBar);
        actionText->setObjectName(QString::fromUtf8("actionText"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/commonToolBar/text.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionText->setIcon(icon1);
        actionVLine = new QAction(CommonToolBar);
        actionVLine->setObjectName(QString::fromUtf8("actionVLine"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/commonToolBar/vLine.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionVLine->setIcon(icon2);
        actionSLine = new QAction(CommonToolBar);
        actionSLine->setObjectName(QString::fromUtf8("actionSLine"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/commonToolBar/sLine.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSLine->setIcon(icon3);
        actionFLine = new QAction(CommonToolBar);
        actionFLine->setObjectName(QString::fromUtf8("actionFLine"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/commonToolBar/fLine.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFLine->setIcon(icon4);
        actionRect = new QAction(CommonToolBar);
        actionRect->setObjectName(QString::fromUtf8("actionRect"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/commonToolBar/rect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRect->setIcon(icon5);
        actionRound = new QAction(CommonToolBar);
        actionRound->setObjectName(QString::fromUtf8("actionRound"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/commonToolBar/round.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRound->setIcon(icon6);
        actionMouse = new QAction(CommonToolBar);
        actionMouse->setObjectName(QString::fromUtf8("actionMouse"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/commonToolBar/pard.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMouse->setIcon(icon7);
        actionDelete = new QAction(CommonToolBar);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/commonToolBar/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon8);
        actionMagnifier = new QAction(CommonToolBar);
        actionMagnifier->setObjectName(QString::fromUtf8("actionMagnifier"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/commonToolBar/magnifier.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMagnifier->setIcon(icon9);
        actionEdit = new QAction(CommonToolBar);
        actionEdit->setObjectName(QString::fromUtf8("actionEdit"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/commonToolBar/eidt.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEdit->setIcon(icon10);

        CommonToolBar->addAction(actionThematic);

        retranslateUi(CommonToolBar);

        QMetaObject::connectSlotsByName(CommonToolBar);
    } // setupUi

    void retranslateUi(QToolBar *CommonToolBar)
    {
        CommonToolBar->setWindowTitle(QApplication::translate("CommonToolBar", "CommonToolBar", 0, QApplication::UnicodeUTF8));
        actionThematic->setText(QApplication::translate("CommonToolBar", "\345\210\266\345\233\276", 0, QApplication::UnicodeUTF8));
        actionText->setText(QApplication::translate("CommonToolBar", "\346\226\207\345\255\227", 0, QApplication::UnicodeUTF8));
        actionVLine->setText(QApplication::translate("CommonToolBar", "\347\233\264\347\272\277", 0, QApplication::UnicodeUTF8));
        actionSLine->setText(QApplication::translate("CommonToolBar", "\346\233\262\347\272\277", 0, QApplication::UnicodeUTF8));
        actionFLine->setText(QApplication::translate("CommonToolBar", "\346\212\230\347\272\277", 0, QApplication::UnicodeUTF8));
        actionRect->setText(QApplication::translate("CommonToolBar", "\347\237\251\345\275\242", 0, QApplication::UnicodeUTF8));
        actionRound->setText(QApplication::translate("CommonToolBar", "\345\234\206\345\275\242", 0, QApplication::UnicodeUTF8));
        actionMouse->setText(QApplication::translate("CommonToolBar", "\351\274\240\346\240\207", 0, QApplication::UnicodeUTF8));
        actionDelete->setText(QApplication::translate("CommonToolBar", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        actionMagnifier->setText(QApplication::translate("CommonToolBar", "\346\224\276\345\244\247\351\225\234", 0, QApplication::UnicodeUTF8));
        actionEdit->setText(QApplication::translate("CommonToolBar", "\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CommonToolBar: public Ui_CommonToolBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMONTOOLBAR_H
