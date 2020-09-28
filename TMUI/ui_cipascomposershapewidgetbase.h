/********************************************************************************
** Form generated from reading UI file 'qgscomposershapewidgetbase.ui'
**
** Created: Wed Aug 31 17:44:47 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPASCOMPOSERSHAPEWIDGETBASE_H
#define UI_CIPASCOMPOSERSHAPEWIDGETBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CipasComposerShapeWidgetBase
{
public:
    QGridLayout *gridLayout_2;
    QToolBox *toolBox;
    QWidget *Shape;
    QGridLayout *gridLayout;
    QComboBox *mShapeComboBox;
    QPushButton *mOutlineColorButton;
    QDoubleSpinBox *mOutlineWidthSpinBox;
    QCheckBox *mTransparentCheckBox;
    QPushButton *mFillColorButton;
    QSpinBox *mRotationSpinBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *CipasComposerShapeWidgetBase)
    {
        if (CipasComposerShapeWidgetBase->objectName().isEmpty())
            CipasComposerShapeWidgetBase->setObjectName(QString::fromUtf8("CipasComposerShapeWidgetBase"));
        CipasComposerShapeWidgetBase->resize(285, 427);
        gridLayout_2 = new QGridLayout(CipasComposerShapeWidgetBase);
		gridLayout_2->setContentsMargins(0,0,0,0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        toolBox = new QToolBox(CipasComposerShapeWidgetBase);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        Shape = new QWidget();
        Shape->setObjectName(QString::fromUtf8("Shape"));
        Shape->setGeometry(QRect(0, 0, 267, 377));
        gridLayout = new QGridLayout(Shape);
		gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mShapeComboBox = new QComboBox(Shape);
        mShapeComboBox->setObjectName(QString::fromUtf8("mShapeComboBox"));

        gridLayout->addWidget(mShapeComboBox, 0, 0, 1, 1);

        mOutlineColorButton = new QPushButton(Shape);
        mOutlineColorButton->setObjectName(QString::fromUtf8("mOutlineColorButton"));

        gridLayout->addWidget(mOutlineColorButton, 1, 0, 1, 1);

        mOutlineWidthSpinBox = new QDoubleSpinBox(Shape);
        mOutlineWidthSpinBox->setObjectName(QString::fromUtf8("mOutlineWidthSpinBox"));

        gridLayout->addWidget(mOutlineWidthSpinBox, 2, 0, 1, 1);

        mTransparentCheckBox = new QCheckBox(Shape);
        mTransparentCheckBox->setObjectName(QString::fromUtf8("mTransparentCheckBox"));

        gridLayout->addWidget(mTransparentCheckBox, 3, 0, 1, 1);

        mFillColorButton = new QPushButton(Shape);
        mFillColorButton->setObjectName(QString::fromUtf8("mFillColorButton"));

        gridLayout->addWidget(mFillColorButton, 4, 0, 1, 1);

        mRotationSpinBox = new QSpinBox(Shape);
        mRotationSpinBox->setObjectName(QString::fromUtf8("mRotationSpinBox"));
        mRotationSpinBox->setMaximum(359);

        gridLayout->addWidget(mRotationSpinBox, 5, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 6, 0, 1, 1);

        toolBox->addItem(Shape, QString::fromUtf8("Shape"));

        gridLayout_2->addWidget(toolBox, 0, 0, 1, 1);


        retranslateUi(CipasComposerShapeWidgetBase);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CipasComposerShapeWidgetBase);
    } // setupUi

    void retranslateUi(QWidget *CipasComposerShapeWidgetBase)
    {
        CipasComposerShapeWidgetBase->setWindowTitle(QApplication::translate("CipasComposerShapeWidgetBase", "Form", 0, QApplication::UnicodeUTF8));
        mOutlineColorButton->setText(QString::fromLocal8Bit("图形外包线颜色..."));
        mOutlineWidthSpinBox->setPrefix(QString::fromLocal8Bit("线宽"));
        mTransparentCheckBox->setText(QString::fromLocal8Bit("透明 填充"));
        mFillColorButton->setText(QString::fromLocal8Bit("图形填充色..."));
        mRotationSpinBox->setPrefix(QString::fromLocal8Bit("旋转"));
        toolBox->setItemText(toolBox->indexOf(Shape), QString::fromLocal8Bit("图形"));
    } // retranslateUi

};

namespace Ui {
    class CipasComposerShapeWidgetBase: public Ui_CipasComposerShapeWidgetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPASCOMPOSERSHAPEWIDGETBASE_H
