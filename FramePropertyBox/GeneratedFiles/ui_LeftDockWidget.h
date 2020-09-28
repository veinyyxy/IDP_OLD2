/********************************************************************************
** Form generated from reading UI file 'LeftDockWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEFTDOCKWIDGET_H
#define UI_LEFTDOCKWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LeftDockWidget
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QToolBar *toolBar;
    QTreeView *treeWidget;

    void setupUi(QDockWidget *LeftDockWidget)
    {
        if (LeftDockWidget->objectName().isEmpty())
            LeftDockWidget->setObjectName(QString::fromUtf8("LeftDockWidget"));
        LeftDockWidget->resize(310, 541);
        LeftDockWidget->setMinimumSize(QSize(280, 240));
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setStyleSheet(QString::fromUtf8("QPushButton{	border: none;color: #000000;}\n"
"QPushButton:hover{border: 1px solid #E0B66E;}\n"
"QPushButton:pressed{border: 1px solid #DE951C;border-bottom: none;color: white;}\n"
"QPushButton:checked{border: 1px solid #DE951C;border-bottom: none;color: white;}"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        toolBar = new QToolBar(widget);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));

        verticalLayout->addWidget(toolBar);

        treeWidget = new QTreeView(widget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->header()->setDefaultSectionSize(0);

        verticalLayout->addWidget(treeWidget);

        LeftDockWidget->setWidget(widget);

        retranslateUi(LeftDockWidget);

        QMetaObject::connectSlotsByName(LeftDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *LeftDockWidget)
    {
        LeftDockWidget->setWindowTitle(QApplication::translate("LeftDockWidget", "\345\233\276\345\261\202\346\216\247\345\210\266", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LeftDockWidget: public Ui_LeftDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEFTDOCKWIDGET_H
