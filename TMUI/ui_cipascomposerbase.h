/********************************************************************************
** Form generated from reading UI file 'qgscomposerbase.ui'
**
** Created: Wed Aug 31 17:44:46 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPASSCOMPOSERBASE_H
#define UI_CIPASSCOMPOSERBASE_H


#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QSplitter>
#include <QtGui/QStackedWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <QtGui/QToolBox>
#include <QtGui/QScrollArea>
#include <QtGui/QGroupBox>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QToolButton>
#include <QObject>
#include <QtGui/QDockWidget>
#include "CipasTemplateTree.h"

QT_BEGIN_NAMESPACE

class Ui_CipasComposerBase
{
public:
	QAction *mActionPrint;
	QAction *mActionZoomAll;
	QAction *mActionZoomIn;
	QAction *mActionZoomOut;
	QAction *mActionExport2Clipboard;
	QAction *mActionAddNewLabel;
	QAction *mActionAddNewLegend;
	QAction *mActionSelectMoveItem;
	QAction *mActionExportAsImage;
	QAction *mActionExportAsPDF;
	QAction *mActionExportAsEPS;
	QAction *mActionExportAsSVG;
	QAction *mActionAddNewScalebar;
	QAction *mActionRefreshView;
	QAction *mActionAddImage;
	//QAction *mActionAddGLW;
	//QAction *mActionMoveItemContent;
	//QAction *mActionMapZoomAll;
	//QAction *mActionMapZoomIn;
	//QAction *mActionMapZoomOut;
	QAction *mActionGroupItems;
	QAction *mActionUngroupItems;
	QAction *mActionRaiseItems;
	QAction *mActionLowerItems;
	QAction *mActionMoveItemsToTop;
	QAction *mActionMoveItemsToBottom;
	QAction *mActionLoadFromTemplate;
	QAction *mActionSaveAsTemplate;
	QAction *mActionAlignLeft;
	QAction *mActionAlignHCenter;
	QAction *mActionAlignRight;
	QAction *mActionAlignTop;
	QAction *mActionAlignVCenter;
	QAction *mActionAlignBottom;
	QAction *mActionQuit;
	QAction *mActionAddBasicShape;
	QAction *mActionAddArrow;
	//QAction *mActionAddTable;
	QAction *mActionPageSetup;
	//QAction *mActionUndo;
	//QAction *mActionRedo;
	QWidget *centralwidget;
	QGridLayout *gridLayout_1;
	QSplitter *mSplitter;
	QFrame *mViewFrame;
	QTabWidget *mOptionsTabWidget;
	QWidget *tab;
	QGridLayout *gridLayout;
	QFrame *mCompositionOptionsFrame;
	QComboBox *mCompositionNameComboBox;
	QLabel *textLabel1;
	QWidget *tab_2;
	QGridLayout *gridLayout1;
	QStackedWidget *mItemStackedWidget;
	QWidget *page;
	QWidget *page_2;
	//QDialogButtonBox *mButtonBox;
	QToolBar *toolBar;
	//QToolBar *toolBarMap;
	QToolBar *toolBarGen;
	QGridLayout *gridLayout_all;
	QScrollArea *scrollArea;
	QWidget *scrollAreaWidgetContents;
	QGridLayout *gridLayout_t;
	QWidget *tab_3;
	QToolBox *toolBoxTemplate;
	QWidget *page_c;
	QWidget *page_DB;
	QGroupBox *groupboxC;
	QGroupBox *groupboxT;
	QPlainTextEdit *textEditC;
	QPlainTextEdit *textEditC_1;
	QLabel *label_ct;
	QSpacerItem *hSpacer;
	QSpacerItem *vSpacer;
	QPushButton *newBlankT;
	QPushButton *saveT;
	QPushButton *saveAsT;
	QLabel *label_T_content;
	CCipasTemplateTree *mCipasTemplateTree;
	QTreeView *mTemplateTreeView;
	//QToolButton *mMoveDownToolButton;
	//QToolButton *mMoveUpToolButton;
	QToolButton *mAddToolButton;
	QToolButton *mRemoveToolButton;
	QToolButton *mEditToolButton;
	QPushButton *mSavePushButton;
	QPushButton *mOpenPushButton;
	QDockWidget *dockProperty;
	QDockWidget *dockTemplate;
	//WId widDockProperty;
	//WId widDockTemplate;
	//WId widtoolbar;
	//WId widmaptoolbar;
	//WId widGentoolbar;

	void setupUi(QMainWindow *CipasComposerBase)
	{
		if (CipasComposerBase->objectName().isEmpty())
			CipasComposerBase->setObjectName(QString::fromUtf8("CipasComposerBase"));
		CipasComposerBase->resize(1100, 600);
		CipasComposerBase->setMouseTracking(true);
		mActionPrint = new QAction(CipasComposerBase);
		mActionPrint->setObjectName(QString::fromUtf8("mActionPrint"));
		QIcon icon;
		icon.addFile(QString::fromUtf8(":/images/thematicMap/mActionFilePrint.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionPrint->setIcon(icon);
		mActionZoomAll = new QAction(CipasComposerBase);
		mActionZoomAll->setObjectName(QString::fromUtf8("mActionZoomAll"));
		QIcon icon1;
		icon1.addFile(QString::fromUtf8(":/images/thematicMap/mActionZoomFullExtent.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionZoomAll->setIcon(icon1);
		mActionZoomIn = new QAction(CipasComposerBase);
		mActionZoomIn->setObjectName(QString::fromUtf8("mActionZoomIn"));
		QIcon icon2;
		icon2.addFile(QString::fromUtf8(":/images/thematicMap/mActionZoomIn.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionZoomIn->setIcon(icon2);
		mActionZoomOut = new QAction(CipasComposerBase);
		mActionZoomOut->setObjectName(QString::fromUtf8("mActionZoomOut"));
		QIcon icon3;
		icon3.addFile(QString::fromUtf8(":/images/thematicMap/mActionZoomOut.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionZoomOut->setIcon(icon3);

		//mActionAddGLW = new QAction(CipasComposerBase);
		//mActionAddGLW->setObjectName(QString::fromUtf8("mActionAddGLW"));
		//QIcon icon4;
		//icon4.addFile(QString::fromUtf8(":/images/thematicMap/mActionAddMap.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		//mActionAddGLW->setIcon(icon4);


		mActionExport2Clipboard = new QAction(CipasComposerBase);
		mActionExport2Clipboard->setObjectName(QString::fromUtf8("mActionExport2Clipboard"));
		//QIcon icon4;
		//icon4.addFile(QString::fromUtf8(":/images/thematicMap/mActionAddMap.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		//mActionAddNewMap->setIcon(icon4);
		mActionAddNewLabel = new QAction(CipasComposerBase);
		mActionAddNewLabel->setObjectName(QString::fromUtf8("mActionAddNewLabel"));
		QIcon icon5;
		icon5.addFile(QString::fromUtf8(":/images/thematicMap/mActionLabel.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionAddNewLabel->setIcon(icon5);
		mActionAddNewLegend = new QAction(CipasComposerBase);
		mActionAddNewLegend->setObjectName(QString::fromUtf8("mActionAddNewLegend"));
		QIcon icon6;
		icon6.addFile(QString::fromUtf8(":/images/thematicMap/mActionAddLegend.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionAddNewLegend->setIcon(icon6);
		mActionSelectMoveItem = new QAction(CipasComposerBase);
		mActionSelectMoveItem->setObjectName(QString::fromUtf8("mActionSelectMoveItem"));
		QIcon icon7;
		icon7.addFile(QString::fromUtf8(":/images/thematicMap/mActionSelectMoveItem.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionSelectMoveItem->setIcon(icon7);
		mActionExportAsImage = new QAction(CipasComposerBase);
		mActionExportAsImage->setObjectName(QString::fromUtf8("mActionExportAsImage"));
		QIcon icon8;
		icon8.addFile(QString::fromUtf8(":/images/thematicMap/mActionSaveMapAsImage.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionExportAsImage->setIcon(icon8);
		mActionExportAsPDF = new QAction(CipasComposerBase);
		mActionExportAsPDF->setObjectName(QString::fromUtf8("mActionExportAsPDF"));
		QIcon icon9;
		icon9.addFile(QString::fromUtf8(":/images/thematicMap/mActionSaveAsPDF.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionExportAsPDF->setIcon(icon9);


		mActionExportAsEPS = new QAction(CipasComposerBase);
		mActionExportAsEPS->setObjectName(QString::fromUtf8("mActionExportAsEPS"));
		QIcon icon10;
		icon10.addFile(QString::fromUtf8(":/images/thematicMap/mActionExportAsEPS.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionExportAsEPS->setIcon(icon10);

		mActionExportAsSVG = new QAction(CipasComposerBase);
		mActionExportAsSVG->setObjectName(QString::fromUtf8("mActionExportAsSVG"));
		//QIcon icon10;
		//icon10.addFile(QString::fromUtf8(":/images/thematicMap/mActionSaveAsSVG.png"), QSize(), QIcon::Normal, QIcon::Off);
		//mActionExportAsSVG->setIcon(icon10);
		mActionAddNewScalebar = new QAction(CipasComposerBase);
		mActionAddNewScalebar->setObjectName(QString::fromUtf8("mActionAddNewScalebar"));
		QIcon icon11;
		icon11.addFile(QString::fromUtf8(":/images/thematicMap/mActionAddNewScalebar.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionAddNewScalebar->setIcon(icon11);
		mActionRefreshView = new QAction(CipasComposerBase);
		mActionRefreshView->setObjectName(QString::fromUtf8("mActionRefreshView"));
		QIcon icon12;
		icon12.addFile(QString::fromUtf8(":/images/thematicMap/mActionRefreshView.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionRefreshView->setIcon(icon12);
		mActionAddImage = new QAction(CipasComposerBase);
		mActionAddImage->setObjectName(QString::fromUtf8("mActionAddImage"));
		QIcon icon13;
		icon13.addFile(QString::fromUtf8(":/images/thematicMap/mActionAddImage.png"),  QSize(50,50), QIcon::Normal, QIcon::Off);
		mActionAddImage->setIcon(icon13);

		//mActionMoveItemContent = new QAction(CipasComposerBase);
		//mActionMoveItemContent->setObjectName(QString::fromUtf8("mActionMoveItemContent"));
		//mActionMapZoomAll = new QAction(CipasComposerBase);
		//mActionMapZoomAll->setObjectName(QString::fromUtf8("mActionMapZoomAll"));
		//mActionMapZoomIn = new QAction(CipasComposerBase);
		//mActionMapZoomIn->setObjectName(QString::fromUtf8("mActionMapZoomIn"));
		//mActionMapZoomOut = new QAction(CipasComposerBase);
		//mActionMapZoomOut->setObjectName(QString::fromUtf8("mActionMapZoomOut"));

		mActionGroupItems = new QAction(CipasComposerBase);
		mActionGroupItems->setObjectName(QString::fromUtf8("mActionGroupItems"));
		mActionUngroupItems = new QAction(CipasComposerBase);
		mActionUngroupItems->setObjectName(QString::fromUtf8("mActionUngroupItems"));
		mActionRaiseItems = new QAction(CipasComposerBase);
		mActionRaiseItems->setObjectName(QString::fromUtf8("mActionRaiseItems"));
		mActionLowerItems = new QAction(CipasComposerBase);
		mActionLowerItems->setObjectName(QString::fromUtf8("mActionLowerItems"));
		mActionMoveItemsToTop = new QAction(CipasComposerBase);
		mActionMoveItemsToTop->setObjectName(QString::fromUtf8("mActionMoveItemsToTop"));
		mActionMoveItemsToBottom = new QAction(CipasComposerBase);
		mActionMoveItemsToBottom->setObjectName(QString::fromUtf8("mActionMoveItemsToBottom"));
		mActionLoadFromTemplate = new QAction(CipasComposerBase);
		mActionLoadFromTemplate->setObjectName(QString::fromUtf8("mActionLoadFromTemplate"));
		mActionSaveAsTemplate = new QAction(CipasComposerBase);
		mActionSaveAsTemplate->setObjectName(QString::fromUtf8("mActionSaveAsTemplate"));
		mActionAlignLeft = new QAction(CipasComposerBase);
		mActionAlignLeft->setObjectName(QString::fromUtf8("mActionAlignLeft"));
		mActionAlignHCenter = new QAction(CipasComposerBase);
		mActionAlignHCenter->setObjectName(QString::fromUtf8("mActionAlignHCenter"));
		mActionAlignRight = new QAction(CipasComposerBase);
		mActionAlignRight->setObjectName(QString::fromUtf8("mActionAlignRight"));
		mActionAlignTop = new QAction(CipasComposerBase);
		mActionAlignTop->setObjectName(QString::fromUtf8("mActionAlignTop"));
		mActionAlignVCenter = new QAction(CipasComposerBase);
		mActionAlignVCenter->setObjectName(QString::fromUtf8("mActionAlignVCenter"));
		mActionAlignBottom = new QAction(CipasComposerBase);
		mActionAlignBottom->setObjectName(QString::fromUtf8("mActionAlignBottom"));
		mActionQuit = new QAction(CipasComposerBase);
		mActionQuit->setObjectName(QString::fromUtf8("mActionQuit"));
		mActionAddBasicShape = new QAction(CipasComposerBase);
		mActionAddBasicShape->setObjectName(QString::fromUtf8("mActionAddBasicShape"));
		mActionAddBasicShape->setCheckable(true);
		mActionAddArrow = new QAction(CipasComposerBase);
		mActionAddArrow->setObjectName(QString::fromUtf8("mActionAddArrow"));
		mActionAddArrow->setCheckable(true);
		//mActionAddTable = new QAction(CipasComposerBase);
		//mActionAddTable->setObjectName(QString::fromUtf8("mActionAddTable"));
		//mActionAddTable->setCheckable(true);
		mActionPageSetup = new QAction(CipasComposerBase);
		mActionPageSetup->setObjectName(QString::fromUtf8("mActionPageSetup"));
		//mActionUndo = new QAction(CipasComposerBase);
		//mActionUndo->setObjectName(QString::fromUtf8("mActionUndo"));
		//QIcon icon14;
		//icon14.addFile(QString::fromUtf8(":/images/themes/default/mActionUndo.png"), QSize(), QIcon::Normal, QIcon::On);
		//mActionUndo->setIcon(icon14);
		//mActionRedo = new QAction(CipasComposerBase);
		//mActionRedo->setObjectName(QString::fromUtf8("mActionRedo"));
		//QIcon icon15;
		//icon15.addFile(QString::fromUtf8(":/images/themes/default/mActionRedo.png"), QSize(), QIcon::Normal, QIcon::Off);
		//mActionRedo->setIcon(icon15);
		centralwidget = new QWidget(CipasComposerBase);
		centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
		centralwidget->setMouseTracking(true);
		gridLayout_1 = new QGridLayout(centralwidget);
		gridLayout_1->setContentsMargins(0,0,0,0);
		gridLayout_1->setObjectName(QString::fromUtf8("gridLayout_1"));
		//mSplitter = new QSplitter(centralwidget);
		//mSplitter->setObjectName(QString::fromUtf8("mSplitter"));
		//mSplitter->setMouseTracking(true);
		//mSplitter->setOrientation(Qt::Horizontal);
		mViewFrame = new QFrame(centralwidget);
		mViewFrame->setObjectName(QString::fromUtf8("mViewFrame"));
		mViewFrame->setMouseTracking(true);
		mViewFrame->setFrameShape(QFrame::StyledPanel);
		mViewFrame->setFrameShadow(QFrame::Raised);
		//mSplitter->addWidget(mViewFrame);
		gridLayout_1->addWidget(mViewFrame);
		dockProperty = new QDockWidget(QString::fromLocal8Bit("属性设置"), CipasComposerBase);
		dockProperty->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		dockProperty->setObjectName(QString::fromUtf8("propertySetup"));
		mOptionsTabWidget = new QTabWidget(CipasComposerBase);
		mOptionsTabWidget->setObjectName(QString::fromUtf8("mOptionsTabWidget"));
		QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);
		sizePolicy.setHeightForWidth(mOptionsTabWidget->sizePolicy().hasHeightForWidth());
		mOptionsTabWidget->setSizePolicy(sizePolicy);
		mOptionsTabWidget->setMinimumSize(QSize(150, 10));
		mOptionsTabWidget->setMaximumSize(QSize(32767, 32767));
		dockProperty->setWidget(mOptionsTabWidget);
		CipasComposerBase->addDockWidget(Qt::RightDockWidgetArea, dockProperty);
		//widDockProperty=dockProperty->winId();
		tab = new QWidget();
		tab->setObjectName(QString::fromUtf8("tab"));
		gridLayout = new QGridLayout(tab);
		gridLayout->setContentsMargins(0,0,0,0);
#ifndef Q_OS_MAC
		gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
		gridLayout->setContentsMargins(9, 9, 9, 9);
#endif
		gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
		mCompositionOptionsFrame = new QFrame(tab);
		mCompositionOptionsFrame->setObjectName(QString::fromUtf8("mCompositionOptionsFrame"));
		mCompositionOptionsFrame->setFrameShape(QFrame::StyledPanel);
		mCompositionOptionsFrame->setFrameShadow(QFrame::Raised);

		gridLayout->addWidget(mCompositionOptionsFrame, 1, 0, 1, 2);

		mCompositionNameComboBox = new QComboBox(tab);
		mCompositionNameComboBox->setObjectName(QString::fromUtf8("mCompositionNameComboBox"));
		mCompositionNameComboBox->setEnabled(true);
		QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
		sizePolicy1.setHorizontalStretch(0);
		sizePolicy1.setVerticalStretch(0);
		sizePolicy1.setHeightForWidth(mCompositionNameComboBox->sizePolicy().hasHeightForWidth());
		mCompositionNameComboBox->setSizePolicy(sizePolicy1);

		gridLayout->addWidget(mCompositionNameComboBox, 0, 1, 1, 1);

		textLabel1 = new QLabel(tab);
		textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
		QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
		sizePolicy2.setHorizontalStretch(0);
		sizePolicy2.setVerticalStretch(0);
		sizePolicy2.setHeightForWidth(textLabel1->sizePolicy().hasHeightForWidth());
		textLabel1->setSizePolicy(sizePolicy2);

		gridLayout->addWidget(textLabel1, 0, 0, 1, 1);

		mOptionsTabWidget->addTab(tab, QString());
		tab_2 = new QWidget();
		tab_2->setObjectName(QString::fromUtf8("tab_2"));
		gridLayout1 = new QGridLayout(tab_2);
#ifndef Q_OS_MAC
		gridLayout1->setSpacing(6);
#endif
		gridLayout1->setContentsMargins(0, 0, 0, 0);
		gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
		mItemStackedWidget = new QStackedWidget(tab_2);
		mItemStackedWidget->setObjectName(QString::fromUtf8("mItemStackedWidget"));
		page = new QWidget();
		page->setObjectName(QString::fromUtf8("page"));
		mItemStackedWidget->addWidget(page);
		page_2 = new QWidget();
		page_2->setObjectName(QString::fromUtf8("page_2"));
		mItemStackedWidget->addWidget(page_2);

		gridLayout1->addWidget(mItemStackedWidget, 0, 0, 1, 1);

		mOptionsTabWidget->addTab(tab_2, QString());
		//////////////////////////////////////////////////////////////////////////
		//增加Tab页 模板
		dockTemplate = new QDockWidget(QString::fromLocal8Bit("模板管理"), CipasComposerBase);
		dockTemplate->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		dockTemplate->setObjectName(QString::fromUtf8("templateManage"));
		tab_3 = new QWidget();
		tab_3->setObjectName(QString::fromUtf8("tab_3"));
		//mOptionsTabWidget->addTab(tab_3, QString());
		dockTemplate->setWidget(tab_3);
		CipasComposerBase->addDockWidget(Qt::RightDockWidgetArea, dockTemplate);
		CipasComposerBase->tabifyDockWidget(dockProperty,dockTemplate);
		//widDockTemplate=dockTemplate->winId();
		gridLayout_all = new QGridLayout(tab_3);
		gridLayout_all->setContentsMargins(0,0,0,0);
		gridLayout_all->setSpacing(6);
		gridLayout_all->setContentsMargins(0, 0, 0, 0);
		gridLayout_all->setObjectName(QString::fromUtf8("gridLayout_all"));

		scrollArea = new QScrollArea(tab_3);
		scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
		scrollArea->setWidgetResizable(true);

		scrollAreaWidgetContents = new QWidget();
		scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
		scrollAreaWidgetContents->setGeometry(QRect(0, 0, 100, 400));
		gridLayout_t = new QGridLayout(scrollAreaWidgetContents);
		gridLayout_t->setContentsMargins(0,0,0,0);
		gridLayout_t->setSpacing(6);
		gridLayout_t->setContentsMargins(11, 11, 11, 11);
		gridLayout_t->setObjectName(QString::fromUtf8("gridLayout_t"));



		/*toolBoxTemplate = new QToolBox(scrollAreaWidgetContents);
		toolBoxTemplate->setObjectName(QString::fromUtf8("toolBoxTemplate"));*/


		groupboxC=new QGroupBox(scrollAreaWidgetContents);
		groupboxC->setTitle(QString::fromLocal8Bit("当前模板"));
		
		label_ct=new QLabel(groupboxC);
		//label_ct->setText(QString::fromLocal8Bit("模板名："));
		//label_ct->setText(tr("%1").arg(QString::fromLocal8Bit("模板名：")));
		//label_T_content=new QLabel(groupboxC);
		//label_T_content->setText(tr("<b>%1</b>").arg(QString::fromLocal8Bit("模板描述：")));
		//textEditC=new QPlainTextEdit(groupboxC);
		//textEditC->setPlainText(QString::fromLocal8Bit("投影：兰伯特\n作者：NCC"));
		//textEditC->setGeometry(QRect(0, 0, 100, 150));
		//textEditC->setEnabled(false);
		QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Expanding);
		sizePolicy4.setHorizontalStretch(0);
		sizePolicy4.setVerticalStretch(0);
		//textEditC->setSizePolicy(sizePolicy4);
		//textEditC->setFixedSize(100,80);
		//textEditC->setFixedHeight(60);

		textEditC_1=new QPlainTextEdit(groupboxC);
		textEditC_1->setPlainText(QString::fromLocal8Bit("投影：兰伯特 作者：NCC"));
		textEditC_1->setGeometry(QRect(0, 0, 100, 150));
		textEditC_1->setEnabled(true);
		//QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Expanding);
		//sizePolicy4.setHorizontalStretch(0);
		//sizePolicy4.setVerticalStretch(0);
		textEditC_1->setSizePolicy(sizePolicy4);
		//textEditC->setFixedSize(100,80);
		textEditC_1->setFixedHeight(60);

		//hSpacer=new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
		//vSpacer=new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
		newBlankT=new QPushButton();
		newBlankT->setObjectName(QString::fromUtf8("newBlankT"));
		newBlankT->setText(QString::fromLocal8Bit("新建空白模板"));
		saveT=new QPushButton();
		saveT->setObjectName(QString::fromUtf8("saveT"));
		saveT->setText(QString::fromLocal8Bit("保存"));
		saveAsT=new QPushButton();
		saveAsT->setObjectName(QString::fromUtf8("saveAsT"));
		saveAsT->setText(QString::fromLocal8Bit("另存为"));
		QGridLayout *gridLayout_c=new QGridLayout();
		gridLayout_c->setContentsMargins(0,0,0,0);
		gridLayout_c->addWidget(label_ct,0,0,1,5);
		//gridLayout_c->addWidget(label_T_content,1,0,1,4);
		//gridLayout_c->addWidget(textEditC,2,0,2,4);
		gridLayout_c->addWidget(newBlankT,1,2,1,1);
		gridLayout_c->addWidget(saveT,1,3,1,1);
		gridLayout_c->addWidget(saveAsT,1,4,1,1);
		//QVBoxLayout *vlayout = new QVBoxLayout;
		//QHBoxLayout *hlayout=new QHBoxLayout;
		//hlayout->addItem(vSpacer);
		//hlayout->addWidget(saveT);
		//hlayout->addWidget(saveAsT);
		//vlayout->addWidget(textEditC);
		//vlayout->addLayout(gridLayout_c);
		groupboxC->setLayout(gridLayout_c);
		//groupboxC->setLayout(vlayout);
		//QGridLayout *gridLayout_c_a=new QGridLayout();
		//gridLayout_c_a->addWidget(groupboxC,0,0,1,1);
		//gridLayout_c_a->addItem(hSpacer,1,0,1,1);
		//page_c->setLayout(gridLayout_c_a);

		groupboxT=new QGroupBox(scrollAreaWidgetContents);
		groupboxT->setTitle(QString::fromLocal8Bit("模板库"));

		
		//toolBoxTemplate->addItem(page_c, QString::fromUtf8("TemplateCurrent"));

		//toolBoxTemplate->addItem(page_DB, QString::fromUtf8("TemplateDB"));


		mCipasTemplateTree = new CCipasTemplateTree(groupboxT);
		mCipasTemplateTree->setObjectName(QString::fromUtf8("mCipasTemplateTree"));
		QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
		sizePolicy3.setHorizontalStretch(0);
		sizePolicy3.setVerticalStretch(0);
		sizePolicy3.setHeightForWidth(mCipasTemplateTree->sizePolicy().hasHeightForWidth());
		mCipasTemplateTree->setSizePolicy(sizePolicy3);
		mCipasTemplateTree->setWordWrap(false);
		mCipasTemplateTree->setHeaderHidden(true);
		//mCipasTemplateTree->header()->setVisible(false);



		QGridLayout *gridLayout_c_db=new QGridLayout();
		gridLayout_c_db->setContentsMargins(0,0,0,0);
		gridLayout_c_db->addWidget(mCipasTemplateTree,0,0,1,9);

		gridLayout_c_db->addWidget(textEditC_1,1,0,1,9);

		/*mMoveUpToolButton=new QToolButton(groupboxT);
		mMoveUpToolButton->setObjectName(QString::fromUtf8("mMoveUpToolButton"));
		mMoveUpToolButton->setToolTip(QString::fromLocal8Bit("上移"));
		gridLayout_c_db->addWidget(mMoveUpToolButton,2,0,1,1);

		mMoveDownToolButton=new QToolButton(groupboxT);
		mMoveDownToolButton->setObjectName(QString::fromUtf8("mMoveDownToolButton"));
		mMoveDownToolButton->setToolTip(QString::fromLocal8Bit("下移"));
		gridLayout_c_db->addWidget(mMoveDownToolButton,2,1,1,1);*/

		mAddToolButton=new QToolButton(groupboxT);
		mAddToolButton->setObjectName(QString::fromUtf8("mAddToolButton"));
		mAddToolButton->setToolTip(QString::fromLocal8Bit("增加分类"));
		gridLayout_c_db->addWidget(mAddToolButton,2,0,1,1);

		mRemoveToolButton=new QToolButton(groupboxT);
		mRemoveToolButton->setObjectName(QString::fromUtf8("mRemoveToolButton"));
		mRemoveToolButton->setToolTip(QString::fromLocal8Bit("删除分类/项"));
		gridLayout_c_db->addWidget(mRemoveToolButton,2,1,1,1);

		mEditToolButton=new QToolButton(groupboxT);
		mEditToolButton->setObjectName(QString::fromUtf8("mEditToolButton"));
		mEditToolButton->setToolTip(QString::fromLocal8Bit("编辑分类/项"));
		gridLayout_c_db->addWidget(mEditToolButton,2,2,1,1);

		mSavePushButton=new QPushButton(groupboxT);
		mSavePushButton->setObjectName(QString::fromUtf8("mSavePushButton"));
		mSavePushButton->setText(QString::fromLocal8Bit("保存设置"));
		mSavePushButton->setToolTip(QString::fromLocal8Bit("保存对模板库的改变"));
		gridLayout_c_db->addWidget(mSavePushButton,2,3,1,3);


		mOpenPushButton=new QPushButton(groupboxT);
		mOpenPushButton->setObjectName(QString::fromUtf8("mOpenPushButton"));
		mOpenPushButton->setText(QString::fromLocal8Bit("载入模板"));
		mOpenPushButton->setToolTip(QString::fromLocal8Bit("打开选定的项"));
		gridLayout_c_db->addWidget(mOpenPushButton,2,6,1,3);

		groupboxT->setLayout(gridLayout_c_db);

		gridLayout_t->addWidget(groupboxC, 0, 0, 1, 1);
		gridLayout_t->addWidget(groupboxT, 1, 0, 1, 1);
		scrollArea->setWidget(scrollAreaWidgetContents);
		gridLayout_all->addWidget(scrollArea, 0, 0, 1, 1);
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//mSplitter->addWidget(mOptionsTabWidget);

		//gridLayout_1->addWidget(mSplitter, 0, 0, 1, 1);

		//mButtonBox = new QDialogButtonBox(centralwidget);
		//mButtonBox->setObjectName(QString::fromUtf8("mButtonBox"));
		//mButtonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::Help);

		//gridLayout_1->addWidget(mButtonBox, 1, 0, 1, 1);

		CipasComposerBase->setCentralWidget(centralwidget);
		toolBarGen = new QToolBar(QString::fromLocal8Bit("常用工具条"),CipasComposerBase);
		toolBarGen->setObjectName(QString::fromUtf8("toolBarGen"));
		toolBarGen->setOrientation(Qt::Horizontal);
		//widGentoolbar=toolBarGen->winId();
		CipasComposerBase->addToolBar(Qt::TopToolBarArea, toolBarGen);
		toolBarGen->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		toolBarGen->addAction(mActionExportAsImage);
		toolBarGen->addAction(mActionExport2Clipboard);
		toolBarGen->addAction(mActionExportAsSVG);
		toolBarGen->addAction(mActionExportAsPDF);
		toolBarGen->addAction(mActionExportAsEPS);
		//toolBarGen->addAction(mActionPageSetup);
		toolBarGen->addAction(mActionPrint);
		toolBarGen->addSeparator();
		toolBarGen->addAction(mActionZoomAll);
		toolBarGen->addAction(mActionZoomIn);
		toolBarGen->addAction(mActionZoomOut);
		toolBarGen->addAction(mActionRefreshView);

		toolBar = new QToolBar(QString::fromLocal8Bit("制图工具条"),CipasComposerBase);
		toolBar->setObjectName(QString::fromUtf8("toolBar"));
		toolBar->setOrientation(Qt::Horizontal);
		//widtoolbar=toolBar->winId();
		CipasComposerBase->addToolBar(Qt::TopToolBarArea, toolBar);
		QWidget::setTabOrder(mCompositionNameComboBox, mOptionsTabWidget);
		toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		//toolBar->setIconSize(QSize(50,50));
		//toolBar->addAction(mActionLoadFromTemplate);
		//toolBar->addAction(mActionSaveAsTemplate);
		//toolBar->addAction(mActionExportAsPDF);
		//toolBar->addSeparator();
		//toolBar->addAction(mActionUndo);
		//toolBar->addAction(mActionRedo);
		//toolBar->addAction(mActionAddNewMap);
		//toolBar->addAction(mActionAddGLW);
		toolBar->addAction(mActionAddImage);

		//toolBar->addAction(mActionAddGeo);
		toolBar->addAction(mActionAddNewLabel);
		toolBar->addAction(mActionAddNewLegend);
		toolBar->addAction(mActionAddNewScalebar);
		toolBar->addAction(mActionAddBasicShape);
		toolBar->addAction(mActionAddArrow);
		//toolBar->addAction(mActionAddTable);
		toolBar->addSeparator();


		toolBar->addAction(mActionGroupItems);
		toolBar->addAction(mActionUngroupItems);

		//toolBarMap = new QToolBar(QString::fromLocal8Bit("地图工具条"),CipasComposerBase);
		//toolBarMap->setObjectName(QString::fromUtf8("toolBarMap"));
		//toolBarMap->setOrientation(Qt::Horizontal);
		//widmaptoolbar=toolBarMap->winId();
		//CipasComposerBase->addToolBar(Qt::TopToolBarArea, toolBarMap);
		//toolBarMap->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		//地图移动
		//toolBarMap->addAction(mActionSelectMoveItem);
		//toolBarMap->addAction(mActionMoveItemContent);
		//toolBarMap->addAction(mActionMapZoomAll);
		//toolBarMap->addAction(mActionMapZoomIn);
		//toolBarMap->addAction(mActionMapZoomOut);
		retranslateUi(CipasComposerBase);

		//mOptionsTabWidget->setCurrentIndex(2);//将模板管理设为当前tab
		//QList<int> sizeList;
		//sizeList.append(650);	//布局宽度，因为前面resize宽度1100
		//sizeList.append(450);
		//mSplitter->setSizes(sizeList);

		QMetaObject::connectSlotsByName(CipasComposerBase);
	} // setupUi

	void retranslateUi(QMainWindow *CipasComposerBase)
	{
		CipasComposerBase->setWindowTitle(QString::fromLocal8Bit("制图"));
		//mActionPrint->setText(QApplication::translate("CipasComposerBase", "&Print...", 0, QApplication::UnicodeUTF8));
		mActionPrint->setText(QString::fromLocal8Bit("打印…"));
		mActionZoomAll->setText(QString::fromLocal8Bit("纸张全图"));
		mActionZoomIn->setText(QString::fromLocal8Bit("纸张放大"));
		mActionZoomOut->setText(QString::fromLocal8Bit("纸张缩小"));
		//mActionMapZoomAll->setText(QString::fromLocal8Bit("地图全图"));
		//mActionMapZoomIn->setText(QString::fromLocal8Bit("地图放大"));
		//mActionMapZoomOut->setText(QString::fromLocal8Bit("地图缩小"));
		//mActionAddNewMap->setText(QApplication::translate("CipasComposerBase", "Add Map", 0, QApplication::UnicodeUTF8));
		//mActionAddGLW->setText(QApplication::translate("CipasComposerBase", "Add GLW", 0, QApplication::UnicodeUTF8));//QString::fromLocal8Bit("制图")
		//mActionAddGLW->setText(QString::fromLocal8Bit("添加地图"));
#ifndef QT_NO_TOOLTIP
		//mActionAddNewMap->setToolTip(QApplication::translate("CipasComposerBase", "Add new map", 0, QApplication::UnicodeUTF8));
		//mActionAddGLW->setToolTip(QString::fromLocal8Bit("添加地图"));
#endif // QT_NO_TOOLTIP
		// mActionAddNewLabel->setText(QApplication::translate("CipasComposerBase", "Add Label", 0, QApplication::UnicodeUTF8));
		mActionAddNewLabel->setText(QString::fromLocal8Bit("添加标签"));
#ifndef QT_NO_TOOLTIP
		//mActionAddNewLabel->setToolTip(QApplication::translate("CipasComposerBase", "Add new label", 0, QApplication::UnicodeUTF8));
		mActionAddNewLabel->setToolTip(QString::fromLocal8Bit("添加标签"));
#endif // QT_NO_TOOLTIP
		mActionAddNewLegend->setText(QString::fromLocal8Bit("添加图例"));
#ifndef QT_NO_TOOLTIP
		mActionAddNewLegend->setToolTip(QString::fromLocal8Bit("添加图例"));
#endif // QT_NO_TOOLTIP
		mActionSelectMoveItem->setText(QString::fromLocal8Bit("选择/移动 项"));
#ifndef QT_NO_TOOLTIP
		mActionSelectMoveItem->setToolTip(QString::fromLocal8Bit("选择/移动 项"));
#endif // QT_NO_TOOLTIP
		mActionExportAsImage->setText(QString::fromLocal8Bit("导出图片"));
		mActionExport2Clipboard->setText(QObject::tr("复制到剪切板"));
		mActionExportAsPDF->setText(QString::fromLocal8Bit("导出PDF"));
		mActionExportAsEPS->setText(QString::fromLocal8Bit("导出EPS"));
		mActionExportAsSVG->setText(QString::fromLocal8Bit("导出SVG"));
		mActionAddNewScalebar->setText(QString::fromLocal8Bit("添加比例尺"));
#ifndef QT_NO_TOOLTIP
		mActionAddNewScalebar->setToolTip(QString::fromLocal8Bit("添加比例尺"));
#endif // QT_NO_TOOLTIP
		mActionRefreshView->setText(QString::fromLocal8Bit("刷新"));
#ifndef QT_NO_TOOLTIP
		mActionRefreshView->setToolTip(QString::fromLocal8Bit("刷新视图"));
#endif // QT_NO_TOOLTIP
		mActionAddImage->setText(QString::fromLocal8Bit("增加图片"));


//		mActionMoveItemContent->setText(QString::fromLocal8Bit("移动地图内容"));
#ifndef QT_NO_TOOLTIP
	//	mActionMoveItemContent->setToolTip(QString::fromLocal8Bit("移动地图内容"));
#endif // QT_NO_TOOLTIP
		mActionGroupItems->setText(QString::fromLocal8Bit("组合"));
#ifndef QT_NO_TOOLTIP
		mActionGroupItems->setToolTip(QString::fromLocal8Bit("组合项"));
#endif // QT_NO_TOOLTIP
		mActionUngroupItems->setText(QString::fromLocal8Bit("分组"));
#ifndef QT_NO_TOOLTIP
		mActionUngroupItems->setToolTip(QString::fromLocal8Bit("分组项"));
#endif // QT_NO_TOOLTIP
		mActionRaiseItems->setText(QString::fromLocal8Bit("前移"));
#ifndef QT_NO_TOOLTIP
		mActionRaiseItems->setToolTip(QString::fromLocal8Bit("向前移动选中的项"));
#endif // QT_NO_TOOLTIP
		mActionLowerItems->setText(QString::fromLocal8Bit("后移"));
#ifndef QT_NO_TOOLTIP
		mActionLowerItems->setToolTip(QString::fromLocal8Bit("向后移动选中的项"));
#endif // QT_NO_TOOLTIP
		mActionMoveItemsToTop->setText((QString::fromLocal8Bit("移至最前")));
#ifndef QT_NO_TOOLTIP
		mActionMoveItemsToTop->setToolTip((QString::fromLocal8Bit("移动选中的项到最前")));
#endif // QT_NO_TOOLTIP
		mActionMoveItemsToBottom->setText((QString::fromLocal8Bit("移至最后")));
#ifndef QT_NO_TOOLTIP
		mActionMoveItemsToBottom->setToolTip((QString::fromLocal8Bit("移动选中的项到最后")));
#endif // QT_NO_TOOLTIP
		mActionLoadFromTemplate->setText(QString::fromLocal8Bit("加载模板"));
		mActionSaveAsTemplate->setText(QString::fromLocal8Bit("保存模板"));
		mActionAlignLeft->setText(QString::fromLocal8Bit("左齐"));
#ifndef QT_NO_TOOLTIP
		mActionAlignLeft->setToolTip(QString::fromLocal8Bit("向左对齐选中项"));
#endif // QT_NO_TOOLTIP
		mActionAlignHCenter->setText(QString::fromLocal8Bit("居中对齐"));
#ifndef QT_NO_TOOLTIP
		mActionAlignHCenter->setToolTip(QString::fromLocal8Bit("水平居中对齐"));
#endif // QT_NO_TOOLTIP
		mActionAlignRight->setText(QString::fromLocal8Bit("右齐"));
#ifndef QT_NO_TOOLTIP
		mActionAlignRight->setToolTip(QString::fromLocal8Bit("向右对齐选中项"));
#endif // QT_NO_TOOLTIP
		mActionAlignTop->setText(QString::fromLocal8Bit("顶端对齐"));
#ifndef QT_NO_TOOLTIP
		mActionAlignTop->setToolTip(QString::fromLocal8Bit("顶端对齐"));
#endif // QT_NO_TOOLTIP
		mActionAlignVCenter->setText(QString::fromLocal8Bit("中间对齐"));
#ifndef QT_NO_TOOLTIP
		mActionAlignVCenter->setToolTip(QString::fromLocal8Bit("垂直中间对齐"));
#endif // QT_NO_TOOLTIP
		mActionAlignBottom->setText(QString::fromLocal8Bit("底端对齐"));
#ifndef QT_NO_TOOLTIP
		mActionAlignBottom->setToolTip(QString::fromLocal8Bit("底端对齐"));
#endif // QT_NO_TOOLTIP
		mActionQuit->setText(QString::fromLocal8Bit("退出"));
#ifndef QT_NO_TOOLTIP
		mActionQuit->setToolTip(QString::fromLocal8Bit("退出"));
#endif // QT_NO_TOOLTIP
		mActionQuit->setShortcut(QApplication::translate("CipasComposerBase", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
		mActionAddBasicShape->setText(QString::fromLocal8Bit("增加图形"));
		mActionAddArrow->setText(QString::fromLocal8Bit("增加箭头"));
		//  mActionAddTable->setText(QApplication::translate("CipasComposerBase", "Add table", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
		//mActionAddTable->setToolTip(QApplication::translate("CipasComposerBase", "Adds attribute table", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
		mActionPageSetup->setText(QString::fromLocal8Bit("打印设置"));
		//mActionUndo->setText(QString::fromLocal8Bit("撤销"));
		//mActionUndo->setText(QApplication::translate("CipasComposerBase", "Undo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
		 //mActionUndo->setToolTip(QApplication::translate("CipasComposerBase", "Revert last change", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
		 //mActionRedo->setText(QString::fromLocal8Bit("重做"));
		//mActionUndo->setShortcut(QApplication::translate("CipasComposerBase", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
		// mActionRedo->setText(QApplication::translate("CipasComposerBase", "Redo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
		// mActionRedo->setToolTip(QApplication::translate("CipasComposerBase", "Restore last change", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
		//mActionRedo->setShortcut(QApplication::translate("CipasComposerBase", "Ctrl+Shift+Z", 0, QApplication::UnicodeUTF8));
		textLabel1->setText(QString::fromLocal8Bit("组成"));
		mOptionsTabWidget->setTabText(mOptionsTabWidget->indexOf(tab), QString::fromLocal8Bit(" 常规 "));
		mOptionsTabWidget->setTabText(mOptionsTabWidget->indexOf(tab_2), QString::fromLocal8Bit(" 当前项 "));
		//mOptionsTabWidget->setTabText(mOptionsTabWidget->indexOf(tab_3), QString::fromLocal8Bit(" 模板管理 "));

		//toolBoxTemplate->setItemText(toolBoxTemplate->indexOf(page_c), QString::fromLocal8Bit("当前模板"));
		//toolBoxTemplate->setItemText(toolBoxTemplate->indexOf(page_DB), QString::fromLocal8Bit("模板库"));
	} // retranslateUi

};

namespace Ui {
	class CipasComposerBase: public Ui_CipasComposerBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPASSCOMPOSERBASE_H
