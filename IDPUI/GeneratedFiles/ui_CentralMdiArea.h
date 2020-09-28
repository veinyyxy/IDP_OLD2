/********************************************************************************
** Form generated from reading UI file 'CentralMdiArea.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CENTRALMDIAREA_H
#define UI_CENTRALMDIAREA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMdiArea>

QT_BEGIN_NAMESPACE

class Ui_CentralMdiArea
{
public:

    void setupUi(QMdiArea *CentralMdiArea)
    {
        if (CentralMdiArea->objectName().isEmpty())
            CentralMdiArea->setObjectName(QString::fromUtf8("CentralMdiArea"));
        CentralMdiArea->resize(400, 300);

        retranslateUi(CentralMdiArea);

        QMetaObject::connectSlotsByName(CentralMdiArea);
    } // setupUi

    void retranslateUi(QMdiArea *CentralMdiArea)
    {
        CentralMdiArea->setWindowTitle(QApplication::translate("CentralMdiArea", "CentralMdiArea", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CentralMdiArea: public Ui_CentralMdiArea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CENTRALMDIAREA_H
