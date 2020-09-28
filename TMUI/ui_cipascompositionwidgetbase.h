/********************************************************************************
** Form generated from reading UI file 'qgscompositionwidgetbase.ui'
**
** Created: Wed Aug 31 17:44:47 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPASCOMPOSITIONWIDGETBASE_H
#define UI_CIPASCOMPOSITIONWIDGETBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>
#include "cipascolorbutton.h"

QT_BEGIN_NAMESPACE

class Ui_CipasCompositionWidgetBase
{
public:
    QGridLayout *gridLayout_4;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *textLabel3;
    QComboBox *mPaperSizeComboBox;
    QLabel *textLabel5;
    QComboBox *mPaperUnitsComboBox;
    QLabel *textLabel7;
    QComboBox *mPaperOrientationComboBox;
    //QCheckBox *mPrintAsRasterCheckBox;
    QSpinBox *mResolutionSpinBox;
    QDoubleSpinBox *mPaperWidthDoubleSpinBox;
    QDoubleSpinBox *mPaperHeightDoubleSpinBox;
    QGroupBox *mSnapGroupBox;
    QGridLayout *gridLayout_2;
    QCheckBox *mSnapToGridCheckBox;
    QDoubleSpinBox *mGridResolutionSpinBox;
    QDoubleSpinBox *mOffsetXSpinBox;
    QDoubleSpinBox *mOffsetYSpinBox;
    QDoubleSpinBox *mPenWidthSpinBox;
    QLabel *mGridColorLabel;
    CipasColorButton *mGridColorButton;
    QLabel *mGridStyleLabel;
    QComboBox *mGridStyleComboBox;
    QSpacerItem *verticalSpacer;
	//////////////////////////////////////////////////////////////////////////
	QGroupBox* mpPaperBoundingLinerSetingGroupBox;
	QGridLayout* mpBoundingLinerLayout;
	CipasColorButton* mpColorButton;
	QComboBox* mpLineStyle;
	//QComboBox* mpConerSytle;
	QDoubleSpinBox* mpLineNum;
	QDoubleSpinBox*  mpLineInterval;
	QDoubleSpinBox* mpLineWidth;
	//QDoubleSpinBox* mpLineDashOffset;
	QCheckBox* mpIsDrawLineRect;
	QLabel* textMargin;
	QLabel* textMarginX;
	QLabel* textMarginY;
	QLabel* textFrameLineColor;
	QDoubleSpinBox* mMarginXSpinBox;
	QDoubleSpinBox* mMarginYSpinBox;

	//////////////////////////////////////////////////////////////////////////
    void setupUi(QWidget *CipasCompositionWidgetBase)
    {
        if (CipasCompositionWidgetBase->objectName().isEmpty())
            CipasCompositionWidgetBase->setObjectName(QString::fromUtf8("CipasCompositionWidgetBase"));
        CipasCompositionWidgetBase->resize(300, 500);
		CipasCompositionWidgetBase->setContentsMargins(0,0,0,0);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CipasCompositionWidgetBase->sizePolicy().hasHeightForWidth());
        CipasCompositionWidgetBase->setSizePolicy(sizePolicy);
        gridLayout_4 = new QGridLayout(CipasCompositionWidgetBase);
		gridLayout_4->setContentsMargins(0,0,0,0);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        scrollArea = new QScrollArea(CipasCompositionWidgetBase);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, -11, 397, 579));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
		gridLayout_3->setContentsMargins(0,0,0,0);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(scrollAreaWidgetContents);
		groupBox->setContentsMargins(0,0,0,0);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
		gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textLabel3 = new QLabel(groupBox);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textLabel3->sizePolicy().hasHeightForWidth());
        textLabel3->setSizePolicy(sizePolicy1);
        textLabel3->setWordWrap(true);

        gridLayout->addWidget(textLabel3, 0, 0, 1, 1);

        mPaperSizeComboBox = new QComboBox(groupBox);
        mPaperSizeComboBox->setObjectName(QString::fromUtf8("mPaperSizeComboBox"));
        mPaperSizeComboBox->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(mPaperSizeComboBox->sizePolicy().hasHeightForWidth());
        mPaperSizeComboBox->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(mPaperSizeComboBox, 1, 0, 1, 1);

        textLabel5 = new QLabel(groupBox);
        textLabel5->setObjectName(QString::fromUtf8("textLabel5"));
        sizePolicy1.setHeightForWidth(textLabel5->sizePolicy().hasHeightForWidth());
        textLabel5->setSizePolicy(sizePolicy1);
        textLabel5->setWordWrap(true);

        gridLayout->addWidget(textLabel5, 2, 0, 1, 1);

        mPaperUnitsComboBox = new QComboBox(groupBox);
        mPaperUnitsComboBox->setObjectName(QString::fromUtf8("mPaperUnitsComboBox"));
        mPaperUnitsComboBox->setEnabled(true);
        sizePolicy2.setHeightForWidth(mPaperUnitsComboBox->sizePolicy().hasHeightForWidth());
        mPaperUnitsComboBox->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(mPaperUnitsComboBox, 3, 0, 1, 1);

        textLabel7 = new QLabel(groupBox);
        textLabel7->setObjectName(QString::fromUtf8("textLabel7"));
        sizePolicy1.setHeightForWidth(textLabel7->sizePolicy().hasHeightForWidth());
        textLabel7->setSizePolicy(sizePolicy1);
        textLabel7->setWordWrap(true);

        gridLayout->addWidget(textLabel7, 6, 0, 1, 1);

        mPaperOrientationComboBox = new QComboBox(groupBox);
        mPaperOrientationComboBox->setObjectName(QString::fromUtf8("mPaperOrientationComboBox"));
        mPaperOrientationComboBox->setEnabled(true);
        sizePolicy2.setHeightForWidth(mPaperOrientationComboBox->sizePolicy().hasHeightForWidth());
        mPaperOrientationComboBox->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(mPaperOrientationComboBox, 7, 0, 1, 1);

        //mPrintAsRasterCheckBox = new QCheckBox(groupBox);
        //mPrintAsRasterCheckBox->setObjectName(QString::fromUtf8("mPrintAsRasterCheckBox"));

       // gridLayout->addWidget(mPrintAsRasterCheckBox, 9, 0, 1, 1);

        mResolutionSpinBox = new QSpinBox(groupBox);
        mResolutionSpinBox->setObjectName(QString::fromUtf8("mResolutionSpinBox"));
        mResolutionSpinBox->setMaximum(3000);

        gridLayout->addWidget(mResolutionSpinBox, 8, 0, 1, 1);

        mPaperWidthDoubleSpinBox = new QDoubleSpinBox(groupBox);
        mPaperWidthDoubleSpinBox->setObjectName(QString::fromUtf8("mPaperWidthDoubleSpinBox"));
        mPaperWidthDoubleSpinBox->setMaximum(99999);

        gridLayout->addWidget(mPaperWidthDoubleSpinBox, 4, 0, 1, 1);

        mPaperHeightDoubleSpinBox = new QDoubleSpinBox(groupBox);
        mPaperHeightDoubleSpinBox->setObjectName(QString::fromUtf8("mPaperHeightDoubleSpinBox"));
        mPaperHeightDoubleSpinBox->setMaximum(99999);

        gridLayout->addWidget(mPaperHeightDoubleSpinBox, 5, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        mSnapGroupBox = new QGroupBox(scrollAreaWidgetContents);
		mSnapGroupBox->setContentsMargins(0,0,0,0);
        mSnapGroupBox->setObjectName(QString::fromUtf8("mSnapGroupBox"));
        gridLayout_2 = new QGridLayout(mSnapGroupBox);
		gridLayout_2->setContentsMargins(0,0,0,0);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        mSnapToGridCheckBox = new QCheckBox(mSnapGroupBox);
        mSnapToGridCheckBox->setObjectName(QString::fromUtf8("mSnapToGridCheckBox"));

        gridLayout_2->addWidget(mSnapToGridCheckBox, 0, 0, 1, 1);

        mGridResolutionSpinBox = new QDoubleSpinBox(mSnapGroupBox);
        mGridResolutionSpinBox->setObjectName(QString::fromUtf8("mGridResolutionSpinBox"));
		mGridResolutionSpinBox->setMinimum(0);
        mGridResolutionSpinBox->setMaximum(9999);

        gridLayout_2->addWidget(mGridResolutionSpinBox, 1, 0, 1, 1);

        mOffsetXSpinBox = new QDoubleSpinBox(mSnapGroupBox);
        mOffsetXSpinBox->setObjectName(QString::fromUtf8("mOffsetXSpinBox"));
        mOffsetXSpinBox->setMaximum(9999);

        gridLayout_2->addWidget(mOffsetXSpinBox, 2, 0, 1, 1);

        mOffsetYSpinBox = new QDoubleSpinBox(mSnapGroupBox);
        mOffsetYSpinBox->setObjectName(QString::fromUtf8("mOffsetYSpinBox"));

        gridLayout_2->addWidget(mOffsetYSpinBox, 3, 0, 1, 1);

        mPenWidthSpinBox = new QDoubleSpinBox(mSnapGroupBox);
        mPenWidthSpinBox->setObjectName(QString::fromUtf8("mPenWidthSpinBox"));

        gridLayout_2->addWidget(mPenWidthSpinBox, 4, 0, 1, 1);

        mGridColorLabel = new QLabel(mSnapGroupBox);
        mGridColorLabel->setObjectName(QString::fromUtf8("mGridColorLabel"));
        mGridColorLabel->setWordWrap(true);

        gridLayout_2->addWidget(mGridColorLabel, 5, 0, 1, 1);

        mGridColorButton = new CipasColorButton(mSnapGroupBox);
        mGridColorButton->setObjectName(QString::fromUtf8("mGridColorButton"));
        sizePolicy1.setHeightForWidth(mGridColorButton->sizePolicy().hasHeightForWidth());
        mGridColorButton->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(mGridColorButton, 6, 0, 1, 1);

        mGridStyleLabel = new QLabel(mSnapGroupBox);
        mGridStyleLabel->setObjectName(QString::fromUtf8("mGridStyleLabel"));
        mGridStyleLabel->setWordWrap(true);

        gridLayout_2->addWidget(mGridStyleLabel, 7, 0, 1, 1);

        mGridStyleComboBox = new QComboBox(mSnapGroupBox);
        mGridStyleComboBox->setObjectName(QString::fromUtf8("mGridStyleComboBox"));

        gridLayout_2->addWidget(mGridStyleComboBox, 8, 0, 1, 1);


        gridLayout_3->addWidget(mSnapGroupBox, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 4, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_4->addWidget(scrollArea, 0, 0, 1, 1);
		//////////////////////////////////////////////////////////////////////////
		mpIsDrawLineRect = new QCheckBox;
		mpIsDrawLineRect->setObjectName(QString::fromUtf8("mpIsDrawLineRect"));
		mpIsDrawLineRect->setText(QString::fromLocal8Bit("显示纸张边框"));
		mpIsDrawLineRect->setChecked(false);

		mpPaperBoundingLinerSetingGroupBox = new QGroupBox(scrollAreaWidgetContents);
		mpPaperBoundingLinerSetingGroupBox->setObjectName(QString::fromUtf8("mpPaperBoundingLinerSetingGroupBox"));
		mpPaperBoundingLinerSetingGroupBox->setTitle(QString::fromLocal8Bit("边框线设置"));
		mpPaperBoundingLinerSetingGroupBox->setContentsMargins(0,0,0,0);
		mpBoundingLinerLayout = new QGridLayout(mpPaperBoundingLinerSetingGroupBox);
		mpBoundingLinerLayout->setContentsMargins(0,0,0,0);
		mpBoundingLinerLayout->setObjectName(QString::fromLocal8Bit("mpBoundingLinerLayout"));

		textFrameLineColor=new QLabel(mpPaperBoundingLinerSetingGroupBox);
		textFrameLineColor->setObjectName(QString::fromUtf8("textFrameLineColor"));
		sizePolicy1.setHeightForWidth(textFrameLineColor->sizePolicy().hasHeightForWidth());
		textFrameLineColor->setSizePolicy(sizePolicy1);
		textFrameLineColor->setWordWrap(true);

		mpColorButton = new CipasColorButton(mpPaperBoundingLinerSetingGroupBox);
		mpColorButton->setObjectName(QString::fromUtf8("mpColorButton"));
		mpColorButton->setText(QString::fromLocal8Bit(""));
		mpColorButton->setSizePolicy(sizePolicy1);

		mpLineStyle = new QComboBox(mpPaperBoundingLinerSetingGroupBox);
		mpLineStyle->setObjectName(QString::fromLocal8Bit("mpLineStyle"));
		mpLineStyle->addItem(QString::fromLocal8Bit("实线"));
		mpLineStyle->addItem(QString::fromLocal8Bit("虚线"));
		mpLineStyle->addItem(QString::fromLocal8Bit("点线"));
		mpLineStyle->addItem(QString::fromLocal8Bit("虚点线"));
		mpLineStyle->addItem(QString::fromLocal8Bit("虚点点线"));
		//mpLineStyle->addItem(QString::fromLocal8Bit("自定义虚线"));

		//mpConerSytle = new QComboBox(mpPaperBoundingLinerSetingGroupBox);
		//mpConerSytle->setObjectName(QString::fromLocal8Bit("mpConerSytle"));
		//mpConerSytle->addItem(QString::fromLocal8Bit("锐角"));
		//mpConerSytle->addItem(QString::fromLocal8Bit("平角"));
		//mpConerSytle->addItem(QString::fromLocal8Bit("圆滑角"));

		mpLineNum = new QDoubleSpinBox(mpPaperBoundingLinerSetingGroupBox);
		mpLineNum->setObjectName(QString::fromLocal8Bit("mpLineNum"));
		mpLineNum->setPrefix(QString::fromLocal8Bit("边线数量:"));
		mpLineNum->setMinimum(1.0);

		mMarginXSpinBox=new QDoubleSpinBox(mpPaperBoundingLinerSetingGroupBox);
		mMarginXSpinBox->setObjectName(QString::fromUtf8("mMarginXSpinBox"));
		mMarginXSpinBox->setPrefix(QString::fromLocal8Bit("横向页边距:"));
		mMarginXSpinBox->setValue(2.0);
		mMarginXSpinBox->setMinimum(.0);

		mMarginYSpinBox=new QDoubleSpinBox(mpPaperBoundingLinerSetingGroupBox);
		mMarginYSpinBox->setObjectName(QString::fromUtf8("mMarginYSpinBox"));
		mMarginYSpinBox->setPrefix(QString::fromLocal8Bit("纵向页边距:"));
		mMarginYSpinBox->setValue(2.0);
		mMarginYSpinBox->setMinimum(.0);

		mpLineInterval = new QDoubleSpinBox(mpPaperBoundingLinerSetingGroupBox);
		mpLineInterval->setObjectName(QString::fromLocal8Bit("mpLineInterval"));
		mpLineInterval->setPrefix(QString::fromLocal8Bit("多条边框间隔:"));
		mpLineInterval->setMinimum(1.0);

		mpLineWidth = new QDoubleSpinBox(mpPaperBoundingLinerSetingGroupBox);
		mpLineWidth->setObjectName(QString::fromLocal8Bit("mpLineWidth"));
		mpLineWidth->setPrefix(QString::fromLocal8Bit("线宽度:"));
		//mpLineWidth->setMinimum(1.0);

		//mpLineDashOffset = new QDoubleSpinBox(mpPaperBoundingLinerSetingGroupBox);
		//mpLineDashOffset->setObjectName(QString::fromLocal8Bit("mpLineDashOffset"));
		//mpLineDashOffset->setPrefix(QString::fromLocal8Bit("自定义虚线偏移:"));
		//mpLineDashOffset->setMinimum(0.0);

		mpPaperBoundingLinerSetingGroupBox->setLayout(mpBoundingLinerLayout);

		mpBoundingLinerLayout->addWidget(mpIsDrawLineRect, 0, 0, 1, 1);;
		mpBoundingLinerLayout->addWidget(textFrameLineColor, 1, 0, 1, 1);
		mpBoundingLinerLayout->addWidget(mpColorButton, 2, 0, 1, 2);
		mpBoundingLinerLayout->addWidget(mpLineStyle, 3, 0, 1, 2);
		//mpBoundingLinerLayout->addWidget(mpConerSytle, 4, 0, 1, 2);
		mpBoundingLinerLayout->addWidget(mMarginXSpinBox, 5, 0, 1, 1);
		mpBoundingLinerLayout->addWidget(mMarginYSpinBox, 5, 1, 1, 1);
		mpBoundingLinerLayout->addWidget(mpLineNum, 6, 0, 1, 2);
		mpBoundingLinerLayout->addWidget(mpLineInterval, 7, 0, 1, 2);
		mpBoundingLinerLayout->addWidget(mpLineWidth, 8, 0, 1, 2);
		//mpBoundingLinerLayout->addWidget(mpLineDashOffset, 9, 0, 1, 2);

		gridLayout_3->addWidget(mpPaperBoundingLinerSetingGroupBox, 3, 0, 1, 1);
		/////////////////////////////////////////////////////////////////////////
#ifndef QT_NO_SHORTCUT
        textLabel3->setBuddy(mPaperSizeComboBox);
        textLabel5->setBuddy(mPaperUnitsComboBox);
        textLabel7->setBuddy(mPaperOrientationComboBox);
        mGridColorLabel->setBuddy(mGridColorButton);
        mGridStyleLabel->setBuddy(mGridStyleComboBox);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(scrollArea, mPaperSizeComboBox);
        QWidget::setTabOrder(mPaperSizeComboBox, mPaperUnitsComboBox);
        QWidget::setTabOrder(mPaperUnitsComboBox, mPaperWidthDoubleSpinBox);
        QWidget::setTabOrder(mPaperWidthDoubleSpinBox, mPaperHeightDoubleSpinBox);
        QWidget::setTabOrder(mPaperHeightDoubleSpinBox, mPaperOrientationComboBox);
        QWidget::setTabOrder(mPaperOrientationComboBox, mResolutionSpinBox);
        //QWidget::setTabOrder(mResolutionSpinBox, mPrintAsRasterCheckBox);
        //QWidget::setTabOrder(mPrintAsRasterCheckBox, mSnapToGridCheckBox);
		QWidget::setTabOrder(mResolutionSpinBox, mSnapToGridCheckBox);
        QWidget::setTabOrder(mResolutionSpinBox, mGridResolutionSpinBox);
        QWidget::setTabOrder(mGridResolutionSpinBox, mOffsetXSpinBox);
        QWidget::setTabOrder(mOffsetXSpinBox, mOffsetYSpinBox);
        QWidget::setTabOrder(mOffsetYSpinBox, mPenWidthSpinBox);
        QWidget::setTabOrder(mPenWidthSpinBox, mGridColorButton);
        QWidget::setTabOrder(mGridColorButton, mGridStyleComboBox);

        retranslateUi(CipasCompositionWidgetBase);

        QMetaObject::connectSlotsByName(CipasCompositionWidgetBase);
    } // setupUi

    void retranslateUi(QWidget *CipasCompositionWidgetBase)
    {
        CipasCompositionWidgetBase->setWindowTitle(QApplication::translate("CipasCompositionWidgetBase", "Composition", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString::fromLocal8Bit("纸张和打印质量"));
		groupBox->setContentsMargins(0,0,0,0);
        textLabel3->setText(QString::fromLocal8Bit("大小"));
        textLabel5->setText(QString::fromLocal8Bit("单位"));
        textLabel7->setText(QString::fromLocal8Bit("方向"));
       // mPrintAsRasterCheckBox->setText(QApplication::translate("CipasCompositionWidgetBase", "Print as raster", 0, QApplication::UnicodeUTF8));
        mResolutionSpinBox->setSuffix(QApplication::translate("CipasCompositionWidgetBase", " dpi", 0, QApplication::UnicodeUTF8));
        mResolutionSpinBox->setPrefix(QString::fromLocal8Bit("打印质量"));
        mPaperWidthDoubleSpinBox->setPrefix(QString::fromLocal8Bit("宽度"));
        mPaperWidthDoubleSpinBox->setSuffix(QString());
        mPaperHeightDoubleSpinBox->setPrefix(QString::fromLocal8Bit("高度"));
        mPaperHeightDoubleSpinBox->setSuffix(QString());
        mSnapGroupBox->setTitle(QString::fromLocal8Bit("捕捉"));
        mSnapToGridCheckBox->setText(QString::fromLocal8Bit("捕捉到格网"));
        mGridResolutionSpinBox->setPrefix(QString::fromLocal8Bit("间隔"));
        mGridResolutionSpinBox->setSuffix(QString());
        mOffsetXSpinBox->setPrefix(QString::fromLocal8Bit("水平偏移量"));
        mOffsetYSpinBox->setPrefix(QString::fromLocal8Bit("垂直偏移量"));
        mPenWidthSpinBox->setPrefix(QString::fromLocal8Bit("画笔宽度"));
        mGridColorLabel->setText(QString::fromLocal8Bit("格网颜色"));
        //mGridColorButton->setText(QString());
        mGridStyleLabel->setText(QString::fromLocal8Bit("格网样式"));
		textFrameLineColor->setText(QString::fromLocal8Bit("边框线颜色"));
    } // retranslateUi

};

namespace Ui {
    class CipasCompositionWidgetBase: public Ui_CipasCompositionWidgetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPASCOMPOSITIONWIDGETBASE_H
