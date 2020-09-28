/********************************************************************************
** Form generated from reading UI file 'PlotDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTDIALOG_H
#define UI_PLOTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_PlotDialog
{
public:

    void setupUi(QDialog *PlotDialog)
    {
        if (PlotDialog->objectName().isEmpty())
            PlotDialog->setObjectName(QString::fromUtf8("PlotDialog"));
        PlotDialog->resize(400, 300);

        retranslateUi(PlotDialog);

        QMetaObject::connectSlotsByName(PlotDialog);
    } // setupUi

    void retranslateUi(QDialog *PlotDialog)
    {
        PlotDialog->setWindowTitle(QApplication::translate("PlotDialog", "\345\211\226\351\235\242\345\233\276", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlotDialog: public Ui_PlotDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTDIALOG_H
