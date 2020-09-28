/********************************************************************************
** Form generated from reading UI file 'qgscomposerlegendwidgetbase.ui'
**
** Created: Wed Aug 31 17:44:46 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPASCOMPOSERLEGENDWIDGETBASE_H
#define UI_CIPASCOMPOSERLEGENDWIDGETBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolBox>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QComboBox>
#include <QtGui/QWidget>
#include <QObject>
QT_BEGIN_NAMESPACE

class Ui_CipasComposerLegendWidgetBase
{
public:
    QGridLayout *gridLayout_4;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QToolBox *toolBox;
    QWidget *page;
    QGridLayout *gridLayout;
    QLabel *mTitleLabel;
	//QLabel *mOrientationLabel;
	//QComboBox *mLegendOrientationComboBox;
	QCheckBox* mLegendItem2tempCheckBox;
	QCheckBox* mLegendStyle2templateCheckBox;
	QLabel *mStyleLabel;
	QComboBox *mLegendSytleComboBox;
    QLineEdit *mTitleLineEdit;
    QPushButton *mTitleFontButton;
   // QPushButton *mGroupFontButton;
    QPushButton *mLayerFontButton;
    QPushButton *mItemFontButton;
	QCheckBox *mCheckBoxLayerDraw;
	QPushButton *mSymbolOutlineColorButton;
	QDoubleSpinBox *mSymbolOutlineWidthSpinBox;
    QDoubleSpinBox *mSymbolWidthSpinBox;
	QDoubleSpinBox *mTotalSymbolWidthSpinBox;
	QDoubleSpinBox *mTotalSymbolHeightSpinBox;
    QDoubleSpinBox *mSymbolHeightSpinBox;
    QDoubleSpinBox *mLayerSpaceSpinBox;
    QDoubleSpinBox *mSymbolSpaceSpinBox;
    QDoubleSpinBox *mIconLabelSpaceSpinBox;
    QDoubleSpinBox *mBoxSpaceSpinBox;
    QSpacerItem *verticalSpacer;
    QWidget *page_2;
    QGridLayout *gridLayout_2;
    QCheckBox *mCheckBoxAutoUpdate;
	QTreeView *mItemTreeView;
	QToolButton *mMoveDownToolButton;
	QToolButton *mMoveUpToolButton;
	QToolButton *mAddToolButton;
	QToolButton *mRemoveToolButton;
	QToolButton *mEditPushButton;
	QToolButton *mUpdatePushButton;
    QToolButton *mUpdateAllPushButton;
   // QToolButton *mAddGroupButton;
	QLabel * mcolumnLabel;
	QSpinBox *mcolumnSpinbox;

    void setupUi(QWidget *CipasComposerLegendWidgetBase)
    {
        if (CipasComposerLegendWidgetBase->objectName().isEmpty())
            CipasComposerLegendWidgetBase->setObjectName(QString::fromUtf8("CipasComposerLegendWidgetBase"));
        CipasComposerLegendWidgetBase->resize(371, 476);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CipasComposerLegendWidgetBase->sizePolicy().hasHeightForWidth());
        CipasComposerLegendWidgetBase->setSizePolicy(sizePolicy);
        gridLayout_4 = new QGridLayout(CipasComposerLegendWidgetBase);
		gridLayout_4->setContentsMargins(0,0,0,0);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        scrollArea = new QScrollArea(CipasComposerLegendWidgetBase);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
		scrollAreaWidgetContents = new QWidget();
		scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
		scrollAreaWidgetContents->setGeometry(QRect(0, 0, 369, 474));
		gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
		gridLayout_3->setContentsMargins(0,0,0,0);
		gridLayout_3->setSpacing(6);
		gridLayout_3->setContentsMargins(11, 11, 11, 11);
		gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
		toolBox = new QToolBox(scrollAreaWidgetContents);
		toolBox->setObjectName(QString::fromUtf8("toolBox"));
		page = new QWidget();
		page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 338, 404));
        gridLayout = new QGridLayout(page);
		gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mTitleLabel = new QLabel(page);
        mTitleLabel->setObjectName(QString::fromUtf8("mTitleLabel"));

        gridLayout->addWidget(mTitleLabel, 0, 0, 1, 2);

        mTitleLineEdit = new QLineEdit(page);
        mTitleLineEdit->setObjectName(QString::fromUtf8("mTitleLineEdit"));

        gridLayout->addWidget(mTitleLineEdit, 1, 0, 1, 2);

        mTitleFontButton = new QPushButton(page);
        mTitleFontButton->setObjectName(QString::fromUtf8("mTitleFontButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mTitleFontButton->sizePolicy().hasHeightForWidth());
        mTitleFontButton->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(mTitleFontButton, 2, 0, 1, 2);

        //mGroupFontButton = new QPushButton(page);
        //mGroupFontButton->setObjectName(QString::fromUtf8("mGroupFontButton"));

        //gridLayout->addWidget(mGroupFontButton, 3, 0, 1, 1);

        mLayerFontButton = new QPushButton(page);
        mLayerFontButton->setObjectName(QString::fromUtf8("mLayerFontButton"));

        gridLayout->addWidget(mLayerFontButton, 3, 0, 1, 1);

		mCheckBoxLayerDraw = new QCheckBox(page);
		mCheckBoxLayerDraw->setObjectName(QString::fromUtf8("mCheckBoxLayerDraw"));
		mCheckBoxLayerDraw->setText(QString::fromLocal8Bit(("显示图层名")));
		gridLayout->addWidget(mCheckBoxLayerDraw,3,1,1,1);

        mItemFontButton = new QPushButton(page);
        mItemFontButton->setObjectName(QString::fromUtf8("mItemFontButton"));

        gridLayout->addWidget(mItemFontButton, 4, 0, 1, 2);

        mSymbolWidthSpinBox = new QDoubleSpinBox(page);
        mSymbolWidthSpinBox->setObjectName(QString::fromUtf8("mSymbolWidthSpinBox"));

        gridLayout->addWidget(mSymbolWidthSpinBox, 5, 0, 1, 1);

        mSymbolHeightSpinBox = new QDoubleSpinBox(page);
        mSymbolHeightSpinBox->setObjectName(QString::fromUtf8("mSymbolHeightSpinBox"));

        gridLayout->addWidget(mSymbolHeightSpinBox, 5, 1, 1, 1);

		mTotalSymbolWidthSpinBox = new QDoubleSpinBox(page);
		mTotalSymbolWidthSpinBox->setObjectName(QString::fromUtf8("mTotalSymbolWidthSpinBox"));
		mTotalSymbolWidthSpinBox->setMaximum(1000);
		gridLayout->addWidget(mTotalSymbolWidthSpinBox, 6, 0, 1, 1);

		mTotalSymbolHeightSpinBox = new QDoubleSpinBox(page);
		mTotalSymbolHeightSpinBox->setObjectName(QString::fromUtf8("mTotalSymbolHeightSpinBox"));
		mTotalSymbolHeightSpinBox->setMaximum(1000);
		gridLayout->addWidget(mTotalSymbolHeightSpinBox, 6, 1, 1, 1);

		mSymbolOutlineColorButton=new QPushButton(page);
		mSymbolOutlineColorButton->setObjectName(QString::fromUtf8("mSymbolOutlineColorButton"));

		gridLayout->addWidget(mSymbolOutlineColorButton, 7, 0, 1, 2);

		mSymbolOutlineWidthSpinBox=new QDoubleSpinBox(page);
		mSymbolOutlineWidthSpinBox->setObjectName(QString::fromUtf8("mSymbolOutlineWidthSpinBox"));

		gridLayout->addWidget(mSymbolOutlineWidthSpinBox, 8, 0, 1, 2);

        mLayerSpaceSpinBox = new QDoubleSpinBox(page);
        mLayerSpaceSpinBox->setObjectName(QString::fromUtf8("mLayerSpaceSpinBox"));

        gridLayout->addWidget(mLayerSpaceSpinBox, 9, 0, 1, 2);

        mSymbolSpaceSpinBox = new QDoubleSpinBox(page);
        mSymbolSpaceSpinBox->setObjectName(QString::fromUtf8("mSymbolSpaceSpinBox"));

        gridLayout->addWidget(mSymbolSpaceSpinBox, 10, 0, 1, 2);

        mIconLabelSpaceSpinBox = new QDoubleSpinBox(page);
        mIconLabelSpaceSpinBox->setObjectName(QString::fromUtf8("mIconLabelSpaceSpinBox"));

		gridLayout->addWidget(mIconLabelSpaceSpinBox, 11, 0, 1, 2);

        mBoxSpaceSpinBox = new QDoubleSpinBox(page);
        mBoxSpaceSpinBox->setObjectName(QString::fromUtf8("mBoxSpaceSpinBox"));

        gridLayout->addWidget(mBoxSpaceSpinBox, 12, 0, 1, 2);
		/*
		mOrientationLabel=new QLabel(page);
		mOrientationLabel->setObjectName(QString::fromUtf8("mOrientationLabel"));
		gridLayout->addWidget(mOrientationLabel,14,0,1,1);

	
		mLegendOrientationComboBox=new QComboBox(page);
		mLegendOrientationComboBox->setObjectName(QString::fromUtf8("mLegendOrientationComboBox"));
		mLegendOrientationComboBox->setEnabled(true);

		gridLayout->addWidget(mLegendOrientationComboBox,15,0,1,1);	*/	
		

		mStyleLabel=new QLabel(page);
		mStyleLabel->setObjectName(QString::fromUtf8("mStyleLabel"));
		gridLayout->addWidget(mStyleLabel,13,0,1,2);

		

		mLegendSytleComboBox=new QComboBox(page);
		mLegendSytleComboBox->setObjectName(QString::fromUtf8("mLegendSytleComboBox"));
		mLegendSytleComboBox->setEnabled(true);

		gridLayout->addWidget(mLegendSytleComboBox,14,0,1,2);
		//保存图例内容到模板，用于图例不变情况 如 ： 降水预报，温度预报等
		mLegendItem2tempCheckBox = new QCheckBox;		
		mLegendItem2tempCheckBox->setObjectName(QString::fromUtf8("mLegendItem2tempCheckBox"));
		mLegendItem2tempCheckBox->setChecked(false);
		gridLayout->addWidget(mLegendItem2tempCheckBox,15,0,1,1);
		//保存图例样式到模板，用于图例样式不变，但内容由图层决定
		mLegendStyle2templateCheckBox = new QCheckBox;		
		mLegendStyle2templateCheckBox->setObjectName(QString::fromUtf8("mLegendStyle2templateCheckBox"));
		mLegendStyle2templateCheckBox->setChecked(false);
		gridLayout->addWidget(mLegendStyle2templateCheckBox,16,0,1,2);

		mcolumnLabel = new QLabel;
		mcolumnLabel->setObjectName(QString::fromUtf8("mcolumnLabel"));
		gridLayout->addWidget(mcolumnLabel,17,0,1,1);
		mcolumnSpinbox =  new QSpinBox;
		mcolumnSpinbox->setObjectName(QString::fromUtf8("mcolumnSpinbox"));
		mcolumnSpinbox->setValue(1);
		gridLayout->addWidget(mcolumnSpinbox,17,1,1,1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);


        gridLayout->addItem(verticalSpacer, 17, 0, 1, 2);


        toolBox->addItem(page, QString::fromUtf8("General"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 340, 390));
        gridLayout_2 = new QGridLayout(page_2);
		gridLayout_2->setContentsMargins(0,0,0,0);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        mCheckBoxAutoUpdate = new QCheckBox(page_2);
        mCheckBoxAutoUpdate->setObjectName(QString::fromUtf8("mCheckBoxAutoUpdate"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(mCheckBoxAutoUpdate->sizePolicy().hasHeightForWidth());
        mCheckBoxAutoUpdate->setSizePolicy(sizePolicy2);
        mCheckBoxAutoUpdate->setChecked(true);
		mCheckBoxAutoUpdate->setVisible(false);
        gridLayout_2->addWidget(mCheckBoxAutoUpdate, 0, 0, 1, 4);

        mItemTreeView = new QTreeView(page_2);
        mItemTreeView->setObjectName(QString::fromUtf8("mItemTreeView"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(mItemTreeView->sizePolicy().hasHeightForWidth());
        mItemTreeView->setSizePolicy(sizePolicy3);
        mItemTreeView->setWordWrap(false);
        mItemTreeView->setHeaderHidden(true);
        mItemTreeView->header()->setVisible(false);

        gridLayout_2->addWidget(mItemTreeView, 1, 0, 1, 8);

        mMoveDownToolButton = new QToolButton(page_2);
        mMoveDownToolButton->setObjectName(QString::fromUtf8("mMoveDownToolButton"));

        gridLayout_2->addWidget(mMoveDownToolButton, 2, 0, 1, 1);

        mMoveUpToolButton = new QToolButton(page_2);
        mMoveUpToolButton->setObjectName(QString::fromUtf8("mMoveUpToolButton"));

        gridLayout_2->addWidget(mMoveUpToolButton, 2, 1, 1, 1);

        mAddToolButton = new QToolButton(page_2);
        mAddToolButton->setObjectName(QString::fromUtf8("mAddToolButton"));

        gridLayout_2->addWidget(mAddToolButton, 2, 2, 1, 1);

        mRemoveToolButton = new QToolButton(page_2);
        mRemoveToolButton->setObjectName(QString::fromUtf8("mRemoveToolButton"));

        gridLayout_2->addWidget(mRemoveToolButton, 2, 3, 1, 1);

        mEditPushButton = new QToolButton(page_2);
        mEditPushButton->setObjectName(QString::fromUtf8("mEditPushButton"));
		mEditPushButton->setVisible(false);
        gridLayout_2->addWidget(mEditPushButton, 2, 4, 1, 1);

        mUpdatePushButton = new QToolButton(page_2);
		mUpdatePushButton->setVisible(false);
        mUpdatePushButton->setObjectName(QString::fromUtf8("mUpdatePushButton"));
        gridLayout_2->addWidget(mUpdatePushButton, 2, 5, 1, 1);

        mUpdateAllPushButton = new QToolButton(page_2);
        mUpdateAllPushButton->setObjectName(QString::fromUtf8("mUpdateAllPushButton"));
		mUpdateAllPushButton->setVisible(false);
        gridLayout_2->addWidget(mUpdateAllPushButton, 2, 6, 1, 1);

        //mAddGroupButton = new QToolButton(page_2);
        //mAddGroupButton->setObjectName(QString::fromUtf8("mAddGroupButton"));

        //gridLayout_2->addWidget(mAddGroupButton, 2, 7, 1, 1);

        toolBox->addItem(page_2, QString::fromUtf8("Legend items"));

        gridLayout_3->addWidget(toolBox, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_4->addWidget(scrollArea, 0, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        mTitleLabel->setBuddy(mTitleLineEdit);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(scrollArea, mTitleLineEdit);
        QWidget::setTabOrder(mTitleLineEdit, mTitleFontButton);
        QWidget::setTabOrder(mTitleFontButton, mLayerFontButton);
       // QWidget::setTabOrder(mGroupFontButton, mLayerFontButton);
        QWidget::setTabOrder(mLayerFontButton, mItemFontButton);
        QWidget::setTabOrder(mItemFontButton, mSymbolWidthSpinBox);
        QWidget::setTabOrder(mSymbolWidthSpinBox, mSymbolHeightSpinBox);
        QWidget::setTabOrder(mSymbolHeightSpinBox, mLayerSpaceSpinBox);
        QWidget::setTabOrder(mLayerSpaceSpinBox, mSymbolSpaceSpinBox);
        QWidget::setTabOrder(mSymbolSpaceSpinBox, mIconLabelSpaceSpinBox);
        QWidget::setTabOrder(mIconLabelSpaceSpinBox, mBoxSpaceSpinBox);
        QWidget::setTabOrder(mBoxSpaceSpinBox, mItemTreeView);
        QWidget::setTabOrder(mItemTreeView, mMoveDownToolButton);
        QWidget::setTabOrder(mMoveDownToolButton, mMoveUpToolButton);
        QWidget::setTabOrder(mMoveUpToolButton, mRemoveToolButton);
        QWidget::setTabOrder(mRemoveToolButton, mEditPushButton);
        QWidget::setTabOrder(mEditPushButton, mUpdatePushButton);
        QWidget::setTabOrder(mUpdatePushButton, mUpdateAllPushButton);
        //QWidget::setTabOrder(mUpdateAllPushButton, mAddGroupButton);

        retranslateUi(CipasComposerLegendWidgetBase);

        toolBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(CipasComposerLegendWidgetBase);
    } // setupUi

    void retranslateUi(QWidget *CipasComposerLegendWidgetBase)
    {
        CipasComposerLegendWidgetBase->setWindowTitle(QApplication::translate("CipasComposerLegendWidgetBase", "Barscale Options", 0, QApplication::UnicodeUTF8));
        //mTitleLabel->setText(QApplication::translate("CipasComposerLegendWidgetBase", "&Title", 0, QApplication::UnicodeUTF8));
		mTitleLabel->setText(QString::fromLocal8Bit("标题"));
        mTitleFontButton->setText(QString::fromLocal8Bit("标题字体..."));
       // mGroupFontButton->setText(QApplication::translate("CipasComposerLegendWidgetBase", "Group Font...", 0, QApplication::UnicodeUTF8));
        mLayerFontButton->setText(QString::fromLocal8Bit("图层字体..."));
        mItemFontButton->setText(QString::fromLocal8Bit("项目字体..."));
        mSymbolWidthSpinBox->setPrefix(QString::fromLocal8Bit("符号宽度"));
        mSymbolWidthSpinBox->setSuffix(QApplication::translate("CipasComposerLegendWidgetBase", " mm", 0, QApplication::UnicodeUTF8));
		mTotalSymbolWidthSpinBox->setPrefix(QString::fromLocal8Bit("符号总宽度"));
		mTotalSymbolWidthSpinBox->setSuffix(QApplication::translate("CipasComposerLegendWidgetBase", " mm", 0, QApplication::UnicodeUTF8));
		mSymbolHeightSpinBox->setPrefix(QString::fromLocal8Bit("符号高度"));
        mSymbolHeightSpinBox->setSuffix(QApplication::translate("CipasComposerLegendWidgetBase", " mm", 0, QApplication::UnicodeUTF8));
		mTotalSymbolHeightSpinBox->setPrefix(QString::fromLocal8Bit("符号总高度"));
		mTotalSymbolHeightSpinBox->setSuffix(QApplication::translate("CipasComposerLegendWidgetBase", " mm", 0, QApplication::UnicodeUTF8));

		mSymbolOutlineColorButton->setText(QString::fromLocal8Bit("符号外包线颜色..."));
		mSymbolOutlineWidthSpinBox->setPrefix(QString::fromLocal8Bit("符号外包线宽度"));
		mSymbolOutlineWidthSpinBox->setSuffix(QApplication::translate("CipasComposerLegendWidgetBase", " mm", 0, QApplication::UnicodeUTF8));
        mLayerSpaceSpinBox->setPrefix(QString::fromLocal8Bit("图层间距"));
        mLayerSpaceSpinBox->setSuffix(QApplication::translate("CipasComposerLegendWidgetBase", " mm", 0, QApplication::UnicodeUTF8));
        mSymbolSpaceSpinBox->setPrefix(QString::fromLocal8Bit("符号间距"));
        mSymbolSpaceSpinBox->setSuffix(QApplication::translate("CipasComposerLegendWidgetBase", " mm", 0, QApplication::UnicodeUTF8));
        mIconLabelSpaceSpinBox->setPrefix(QString::fromLocal8Bit("符号标注间距"));
        mIconLabelSpaceSpinBox->setSuffix(QApplication::translate("CipasComposerLegendWidgetBase", " mm", 0, QApplication::UnicodeUTF8));
        mBoxSpaceSpinBox->setPrefix(QString::fromLocal8Bit("边距"));
        mBoxSpaceSpinBox->setSuffix(QApplication::translate("CipasComposerLegendWidgetBase", " mm", 0, QApplication::UnicodeUTF8));
		//mOrientationLabel->setText(QString::fromLocal8Bit("图例方向"));
		mStyleLabel->setText(QString::fromLocal8Bit("图例样式"));
		mLegendItem2tempCheckBox->setText(QString::fromLocal8Bit("保存图例项内容到模板"));
		mLegendStyle2templateCheckBox->setText(QObject::tr("保存图例样式到模板"));
        toolBox->setItemText(toolBox->indexOf(page), QString::fromLocal8Bit("常规"));
		mcolumnLabel->setText(QString::fromLocal8Bit("列数:"));

		//next item
        mCheckBoxAutoUpdate->setText(QString::fromLocal8Bit("自动更新"));
        mMoveDownToolButton->setText(QString());
        mMoveUpToolButton->setText(QString());
        mAddToolButton->setText(QString());
        mRemoveToolButton->setText(QString());
        mEditPushButton->setText(QString());
        mUpdatePushButton->setText(QString::fromLocal8Bit("重置"));
        mUpdateAllPushButton->setText(QString::fromLocal8Bit("全部"));

       // mAddGroupButton->setText(QApplication::translate("CipasComposerLegendWidgetBase", "Add group", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_2), QString::fromLocal8Bit("图例项目"));
    } // retranslateUi

};

namespace Ui {
    class CipasComposerLegendWidgetBase: public Ui_CipasComposerLegendWidgetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPASCOMPOSERLEGENDWIDGETBASE_H
