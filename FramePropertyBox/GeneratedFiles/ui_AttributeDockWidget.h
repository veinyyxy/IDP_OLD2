/********************************************************************************
** Form generated from reading UI file 'AttributeDockWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTRIBUTEDOCKWIDGET_H
#define UI_ATTRIBUTEDOCKWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AttributeDockWidget
{
public:
    QWidget *widget;

    void setupUi(QDockWidget *AttributeDockWidget)
    {
        if (AttributeDockWidget->objectName().isEmpty())
            AttributeDockWidget->setObjectName(QString::fromUtf8("AttributeDockWidget"));
        AttributeDockWidget->resize(400, 300);
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        AttributeDockWidget->setWidget(widget);

        retranslateUi(AttributeDockWidget);

        QMetaObject::connectSlotsByName(AttributeDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *AttributeDockWidget)
    {
        AttributeDockWidget->setWindowTitle(QApplication::translate("AttributeDockWidget", "\345\233\276\345\261\202\345\261\236\346\200\247", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AttributeDockWidget: public Ui_AttributeDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTRIBUTEDOCKWIDGET_H
