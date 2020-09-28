/********************************************************************************
** Form generated from reading UI file 'qgscomposeritemwidgetbase.ui'
**
** Created: Wed Aug 31 17:44:46 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPASCOMPOSERITEMWIDGETBASE_H
#define UI_CIPASCOMPOSERITEMWIDGETBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CipasComposerItemWidgetBase//:public QObject
{
	
	//Q_OBJECT

public:
    QGridLayout *gridLayout;
    QPushButton *mFrameColorButton;
    QPushButton *mBackgroundColorButton;
    QLabel *mOpacityLabel;
    QSlider *mOpacitySlider;
    QLabel *mOutlineWidthLabel;
    QDoubleSpinBox *mOutlineWidthSpinBox;
    //QPushButton *mPositionButton;
    QCheckBox *mFrameCheckBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *CipasComposerItemWidgetBase)
    {
        if (CipasComposerItemWidgetBase->objectName().isEmpty())
            CipasComposerItemWidgetBase->setObjectName(QString::fromUtf8("CipasComposerItemWidgetBase"));
        CipasComposerItemWidgetBase->resize(236, 314);
        gridLayout = new QGridLayout(CipasComposerItemWidgetBase);
		gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mFrameColorButton = new QPushButton(CipasComposerItemWidgetBase);
        mFrameColorButton->setObjectName(QString::fromUtf8("mFrameColorButton"));

        gridLayout->addWidget(mFrameColorButton, 0, 0, 1, 1);

        mBackgroundColorButton = new QPushButton(CipasComposerItemWidgetBase);
        mBackgroundColorButton->setObjectName(QString::fromUtf8("mBackgroundColorButton"));

        gridLayout->addWidget(mBackgroundColorButton, 1, 0, 1, 1);

        mOpacityLabel = new QLabel(CipasComposerItemWidgetBase);
        mOpacityLabel->setObjectName(QString::fromUtf8("mOpacityLabel"));
        mOpacityLabel->setWordWrap(true);

        gridLayout->addWidget(mOpacityLabel, 2, 0, 1, 1);

        mOpacitySlider = new QSlider(CipasComposerItemWidgetBase);
        mOpacitySlider->setObjectName(QString::fromUtf8("mOpacitySlider"));
        mOpacitySlider->setMaximum(255);
        mOpacitySlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(mOpacitySlider, 3, 0, 1, 1);

        mOutlineWidthLabel = new QLabel(CipasComposerItemWidgetBase);
        mOutlineWidthLabel->setObjectName(QString::fromUtf8("mOutlineWidthLabel"));
        mOutlineWidthLabel->setWordWrap(true);

        gridLayout->addWidget(mOutlineWidthLabel, 4, 0, 1, 1);

        mOutlineWidthSpinBox = new QDoubleSpinBox(CipasComposerItemWidgetBase);
        mOutlineWidthSpinBox->setObjectName(QString::fromUtf8("mOutlineWidthSpinBox"));

        gridLayout->addWidget(mOutlineWidthSpinBox, 5, 0, 1, 1);

       /* mPositionButton = new QPushButton(CipasComposerItemWidgetBase);
        mPositionButton->setObjectName(QString::fromUtf8("mPositionButton"));

        gridLayout->addWidget(mPositionButton, 6, 0, 1, 1);*/

        mFrameCheckBox = new QCheckBox(CipasComposerItemWidgetBase);
        mFrameCheckBox->setObjectName(QString::fromUtf8("mFrameCheckBox"));

        gridLayout->addWidget(mFrameCheckBox, 6, 0, 1, 1);

        verticalSpacer = new QSpacerItem(215, 57, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 7, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        mOpacityLabel->setBuddy(mOpacitySlider);
        mOutlineWidthLabel->setBuddy(mOutlineWidthSpinBox);
#endif // QT_NO_SHORTCUT

        retranslateUi(CipasComposerItemWidgetBase);

        QMetaObject::connectSlotsByName(CipasComposerItemWidgetBase);
    } // setupUi

    void retranslateUi(QWidget *CipasComposerItemWidgetBase)
    {
        CipasComposerItemWidgetBase->setWindowTitle(QApplication::translate("CipasComposerItemWidgetBase", "Form", 0, QApplication::UnicodeUTF8));
        mFrameColorButton->setText(QObject::tr("边框颜色..."));
        mBackgroundColorButton->setText(QObject::tr("背景颜色..."));
        mOpacityLabel->setText(QObject::tr("透明度"));
        mOutlineWidthLabel->setText(QObject::tr("外包线宽"));
        //mPositionButton->setText(QObject::tr("位置和大小..."));
        mFrameCheckBox->setText(QObject::tr("显示边框"));
    } // retranslateUi

};

namespace Ui {
    class CipasComposerItemWidgetBase: public Ui_CipasComposerItemWidgetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPASCOMPOSERITEMWIDGETBASE_H
