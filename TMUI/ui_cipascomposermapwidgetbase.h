/********************************************************************************
** Form generated from reading UI file 'qgscomposermapwidgetbase.ui'
**
** Created: Wed Aug 31 17:44:47 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPASCOMPOSERMAPWIDGETBASE_H
#define UI_CIPASCOMPOSERMAPWIDGETBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>
#include <QtGui/QButtonGroup>
#include "cipascolorbutton.h"



QT_BEGIN_NAMESPACE

class Ui_CipasComposerMapWidgetBase
{
public:
    QGridLayout *gridLayout_8;
    QToolBox *toolBox;
    QWidget *page;
    QGridLayout *gridLayout;
   // QGroupBox *groupBox;
   // QGridLayout *gridLayout_4;
   // QComboBox *mPreviewModeComboBox;
    //QPushButton *mUpdatePreviewButton;
    QGroupBox *mMapGroupBox;
    QGridLayout *gridLayout_2;
    QLabel *textLabel1;
    QLineEdit *mWidthLineEdit;
    QLabel *label_3;
    QLineEdit *mHeightLineEdit;
    //QLabel *label_2;
    //QLineEdit *mScaleLineEdit;
    QLabel *mRotationLabel;
    QSpinBox *mRotationSpinBox;
    QHBoxLayout *horizontalLayout_2;
    //QCheckBox *mKeepLayerListCheckBox;
    //QLabel *label;
   // QCheckBox *mDrawCanvasItemsCheckBox;
    QSpacerItem *verticalSpacer;
    QWidget *page_2;
    QGridLayout *gridLayout_5;
    QGroupBox *mMapExtentGroupBox;
    QGridLayout *gridLayout_3;
    QLabel *mXMinLabel;
    QLineEdit *mXMinLineEdit;
    QLabel *mXMaxLabel;
    QLineEdit *mXMaxLineEdit;
    QLabel *mYMinLabel;
    QLineEdit *mYMinLineEdit;
    QLabel *mYMaxLabel;
    QLineEdit *mYMaxLineEdit;
    QPushButton *mSetToMapCanvasExtentButton;
    QSpacerItem *verticalSpacer_3;
    QWidget *page_3;
    QGridLayout *gridLayout_7;
    QGroupBox *mGridCheckBox;
    QGridLayout *gridLayout_6;
    QLabel *mGridTypeLabel;
    QComboBox *mGridTypeComboBox;
    //QLabel *mIntervalXLabel;
    QDoubleSpinBox *mIntervalXSpinBox;
    //QLabel *mOffsetXLabel;
    QDoubleSpinBox *mOffsetXSpinBox;
   // QLabel *mLineWidthLabel;
	QButtonGroup *mLabelAxesButtonGroup;
    QDoubleSpinBox *mLineWidthSpinBox;
    QCheckBox *mDrawAnnotationCheckBox;
	QLabel *mGridPositionLabel;
    QLabel *mAnnotationPositionLabel;
    QComboBox *mAnnotationPositionComboBox;
	QComboBox *mAnnotationPositionComboBox2;
	QComboBox *mAnnotationStyleComboBox;
    QLabel *mAnnotationDirectionLabel;
    QComboBox *mAnnotationDirectionComboBox;
    QPushButton *mAnnotationFontButton;
   // QLabel *mDistanceToFrameLabel;
    QDoubleSpinBox *mDistanceToMapFrameSpinBox;
    //QLabel *mCoordinatePrecisionLabel;
    QSpinBox *mCoordinatePrecisionSpinBox;
   // QDoubleSpinBox *mCrossWidthSpinBox;
    QDoubleSpinBox *mOffsetYSpinBox;
   // QLabel *mIntervalYLabel;
    QDoubleSpinBox *mIntervalYSpinBox;
	QGroupBox *mLabelAxesGroupBox;
	QCheckBox *mLabelAxesTopChb;
	QCheckBox *mLabelAxesLeftChb;
	QCheckBox *mLabelAxesBottomChb;
	QCheckBox *mLabelAxesRightChb;
    //QLabel *mOffsetYLabel;
    //QLabel *mCrossWidthLabel;
    QSpacerItem *verticalSpacer_2;
    //QLabel *mLineColorLabel;
	QLabel *mAnnotationStyleLabel;
    CipasColorButton *mLineColorButton;
	CipasColorButton *mAnnotationColorButton;

    void setupUi(QWidget *CipasComposerMapWidgetBase)
    {
        if (CipasComposerMapWidgetBase->objectName().isEmpty())
            CipasComposerMapWidgetBase->setObjectName(QString::fromUtf8("CipasComposerMapWidgetBase"));
        CipasComposerMapWidgetBase->resize(404, 447);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CipasComposerMapWidgetBase->sizePolicy().hasHeightForWidth());
        CipasComposerMapWidgetBase->setSizePolicy(sizePolicy);
        gridLayout_8 = new QGridLayout(CipasComposerMapWidgetBase);
		gridLayout_8->setContentsMargins(0,0,0,0);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        toolBox = new QToolBox(CipasComposerMapWidgetBase);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 377, 414));
        gridLayout = new QGridLayout(page);
		gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        //groupBox = new QGroupBox(page);
        //groupBox->setObjectName(QString::fromUtf8("groupBox"));
        //gridLayout_4 = new QGridLayout(groupBox);
        //gridLayout_4->setSpacing(6);
        //gridLayout_4->setContentsMargins(11, 11, 11, 11);
        //gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        //mPreviewModeComboBox = new QComboBox(groupBox);
        //mPreviewModeComboBox->setObjectName(QString::fromUtf8("mPreviewModeComboBox"));
        //QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        //sizePolicy1.setHorizontalStretch(0);
        //sizePolicy1.setVerticalStretch(0);
        //sizePolicy1.setHeightForWidth(mPreviewModeComboBox->sizePolicy().hasHeightForWidth());
        //mPreviewModeComboBox->setSizePolicy(sizePolicy1);

        //gridLayout_4->addWidget(mPreviewModeComboBox, 0, 0, 1, 1);

        //mUpdatePreviewButton = new QPushButton(groupBox);
        //mUpdatePreviewButton->setObjectName(QString::fromUtf8("mUpdatePreviewButton"));

        //gridLayout_4->addWidget(mUpdatePreviewButton, 1, 0, 1, 1);


        //gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        mMapGroupBox = new QGroupBox(page);
        mMapGroupBox->setObjectName(QString::fromUtf8("mMapGroupBox"));
        gridLayout_2 = new QGridLayout(mMapGroupBox);
		gridLayout_2->setContentsMargins(0,0,0,0);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        textLabel1 = new QLabel(mMapGroupBox);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(true);

        gridLayout_2->addWidget(textLabel1, 0, 1, 1, 1);

        mWidthLineEdit = new QLineEdit(mMapGroupBox);
        mWidthLineEdit->setObjectName(QString::fromUtf8("mWidthLineEdit"));

        gridLayout_2->addWidget(mWidthLineEdit, 1, 1, 1, 1);

        label_3 = new QLabel(mMapGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setWordWrap(true);

        gridLayout_2->addWidget(label_3, 2, 1, 1, 1);

        mHeightLineEdit = new QLineEdit(mMapGroupBox);
        mHeightLineEdit->setObjectName(QString::fromUtf8("mHeightLineEdit"));

        gridLayout_2->addWidget(mHeightLineEdit, 3, 1, 1, 1);

        //label_2 = new QLabel(mMapGroupBox);
        //label_2->setObjectName(QString::fromUtf8("label_2"));
		QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
		sizePolicy2.setHorizontalStretch(0);
		sizePolicy2.setVerticalStretch(0);
		QSizePolicy sizePolicy9(QSizePolicy::Preferred, QSizePolicy::Preferred);
		sizePolicy9.setHorizontalStretch(0);
		sizePolicy9.setVerticalStretch(0);
		//sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        //label_2->setSizePolicy(sizePolicy2);
        //label_2->setWordWrap(true);

        //gridLayout_2->addWidget(label_2, 4, 1, 1, 1);

        //mScaleLineEdit = new QLineEdit(mMapGroupBox);
        //mScaleLineEdit->setObjectName(QString::fromUtf8("mScaleLineEdit"));

        //gridLayout_2->addWidget(mScaleLineEdit, 5, 1, 1, 1);

        mRotationLabel = new QLabel(mMapGroupBox);
        mRotationLabel->setObjectName(QString::fromUtf8("mRotationLabel"));
        mRotationLabel->setWordWrap(true);

        gridLayout_2->addWidget(mRotationLabel, 4, 1, 1, 1);

        mRotationSpinBox = new QSpinBox(mMapGroupBox);
        mRotationSpinBox->setObjectName(QString::fromUtf8("mRotationSpinBox"));
        mRotationSpinBox->setMaximum(359);

        gridLayout_2->addWidget(mRotationSpinBox, 5, 1, 1, 1);

        //horizontalLayout_2 = new QHBoxLayout();
        //horizontalLayout_2->setSpacing(6);
        //horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        //mKeepLayerListCheckBox = new QCheckBox(mMapGroupBox);
        //mKeepLayerListCheckBox->setObjectName(QString::fromUtf8("mKeepLayerListCheckBox"));
        //mKeepLayerListCheckBox->setMaximumSize(QSize(20, 16777215));

        //horizontalLayout_2->addWidget(mKeepLayerListCheckBox);

        //label = new QLabel(mMapGroupBox);
        //label->setObjectName(QString::fromUtf8("label"));
        //label->setWordWrap(true);

        //horizontalLayout_2->addWidget(label);


        //gridLayout_2->addLayout(horizontalLayout_2, 8, 0, 2, 2);

       /* mDrawCanvasItemsCheckBox = new QCheckBox(mMapGroupBox);
        mDrawCanvasItemsCheckBox->setObjectName(QString::fromUtf8("mDrawCanvasItemsCheckBox"));

        gridLayout_2->addWidget(mDrawCanvasItemsCheckBox, 10, 0, 1, 2);*/

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 8, 0, 1, 1);


        gridLayout->addWidget(mMapGroupBox, 0, 0, 1, 1);

        toolBox->addItem(page, QString::fromUtf8("Map"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 394, 356));
        gridLayout_5 = new QGridLayout(page_2);
		gridLayout_5->setContentsMargins(0,0,0,0);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        mMapExtentGroupBox = new QGroupBox(page_2);
        mMapExtentGroupBox->setObjectName(QString::fromUtf8("mMapExtentGroupBox"));
        gridLayout_3 = new QGridLayout(mMapExtentGroupBox);
		gridLayout_3->setContentsMargins(0,0,0,0);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        mXMinLabel = new QLabel(mMapExtentGroupBox);
        mXMinLabel->setObjectName(QString::fromUtf8("mXMinLabel"));
        mXMinLabel->setEnabled(true);
        mXMinLabel->setWordWrap(true);

        gridLayout_3->addWidget(mXMinLabel, 0, 0, 1, 1);

        mXMinLineEdit = new QLineEdit(mMapExtentGroupBox);
        mXMinLineEdit->setObjectName(QString::fromUtf8("mXMinLineEdit"));

        gridLayout_3->addWidget(mXMinLineEdit, 1, 0, 1, 1);

        mXMaxLabel = new QLabel(mMapExtentGroupBox);
        mXMaxLabel->setObjectName(QString::fromUtf8("mXMaxLabel"));
        mXMaxLabel->setWordWrap(true);

        gridLayout_3->addWidget(mXMaxLabel, 2, 0, 1, 1);

        mXMaxLineEdit = new QLineEdit(mMapExtentGroupBox);
        mXMaxLineEdit->setObjectName(QString::fromUtf8("mXMaxLineEdit"));

        gridLayout_3->addWidget(mXMaxLineEdit, 3, 0, 1, 1);

        mYMinLabel = new QLabel(mMapExtentGroupBox);
        mYMinLabel->setObjectName(QString::fromUtf8("mYMinLabel"));
        mYMinLabel->setWordWrap(true);

        gridLayout_3->addWidget(mYMinLabel, 4, 0, 1, 1);

        mYMinLineEdit = new QLineEdit(mMapExtentGroupBox);
        mYMinLineEdit->setObjectName(QString::fromUtf8("mYMinLineEdit"));

        gridLayout_3->addWidget(mYMinLineEdit, 5, 0, 1, 1);

        mYMaxLabel = new QLabel(mMapExtentGroupBox);
        mYMaxLabel->setObjectName(QString::fromUtf8("mYMaxLabel"));
        mYMaxLabel->setWordWrap(true);

        gridLayout_3->addWidget(mYMaxLabel, 6, 0, 1, 1);

        mYMaxLineEdit = new QLineEdit(mMapExtentGroupBox);
        mYMaxLineEdit->setObjectName(QString::fromUtf8("mYMaxLineEdit"));

        gridLayout_3->addWidget(mYMaxLineEdit, 7, 0, 1, 1);

        mSetToMapCanvasExtentButton = new QPushButton(mMapExtentGroupBox);
        mSetToMapCanvasExtentButton->setObjectName(QString::fromUtf8("mSetToMapCanvasExtentButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(mSetToMapCanvasExtentButton->sizePolicy().hasHeightForWidth());
        mSetToMapCanvasExtentButton->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(mSetToMapCanvasExtentButton, 8, 0, 1, 1);


        gridLayout_5->addWidget(mMapExtentGroupBox, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_3, 1, 0, 1, 1);
		page_2->hide();
        //toolBox->addItem(page_2, QString::fromUtf8("Extents"));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        page_3->setGeometry(QRect(0, 0, 377, 720));
        gridLayout_7 = new QGridLayout(page_3);
		gridLayout_7->setContentsMargins(0,0,0,0);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));


        mGridCheckBox = new QGroupBox(page_3);
        mGridCheckBox->setObjectName(QString::fromUtf8("mGridCheckBox"));
        mGridCheckBox->setCheckable(true);
        mGridCheckBox->setChecked(false);
        gridLayout_6 = new QGridLayout(mGridCheckBox);
		gridLayout_6->setContentsMargins(0,0,0,0);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
		//label：格网属性设置
        mGridTypeLabel = new QLabel(mGridCheckBox);
        mGridTypeLabel->setObjectName(QString::fromUtf8("mGridTypeLabel"));
        mGridTypeLabel->setWordWrap(true);

        gridLayout_6->addWidget(mGridTypeLabel, 0, 0, 1, 1);
		//cmb:线型
        mGridTypeComboBox = new QComboBox(mGridCheckBox);
        mGridTypeComboBox->setObjectName(QString::fromUtf8("mGridTypeComboBox"));
		mGridTypeComboBox->insertItem( 0, QString::fromLocal8Bit("实线") );
		mGridTypeComboBox->insertItem( 1, QString::fromLocal8Bit("虚线") );

        gridLayout_6->addWidget(mGridTypeComboBox, 0, 1, 1, 1);
		//// label: 线宽度
		//mLineWidthLabel = new QLabel(mGridCheckBox);
		//mLineWidthLabel->setObjectName(QString::fromUtf8("mLineWidthLabel"));
		//mLineWidthLabel->setWordWrap(true);

		//gridLayout_6->addWidget(mLineWidthLabel, 17, 0, 1, 1);
		//spinbox: 线宽度

		mLineWidthSpinBox = new QDoubleSpinBox(mGridCheckBox);
		mLineWidthSpinBox->setObjectName(QString::fromUtf8("mLineWidthSpinBox"));
		mLineWidthSpinBox->setPrefix(QString::fromLocal8Bit("线宽度"));
		mLineWidthSpinBox->setDecimals(5);

		gridLayout_6->addWidget(mLineWidthSpinBox, 1, 0, 1, 1);
		//label:线颜色
		//mLineColorLabel = new QLabel(mGridCheckBox);
		//mLineColorLabel->setObjectName(QString::fromUtf8("mLineColorLabel"));
		//mLineColorLabel->setWordWrap(true);

		//gridLayout_6->addWidget(mLineColorLabel, 2, 0, 1, 1);
		//button：setup color
		mLineColorButton = new CipasColorButton(mGridCheckBox);
		mLineColorButton->setObjectName(QString::fromUtf8("mLineColorButton"));
		sizePolicy2.setHeightForWidth(mLineColorButton->sizePolicy().hasHeightForWidth());
		mLineColorButton->setSizePolicy(sizePolicy2);

		gridLayout_6->addWidget(mLineColorButton, 1, 1, 1, 1);

        //mIntervalXLabel = new QLabel(mGridCheckBox);
        //mIntervalXLabel->setObjectName(QString::fromUtf8("mIntervalXLabel"));
        //mIntervalXLabel->setWordWrap(true);

        //gridLayout_6->addWidget(mIntervalXLabel, 2, 0, 1, 1);

        mIntervalXSpinBox = new QDoubleSpinBox(mGridCheckBox);
        mIntervalXSpinBox->setObjectName(QString::fromUtf8("mIntervalXSpinBox"));
		mIntervalXSpinBox->setPrefix(QString::fromLocal8Bit("X间隔"));
        mIntervalXSpinBox->setDecimals(5);
        mIntervalXSpinBox->setMaximum(1e+07);

        gridLayout_6->addWidget(mIntervalXSpinBox, 2, 0, 1, 1);


		//mIntervalYLabel = new QLabel(mGridCheckBox);
		//mIntervalYLabel->setObjectName(QString::fromUtf8("mIntervalYLabel"));
		//mIntervalYLabel->setWordWrap(true);

		//gridLayout_6->addWidget(mIntervalYLabel, 4, 0, 1, 1);

		mIntervalYSpinBox = new QDoubleSpinBox(mGridCheckBox);
		mIntervalYSpinBox->setObjectName(QString::fromUtf8("mIntervalYSpinBox"));
		mIntervalYSpinBox->setPrefix(QString::fromLocal8Bit("Y间隔"));
		mIntervalYSpinBox->setDecimals(5);
		mIntervalYSpinBox->setMaximum(1e+07);

		gridLayout_6->addWidget(mIntervalYSpinBox, 2, 1, 1, 1);


		//mOffsetXLabel = new QLabel(mGridCheckBox);
		//mOffsetXLabel->setObjectName(QString::fromUtf8("mOffsetXLabel"));
		//mOffsetXLabel->setWordWrap(true);

		//gridLayout_6->addWidget(mOffsetXLabel, 7, 0, 1, 1);

        mOffsetXSpinBox = new QDoubleSpinBox(mGridCheckBox);
        mOffsetXSpinBox->setObjectName(QString::fromUtf8("mOffsetXSpinBox"));
		mOffsetXSpinBox->setPrefix(QString::fromLocal8Bit("X偏移"));
        mOffsetXSpinBox->setDecimals(5);
        mOffsetXSpinBox->setMaximum(1e+07);
		mOffsetXSpinBox->setVisible(false);
        gridLayout_6->addWidget(mOffsetXSpinBox, 3, 0, 1, 1);

		mOffsetYSpinBox = new QDoubleSpinBox(mGridCheckBox);
		mOffsetYSpinBox->setObjectName(QString::fromUtf8("mOffsetYSpinBox"));
		mOffsetYSpinBox->setPrefix(QString::fromLocal8Bit("Y偏移"));
		mOffsetYSpinBox->setDecimals(5);
		mOffsetYSpinBox->setMaximum(1e+07);
		mOffsetYSpinBox->setVisible(false);
		gridLayout_6->addWidget(mOffsetYSpinBox, 3, 1, 1, 1);



		//mOffsetYLabel = new QLabel(mGridCheckBox);
		//mOffsetYLabel->setObjectName(QString::fromUtf8("mOffsetYLabel"));
		//mOffsetYLabel->setWordWrap(true);

		//gridLayout_6->addWidget(mOffsetYLabel, 9, 0, 1, 1);
		//label: 格网标注
		mGridPositionLabel = new QLabel(mGridCheckBox);
		mGridPositionLabel->setObjectName(QString::fromUtf8("mAnnotationPositionLabel"));
		mGridPositionLabel->setWordWrap(true);

		gridLayout_6->addWidget(mGridPositionLabel, 4, 0, 1, 1);
		//combo:格网方向
		mAnnotationPositionComboBox2 = new QComboBox(mGridCheckBox);
		//mCrossWidthLabel = new QLabel(mGridCheckBox);
		mAnnotationPositionComboBox2->setObjectName(QString::fromUtf8("mAnnotationPositionComboBox2"));
		//mCrossWidthLabel->setWordWrap(true);
		//下面顺序不要调成 返回时候 按照0 1 返回，界面也根据这个作为index 来设置当前项
		mAnnotationPositionComboBox2->insertItem( 0, QString::fromLocal8Bit("框架内") );
		mAnnotationPositionComboBox2->insertItem( 1, QString::fromLocal8Bit("框架外") );
		gridLayout_6->addWidget(mAnnotationPositionComboBox2, 4, 1, 1, 1);


		//ckb:是否绘制标注
        mDrawAnnotationCheckBox = new QCheckBox(mGridCheckBox);
        mDrawAnnotationCheckBox->setObjectName(QString::fromUtf8("mDrawAnnotationCheckBox"));

        gridLayout_6->addWidget(mDrawAnnotationCheckBox, 5, 0, 1, 1);

        mAnnotationPositionLabel = new QLabel(mGridCheckBox);
        mAnnotationPositionLabel->setObjectName(QString::fromUtf8("mAnnotationPositionLabel"));
        mAnnotationPositionLabel->setWordWrap(true);

        gridLayout_6->addWidget(mAnnotationPositionLabel, 6, 0, 1, 1);
		//cmb： 标注位置
        mAnnotationPositionComboBox = new QComboBox(mGridCheckBox);
        mAnnotationPositionComboBox->setObjectName(QString::fromUtf8("mAnnotationPositionComboBox"));
		mAnnotationPositionComboBox->insertItem( 0, QString::fromLocal8Bit("框架内") );
		mAnnotationPositionComboBox->insertItem( 1, QString::fromLocal8Bit("框架外") );
        gridLayout_6->addWidget(mAnnotationPositionComboBox, 6, 1, 1, 1);
		//goupbox: 标注轴
		mLabelAxesGroupBox=new QGroupBox(mGridCheckBox);
		mLabelAxesGroupBox->setTitle(QString::fromLocal8Bit("标注轴"));
		mLabelAxesTopChb=new QCheckBox(mGridCheckBox);
		mLabelAxesTopChb->setObjectName(QString::fromUtf8("mLabelAxesTopChb"));
		mLabelAxesTopChb->setChecked(false);
		mLabelAxesBottomChb=new QCheckBox(mGridCheckBox);
		mLabelAxesBottomChb->setObjectName(QString::fromUtf8("mLabelAxesBottomChb"));
		mLabelAxesBottomChb->setChecked(true);
		mLabelAxesLeftChb=new QCheckBox(mGridCheckBox);
		mLabelAxesLeftChb->setObjectName(QString::fromUtf8("mLabelAxesLeftChb"));
		mLabelAxesLeftChb->setChecked(true);
		mLabelAxesRightChb=new QCheckBox(mGridCheckBox);
		mLabelAxesRightChb->setObjectName(QString::fromUtf8("mLabelAxesRightChb"));
		mLabelAxesRightChb->setChecked(false);

		mLabelAxesButtonGroup=new QButtonGroup(mLabelAxesGroupBox);
		mLabelAxesButtonGroup->setObjectName(QString::fromUtf8("mLabelAxesButtonGroup"));
		mLabelAxesButtonGroup->setExclusive(false);
		mLabelAxesButtonGroup->addButton(mLabelAxesTopChb,0);
		mLabelAxesButtonGroup->addButton(mLabelAxesLeftChb,1);
		mLabelAxesButtonGroup->addButton(mLabelAxesBottomChb,2);
		mLabelAxesButtonGroup->addButton(mLabelAxesRightChb,3);

		QHBoxLayout *hlayoutLabelAxes = new QHBoxLayout(mLabelAxesGroupBox);
		hlayoutLabelAxes->setContentsMargins(0,0,0,0);
		hlayoutLabelAxes->addWidget(mLabelAxesTopChb);
		hlayoutLabelAxes->addWidget(mLabelAxesLeftChb);
		hlayoutLabelAxes->addWidget(mLabelAxesBottomChb);
		hlayoutLabelAxes->addWidget(mLabelAxesRightChb);

		gridLayout_6->addWidget(mLabelAxesGroupBox, 7, 0, 1, 2);
		//lebel: 标注方向
        mAnnotationDirectionLabel = new QLabel(mGridCheckBox);
        mAnnotationDirectionLabel->setObjectName(QString::fromUtf8("mAnnotationDirectionLabel"));
        mAnnotationDirectionLabel->setFrameShape(QFrame::NoFrame);
        mAnnotationDirectionLabel->setWordWrap(true);

        gridLayout_6->addWidget(mAnnotationDirectionLabel, 8, 0, 1, 1);
		//cmb; 标注方向选择
        mAnnotationDirectionComboBox = new QComboBox(mGridCheckBox);
        mAnnotationDirectionComboBox->setObjectName(QString::fromUtf8("mAnnotationDirectionComboBox"));
		mAnnotationDirectionComboBox->insertItem( 0, QString::fromLocal8Bit("水平"));
		mAnnotationDirectionComboBox->insertItem( 1, QString::fromLocal8Bit("垂直") );
		mAnnotationDirectionComboBox->insertItem( 2, QString::fromLocal8Bit("水平和垂直") );
		mAnnotationDirectionComboBox->insertItem( 2, QString::fromLocal8Bit("边界方向") );
        gridLayout_6->addWidget(mAnnotationDirectionComboBox, 8, 1, 1, 1);
		// 标注字体
        mAnnotationFontButton = new QPushButton(mGridCheckBox);
        mAnnotationFontButton->setObjectName(QString::fromUtf8("mAnnotationFontButton"));

        gridLayout_6->addWidget(mAnnotationFontButton, 9, 0, 1, 1);
		// 标注颜色
		mAnnotationColorButton = new CipasColorButton(mGridCheckBox);
		mAnnotationColorButton->setObjectName(QString::fromUtf8("mAnnotationColorButton"));
		sizePolicy9.setHeightForWidth(mAnnotationColorButton->sizePolicy().hasHeightForWidth());
		mAnnotationColorButton->setSizePolicy(sizePolicy9);
		 gridLayout_6->addWidget(mAnnotationColorButton, 9, 1, 1, 1);

        //mDistanceToFrameLabel = new QLabel(mGridCheckBox);
        //mDistanceToFrameLabel->setObjectName(QString::fromUtf8("mDistanceToFrameLabel"));
        //mDistanceToFrameLabel->setWordWrap(true);

        //gridLayout_6->addWidget(mDistanceToFrameLabel, 35, 0, 1, 1);

        mDistanceToMapFrameSpinBox = new QDoubleSpinBox(mGridCheckBox);
        mDistanceToMapFrameSpinBox->setObjectName(QString::fromUtf8("mDistanceToMapFrameSpinBox"));
		mDistanceToMapFrameSpinBox->setPrefix(QString::fromLocal8Bit("标注边框间距"));
        gridLayout_6->addWidget(mDistanceToMapFrameSpinBox, 10, 0, 1, 1);

        //mCoordinatePrecisionLabel = new QLabel(mGridCheckBox);
        //mCoordinatePrecisionLabel->setObjectName(QString::fromUtf8("mCoordinatePrecisionLabel"));
        //mCoordinatePrecisionLabel->setWordWrap(true);

        //gridLayout_6->addWidget(mCoordinatePrecisionLabel, 37, 0, 1, 1);

        mCoordinatePrecisionSpinBox = new QSpinBox(mGridCheckBox);
		mCoordinatePrecisionSpinBox->setPrefix(QString::fromLocal8Bit("坐标精度"));
        mCoordinatePrecisionSpinBox->setObjectName(QString::fromUtf8("mCoordinatePrecisionSpinBox"));

        gridLayout_6->addWidget(mCoordinatePrecisionSpinBox, 10, 1, 1, 1);

		mAnnotationStyleLabel = new QLabel(mGridCheckBox);
		mAnnotationStyleLabel->setObjectName(QString::fromUtf8("mAnnotationStyleLabel"));
		mAnnotationStyleLabel->setWordWrap(true);
		gridLayout_6->addWidget(mAnnotationStyleLabel, 11, 0, 1, 1);

		mAnnotationStyleComboBox = new QComboBox(mGridCheckBox);
		mAnnotationStyleComboBox->setObjectName(QString::fromUtf8("mAnnotationStyleComboBox"));
		mAnnotationStyleComboBox->insertItem(0,QString::fromLocal8Bit("无"));
		mAnnotationStyleComboBox->insertItem(1,QString::fromLocal8Bit("度"));
		//mAnnotationStyleComboBox->insertItem(1,QString::fromLocal8Bit("度分秒"));
		gridLayout_6->addWidget(mAnnotationStyleComboBox, 11, 1, 1, 1);
        //mCrossWidthSpinBox = new QDoubleSpinBox(mGridCheckBox);
        //mCrossWidthSpinBox->setObjectName(QString::fromUtf8("mCrossWidthSpinBox"));
        //mCrossWidthSpinBox->setDecimals(5);

        //gridLayout_6->addWidget(mCrossWidthSpinBox, 15, 0, 1, 1);





        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_2, 39, 0, 1, 1);




        gridLayout_7->addWidget(mGridCheckBox, 0, 0, 1, 1);

		page_3->hide();
        //toolBox->addItem(page_3, QString::fromUtf8("Grid"));

        gridLayout_8->addWidget(toolBox, 0, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        textLabel1->setBuddy(mWidthLineEdit);
        label_3->setBuddy(mHeightLineEdit);
       // label_2->setBuddy(mScaleLineEdit);
        mRotationLabel->setBuddy(mRotationSpinBox);
        mXMinLabel->setBuddy(mXMinLineEdit);
        mXMaxLabel->setBuddy(mXMaxLineEdit);
        mYMinLabel->setBuddy(mYMinLineEdit);
        mYMaxLabel->setBuddy(mYMaxLineEdit);
        mGridTypeLabel->setBuddy(mGridTypeComboBox);
       // mIntervalXLabel->setBuddy(mIntervalXSpinBox);
       // mOffsetXLabel->setBuddy(mOffsetXSpinBox);
       // mLineWidthLabel->setBuddy(mLineWidthSpinBox);
        mAnnotationPositionLabel->setBuddy(mAnnotationPositionComboBox);
        mAnnotationDirectionLabel->setBuddy(mAnnotationDirectionComboBox);
       // mDistanceToFrameLabel->setBuddy(mDistanceToMapFrameSpinBox);
     //   mCoordinatePrecisionLabel->setBuddy(mCoordinatePrecisionSpinBox);
      //  mIntervalYLabel->setBuddy(mIntervalYSpinBox);
       // mOffsetYLabel->setBuddy(mOffsetYSpinBox);
        //mCrossWidthLabel->setBuddy(mCrossWidthSpinBox);
        //mLineColorLabel->setBuddy(mLineColorButton);
#endif // QT_NO_SHORTCUT
        //QWidget::setTabOrder(mPreviewModeComboBox, mUpdatePreviewButton);
       /// QWidget::setTabOrder(mUpdatePreviewButton, mWidthLineEdit);
       // QWidget::setTabOrder(mWidthLineEdit, mHeightLineEdit);
       // QWidget::setTabOrder(mHeightLineEdit, mRotationSpinBox);
       //// QWidget::setTabOrder(mScaleLineEdit, mRotationSpinBox);
       // QWidget::setTabOrder(mRotationSpinBox, mXMinLineEdit);
       // //QWidget::setTabOrder(mKeepLayerListCheckBox, mXMinLineEdit);
       // QWidget::setTabOrder(mXMinLineEdit, mXMaxLineEdit);
       // QWidget::setTabOrder(mXMaxLineEdit, mYMinLineEdit);
       // QWidget::setTabOrder(mYMinLineEdit, mYMaxLineEdit);
       // QWidget::setTabOrder(mYMaxLineEdit, mSetToMapCanvasExtentButton);
       // QWidget::setTabOrder(mSetToMapCanvasExtentButton, mGridCheckBox);
       // QWidget::setTabOrder(mGridCheckBox, mGridTypeComboBox);
       // QWidget::setTabOrder(mGridTypeComboBox, mIntervalXSpinBox);
       // QWidget::setTabOrder(mIntervalXSpinBox, mIntervalYSpinBox);
       // QWidget::setTabOrder(mIntervalYSpinBox, mOffsetXSpinBox);
       // QWidget::setTabOrder(mOffsetXSpinBox, mOffsetYSpinBox);
       // QWidget::setTabOrder(mOffsetYSpinBox, mCrossWidthSpinBox);
       // QWidget::setTabOrder(mCrossWidthSpinBox, mLineWidthSpinBox);
       // QWidget::setTabOrder(mLineWidthSpinBox, mLineColorButton);
       // QWidget::setTabOrder(mLineColorButton, mDrawAnnotationCheckBox);
       // QWidget::setTabOrder(mDrawAnnotationCheckBox, mAnnotationPositionComboBox);
       // QWidget::setTabOrder(mAnnotationPositionComboBox, mAnnotationDirectionComboBox);
       // QWidget::setTabOrder(mAnnotationDirectionComboBox, mAnnotationFontButton);
       // QWidget::setTabOrder(mAnnotationFontButton, mDistanceToMapFrameSpinBox);
       // QWidget::setTabOrder(mDistanceToMapFrameSpinBox, mCoordinatePrecisionSpinBox);

        retranslateUi(CipasComposerMapWidgetBase);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CipasComposerMapWidgetBase);
    } // setupUi

    void retranslateUi(QWidget *CipasComposerMapWidgetBase)
    {
        CipasComposerMapWidgetBase->setWindowTitle(QString::fromLocal8Bit("地图选项"));
       // groupBox->setTitle(QString::fromLocal8Bit("预览"));
       // mUpdatePreviewButton->setText(QString::fromLocal8Bit("更新预览"));
        mMapGroupBox->setTitle(QString::fromLocal8Bit("当前项大小（毫米）"));
        textLabel1->setText(QString::fromLocal8Bit("宽度"));
        label_3->setText(QString::fromLocal8Bit("高度"));
        //label_2->setText(QString::fromLocal8Bit("缩放比例"));
       // mScaleLineEdit->setInputMask(QString());
        mRotationLabel->setText(QString::fromLocal8Bit("旋转"));
       // mKeepLayerListCheckBox->setText(QString());
        //label->setText(QString::fromLocal8Bit("锁定层为地图项"));
        //mDrawCanvasItemsCheckBox->setText(QString::fromLocal8Bit("绘制地图区域项"));
        toolBox->setItemText(toolBox->indexOf(page), QString::fromLocal8Bit("版面"));
        mMapExtentGroupBox->setTitle(QString::fromLocal8Bit("地图范围（经纬度）"));
        mXMinLabel->setText(QString::fromLocal8Bit("X 最小值"));
        mXMaxLabel->setText(QString::fromLocal8Bit("X 最大值"));
        mYMinLabel->setText(QString::fromLocal8Bit("Y 最小值"));
        mYMaxLabel->setText(QString::fromLocal8Bit("Y 最大值"));
        mSetToMapCanvasExtentButton->setText(QString::fromLocal8Bit("设置地图区域范围"));
        toolBox->setItemText(toolBox->indexOf(page_2), QString::fromLocal8Bit("地图"));
        mGridCheckBox->setTitle(QString::fromLocal8Bit("显示格网"));
#ifndef QT_NO_ACCESSIBILITY
        mGridTypeLabel->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        mGridTypeLabel->setText(QString::fromLocal8Bit("线类型"));
        //mIntervalXLabel->setText(QString::fromLocal8Bit("X间隔"));
        //mOffsetXLabel->setText(QString::fromLocal8Bit("X偏移"));
        //mLineWidthLabel->setText(QString::fromLocal8Bit("线宽度"));
		mGridPositionLabel->setText(QString::fromLocal8Bit("格网位置"));
        mDrawAnnotationCheckBox->setText(QString::fromLocal8Bit("绘制标注"));
        mAnnotationPositionLabel->setText(QString::fromLocal8Bit("标注位置"));
        mAnnotationDirectionLabel->setText(QString::fromLocal8Bit("标注方向"));
        mAnnotationFontButton->setText(QString::fromLocal8Bit("标注字体..."));
		mAnnotationColorButton->setText(QString::fromLocal8Bit("标注颜色..."));
		mAnnotationStyleLabel->setText(QString::fromLocal8Bit("标注样式"));
       // mDistanceToFrameLabel->setText(QString::fromLocal8Bit("到地图边框的距离"));
        //mCoordinatePrecisionLabel->setText(QString::fromLocal8Bit("坐标精度"));
		//mLabelAxesGroupBox->setTitle(QString::fromLocal8Bit("标注轴"));
		mLabelAxesTopChb->setText(QString::fromLocal8Bit("上"));
		mLabelAxesLeftChb->setText(QString::fromLocal8Bit("左"));
		mLabelAxesBottomChb->setText(QString::fromLocal8Bit("下"));
		mLabelAxesRightChb->setText(QString::fromLocal8Bit("右"));
        //mIntervalYLabel->setText(QString::fromLocal8Bit("Y间隔"));
        //mOffsetYLabel->setText(QString::fromLocal8Bit("Y偏移"));
        //mCrossWidthLabel->setText(QString::fromLocal8Bit("点宽度"));
       // mLineColorLabel->setText(QString::fromLocal8Bit("线颜色"));
        mLineColorButton->setText(QString::fromLocal8Bit("线颜色..."));
        toolBox->setItemText(toolBox->indexOf(page_3), QString::fromLocal8Bit("地图格网"));
    } // retranslateUi

};

namespace Ui {
    class CipasComposerMapWidgetBase: public Ui_CipasComposerMapWidgetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPASCOMPOSERMAPWIDGETBASE_H
