
#include <QDateTime>
#include <QDir>
#include <QCloseEvent>
#include <QCheckBox>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QImageWriter>
#include <QMatrix>
#include <QMenuBar>
#include <QMessageBox>
#include <QPageSetupDialog>
#include <QPainter>
#include <QPixmap>
#include <QPrintDialog>
#include <QSettings>
#include <QSizeGrip>
#include <QSvgGenerator>
#include <QToolBar>
#include <QToolButton>
#include <QMdiSubWindow>
#include <QGraphicsView>
#include <QStandardItemModel>
#include <QAbstractListModel>
#include <QStandardItem>
#include <QDialog>
#include <QStatusBar>
#include <QClipboard>
#include <QDesktopWidget>

#include "CipasTemplateItemDialog.h"
#include "CipasComposerView.h"
#include "ComposerItem.h"
#include "CipasComposerItemWidget.h"
#include "CipasComposition.h"
#include "CipasCompositionWidget.h"
#include "CipasComposerLabel.h"
#include "CipasComposerLabelWidget.h"
#include "CipasComposerPicture.h"
#include "CipasComposerPictureWidget.h"
#include "CipasComposerArrow.h"
#include "CipasComposerArrowWidget.h"
#include "CipasComposerScaleBar.h"
#include "CipasComposerScaleBarWidget.h"
#include "CipasComposerShape.h"
#include "CipasComposerShapeWidget.h"
#include "CipasComposerMap.h"
#include "CipasTemplateModel.h"
#include "CipasComposerLegend.h"
#include "CipasComposerLegendWidget.h"
#include "CipasComposerGLW.h"
#include "CipasComposerGLWidget.h"
#include "CipasExportDlg.h"

#include "Map.h"
#include "IDPApplication.h"
#include "Tempalte.h"

#include "CipasComposer.h"

CCipasComposer::CCipasComposer(QWidget* parent): QMainWindow(parent), mUndoView( 0 )
{
	setupUi(this);
	setuoUi2();

	setupTheme();
	
	isExtentInput=false;
	setLoadFromTemplateIsClicked(false);
	
	//////////////////////////////////////////////////////////////////////////
	//模板管理
	QString templatedb = IDPApplication::TemplatePath()+ "/ThemeTemplate/TemplateDB.xml";
	//使用 TreeWidget
	QFile file(templatedb);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Read Templates DB File"),
			tr("Cannot read file %1:\n%2.")
			.arg(templatedb)
			.arg(file.errorString()));
		return;
	}
	mCipasTemplateTree->read(&file);
	mCipasTemplateTree->setDragDropMode(QAbstractItemView::InternalMove);
	//mCipasTemplateTree->expandAll();
	//////////////////////////////////////////////////////////////////////////
}

CCipasComposer::~CCipasComposer(void)
{
	deleteItems();
}

void CCipasComposer::setupTheme()
{
	setIconSizes(40);
	mActionQuit->setIcon(  QIcon(":/images/thematicMap/mActionFileExit.png" ) );
	//mActionLoadFromTemplate->setIcon(QIcon(":/images/mActionFileOpen.png"));
	mActionLoadFromTemplate->setVisible(false);
	//mActionSaveAsTemplate->setIcon(  QIcon(":/images/save.png" ) );
	mActionExport2Clipboard->setIcon(QIcon(":/images/thematicMap/copy.png" ));
	mActionSaveAsTemplate->setVisible(false);
	mActionExportAsImage->setIcon(  QIcon(":/images/thematicMap/savepict.png" ) );
	mActionExportAsSVG->setIcon(  QIcon(":/images/thematicMap/mActionSaveAsSVG.png" ) );
	mActionExportAsPDF->setIcon(  QIcon(":/images/thematicMap/mActionSaveAsPDF.png" ) );
	mActionPrint->setIcon(  QIcon(":/images/thematicMap/mActionFilePrint.png" ) );
	mActionZoomAll->setIcon(  QIcon(":/images/thematicMap/mActionPageZoomFullExtent.png" ) );
	mActionZoomIn->setIcon(  QIcon(":/images/thematicMap/mActionPageZoomIn.png" ) );
	mActionZoomOut->setIcon(  QIcon(":/images/thematicMap/mActionPageZoomOut.png" ) );
	mActionRefreshView->setIcon(  QIcon(":/images/thematicMap/mActionRefreshView.png" ) );
	//mActionUndo->setIcon(  QIcon(":/images/thematicMap/mActionUndo.png" ) );
	//mActionRedo->setIcon(  QIcon(":/images/thematicMap/mActionRedo.png" ) );
	mActionAddImage->setIcon(  QIcon(":/images/thematicMap/mActionAddImage.png" ) );
	//mActionAddGLW->setIcon(  QIcon(":/images/thematicMap/mActionAddMap.png" ) );
	//mActionAddNewMap->setIcon(  getThemeIcon( "/mActionAddMap.png" ) );
	mActionAddNewLabel->setIcon(  QIcon(":/images/thematicMap/mActionLabel.png" ) );
	mActionAddNewLegend->setIcon(  QIcon(":/images/thematicMap/mActionAddLegend.png" ) );
	mActionAddNewScalebar->setIcon(  QIcon(":/images/thematicMap/mActionScaleBar.png" ) );
	mActionAddBasicShape->setIcon(  QIcon(":/images/thematicMap/mActionAddBasicShape.png" ) );
	mActionAddArrow->setIcon(  QIcon(":/images/thematicMap/mActionAddArrow.png" ) );
	//mActionAddTable->setIcon(  getThemeIcon( "/mActionOpenTable.png" ) );
	mActionSelectMoveItem->setIcon(  QIcon(":/images/thematicMap/mActionSelectMoveItem.png" ) );
//	mActionMoveItemContent->setIcon(  QIcon(":/images/thematicMap/mActionPan.png" ) );
//	mActionMapZoomAll->setIcon(  QIcon(":/images/thematicMap/mActionZoomFullExtent.png" ) );
//	mActionMapZoomIn->setIcon(  QIcon(":/images/thematicMap/mActionZoomIn.png" ) );
//	mActionMapZoomOut->setIcon(  QIcon(":/images/thematicMap/mActionZoomOut.png" ) );
	mActionGroupItems->setIcon(  QIcon(":/images/thematicMap/mActionGroupItems.png" ) );
	mActionUngroupItems->setIcon(  QIcon(":/images/thematicMap/mActionUngroupItems.png" ) );
	mActionRaiseItems->setIcon(      QIcon (":/images/thematicMap/mActionRaiseItems.png" ) );
	mActionLowerItems->setIcon(  QIcon(":/images/thematicMap/mActionLowerItems.png" ) );
	mActionMoveItemsToTop->setIcon(  QIcon(":/images/thematicMap/mActionMoveItemsToTop.png" ) );
	mActionMoveItemsToBottom->setIcon(  QIcon(":/images/thematicMap/mActionMoveItemsToBottom.png" ) );
	mActionAlignLeft->setIcon(  QIcon(":/images/thematicMap/mActionAlignLeft.png" ) );
	mActionAlignHCenter->setIcon(  QIcon(":/images/thematicMap/mActionAlignHCenter.png" ) );
	mActionAlignRight->setIcon(  QIcon(":/images/thematicMap/mActionAlignRight.png" ) );
	mActionAlignTop->setIcon(  QIcon(":/images/thematicMap/mActionAlignTop.png" ) );
	mActionAlignVCenter->setIcon(  QIcon(":/images/thematicMap/mActionAlignVCenter.png" ) );
	mActionAlignBottom->setIcon(  QIcon(":/images/thematicMap/mActionAlignBottom.png" ) );
	//////////////////////////////////////////////////////////////////////////
	//模板库的几个toolbutton 的图标
	mAddToolButton->setIcon( QIcon(":/images/thematicMap/symbologyAdd.png"  ) );
	mEditToolButton->setIcon( QIcon(":/images/thematicMap/symbologyEdit.png" ) ) ;
	mRemoveToolButton->setIcon( QIcon(":/images/thematicMap/symbologyRemove.png" ) );
	//mMoveUpToolButton->setIcon( QIcon(":/images/thematicMap/symbologyUp.png" ) ) ;
	//mMoveDownToolButton->setIcon(QIcon(":/images/thematicMap/symbologyDown.png"  ) );
	//////////////////////////////////////////////////////////////////////////
	//label_T_content->setText(tr("<b>%1</b>").arg(QString::fromLocal8Bit("模板描述：")));
}

void CCipasComposer::setuoUi2()
{
	QSettings settings;
	int size = settings.value( "/IconSize", 32 ).toInt();
	setIconSize( QSize( size, size ) );

	//向前向后移动项目的按钮
	QToolButton* orderingToolButton = new QToolButton( this );
	orderingToolButton->setPopupMode( QToolButton::InstantPopup );
	orderingToolButton->setAutoRaise( true );
	orderingToolButton->setToolButtonStyle( Qt::ToolButtonIconOnly );

	orderingToolButton->addAction( mActionRaiseItems );
	orderingToolButton->addAction( mActionLowerItems );
	orderingToolButton->addAction( mActionMoveItemsToTop );
	orderingToolButton->addAction( mActionMoveItemsToBottom );
	orderingToolButton->setDefaultAction( mActionRaiseItems );
	toolBar->addWidget( orderingToolButton );

	//对齐按钮
	QToolButton* alignToolButton = new QToolButton( this );
	alignToolButton->setPopupMode( QToolButton::InstantPopup );
	alignToolButton->setAutoRaise( true );
	alignToolButton->setToolButtonStyle( Qt::ToolButtonIconOnly );

	alignToolButton->addAction( mActionAlignLeft );
	alignToolButton->addAction( mActionAlignHCenter );
	alignToolButton->addAction( mActionAlignRight );
	alignToolButton->addAction( mActionAlignTop );
	alignToolButton->addAction( mActionAlignVCenter );
	alignToolButton->addAction( mActionAlignBottom );
	alignToolButton->setDefaultAction( mActionAlignLeft );
	toolBar->addWidget( alignToolButton );

	QActionGroup* toggleActionGroup = new QActionGroup( this );
//	toggleActionGroup->addAction( mActionMoveItemContent );
	//toggleActionGroup->addAction( mActionAddNewMap );
	toggleActionGroup->addAction( mActionAddNewLabel );
	toggleActionGroup->addAction( mActionAddNewLegend );
	toggleActionGroup->addAction( mActionAddNewScalebar );
	toggleActionGroup->addAction( mActionAddImage );
	//toggleActionGroup->addAction(mActionAddGLW);
	toggleActionGroup->addAction( mActionSelectMoveItem );
	toggleActionGroup->addAction( mActionAddBasicShape );
	toggleActionGroup->addAction( mActionAddArrow );
	//toggleActionGroup->addAction( mActionAddTable );
	toggleActionGroup->setExclusive( true );

	//mActionAddNewMap->setCheckable( true );
	mActionAddNewLabel->setCheckable( true );
	mActionAddNewLegend->setCheckable( true );
	mActionSelectMoveItem->setCheckable( true );
	mActionAddNewScalebar->setCheckable( true );
	mActionAddImage->setCheckable( true );
//	mActionAddGLW->setCheckable(true);
//	mActionMoveItemContent->setCheckable( true );
	mActionAddBasicShape->setCheckable( true );
	mActionAddArrow->setCheckable( true );

	QMenu *fileMenu = menuBar()->addMenu( QString::fromLocal8Bit("文件") );
	fileMenu->addAction( mActionLoadFromTemplate );
	fileMenu->addAction( mActionSaveAsTemplate );
	fileMenu->addSeparator();
	fileMenu->addAction( mActionExportAsImage );
	fileMenu->addAction( mActionExport2Clipboard );
	fileMenu->addAction( mActionExportAsPDF );
	fileMenu->addAction( mActionExportAsSVG );
	fileMenu->addSeparator();
	//fileMenu->addAction( mActionPageSetup );
	fileMenu->addAction( mActionPrint );
	fileMenu->addSeparator();
	fileMenu->addAction( mActionQuit );
	QObject::connect( mActionQuit, SIGNAL( triggered() ), this, SLOT( close() ) );

	QMenu *viewMenu = menuBar()->addMenu( QString::fromLocal8Bit("视图") );
	viewMenu->addAction( mActionZoomIn );
	viewMenu->addAction( mActionZoomOut );
	viewMenu->addAction( mActionZoomAll );
	viewMenu->addSeparator();
	viewMenu->addAction( mActionRefreshView );

	QMenu *layoutMenu = menuBar()->addMenu( QString::fromLocal8Bit("制图") );
	//layoutMenu->addAction( mActionUndo );
	//layoutMenu->addAction( mActionRedo );
	//layoutMenu->addSeparator();
	//layoutMenu->addAction( mActionAddNewMap );
//	layoutMenu->addAction( mActionAddGLW );
	layoutMenu->addAction( mActionAddNewLabel );
	layoutMenu->addAction( mActionAddNewScalebar );
	layoutMenu->addAction( mActionAddNewLegend );
	layoutMenu->addAction( mActionAddImage );
	layoutMenu->addAction( mActionAddBasicShape );
	layoutMenu->addAction( mActionAddArrow );
	//layoutMenu->addAction( mActionAddGLW );

	layoutMenu->addSeparator();
	layoutMenu->addAction( mActionSelectMoveItem );
////	layoutMenu->addAction( mActionMoveItemContent );
//	layoutMenu->addAction( mActionMapZoomAll );
//	layoutMenu->addAction( mActionMapZoomIn );
//	layoutMenu->addAction( mActionMapZoomOut );

	//
	//layoutMenu->addAction( mActionAddTable );
	layoutMenu->addSeparator();
	layoutMenu->addAction( mActionGroupItems );
	layoutMenu->addAction( mActionUngroupItems );
	layoutMenu->addAction( mActionRaiseItems );
	layoutMenu->addAction( mActionLowerItems );
	layoutMenu->addAction( mActionMoveItemsToTop );
	layoutMenu->addAction( mActionMoveItemsToBottom );

	mFirstTime = true;

	// Create action to select this window
	mWindowAction = new QAction( windowTitle(), this );
	connect( mWindowAction, SIGNAL( triggered() ), this, SLOT( activate() ) );

	//QgsDebugMsg( "entered." );

	setMouseTracking( true );
	//mSplitter->setMouseTracking(true);
	mViewFrame->setMouseTracking( true );

	////create composer view
	mView = new CCipasComposerView(mViewFrame );
	mView->SetOsgViewers(m_osgViewers);
	connect( mView, SIGNAL( emitMapFull() ), this, SIGNAL( eMapFull() ) );
	connect( mView, SIGNAL( emitMapZoonIn() ), this, SIGNAL( eMapZoonIn() ) );
	connect( mView, SIGNAL( emitMapZoonOut() ), this, SIGNAL( eMapZoonOut() ) );
	connect( mView, SIGNAL( emitMapMove(QPoint,QPoint) ), this, SIGNAL( eMapMove(QPoint,QPoint) ) );

	connectSlots();
	//mapView=new QGraphicsView(mViewFrame);
	////init undo/redo buttons
	mComposition  = new CCipasComposition();
	//mActionUndo->setEnabled( false );
	//mActionRedo->setEnabled( false );
	//if ( mComposition->undoStack() )
	{
		//connect( mComposition->undoStack(), SIGNAL( canUndoChanged( bool ) ), mActionUndo, SLOT( setEnabled( bool ) ) );
		//connect( mComposition->undoStack(), SIGNAL( canRedoChanged( bool ) ), mActionRedo, SLOT( setEnabled( bool ) ) );
	}

	mComposition->setParent( mView );
	mView->setComposition( mComposition );
	CCipasCompositionWidget* compositionWidget = new CCipasCompositionWidget( mCompositionOptionsFrame, mComposition );
	QObject::connect( mComposition, SIGNAL( paperSizeChanged() ), compositionWidget, SLOT( displayCompositionWidthHeight() ) );
	compositionWidget->show();

	mCompositionOptionsLayout = new QGridLayout( mCompositionOptionsFrame );
	mCompositionOptionsLayout->setMargin( 0 );
	mCompositionOptionsLayout->addWidget( compositionWidget );

	QGridLayout *l = new QGridLayout( mViewFrame );
	l->setMargin( 0 );
	l->addWidget( mView, 0, 0 );

	mCompositionNameComboBox->insertItem( 0, QString::fromLocal8Bit("地图窗口 1") );

	refreshmCompositionNameComboBox();

	mSizeGrip = new QSizeGrip( this );
	mSizeGrip->resize( mSizeGrip->sizeHint() );
	mSizeGrip->move( rect().bottomRight() - mSizeGrip->rect().bottomRight() );

	restoreWindowState();
	setSelectionTool();

	mView->setFocus();
}

QIcon CCipasComposer::getThemeIcon( const QString theName )
{
	QString themPath=IDPApplication::Path()  +"/themes/default/";
	//QString myPreferredPath = QgsApplication::activeThemePath() + QDir::separator() + theName;
	QString myDefaultPath =themPath  + theName;
	return QIcon( myDefaultPath );
}

void CCipasComposer::setIconSizes( int size )
{
	setIconSize( QSize( size, size ) );
	QList<QToolBar *> toolbars = findChildren<QToolBar *>();
	foreach( QToolBar * toolbar, toolbars )
	{
		toolbar->setIconSize( QSize( size, size ) );
	}
}

void CCipasComposer::selfAdaptLayout()
{
	QDesktopWidget* desktopWidget = QApplication::desktop();
	//得到客户区矩形
	QRect clientRect = desktopWidget->availableGeometry();
	//得到应用程序矩形
	QRect applicationRect = desktopWidget->screenGeometry();
	////得到当前纸张
	QList<int> sizeList;
	sizeList.append(clientRect.width()*0.55);	//布局宽度，因为前面resize宽度1100
	sizeList.append(clientRect.width()*0.45-40);
}

void CCipasComposer::connectSlots()
{
	connect( mView, SIGNAL( selectedItemChanged( CComposerItem* ) ), this, SLOT( showItemOptions( CComposerItem* ) ) );
	connect( mView, SIGNAL( composerLabelAdded( CCipasComposerLabel* ) ), this, SLOT( addComposerLabel( CCipasComposerLabel* ) ) );
	//connect( mView, SIGNAL( composerMapAdded( CCipasComposerMap* ) ), this, SLOT( addComposerMap( CCipasComposerMap* ) ) );
	connect( mView, SIGNAL( itemRemoved( CComposerItem* ) ), this, SLOT( deleteItem( CComposerItem* ) ) );
	connect( mView, SIGNAL( composerScaleBarAdded( CCipasComposerScaleBar* ) ), this, SLOT( addComposerScaleBar( CCipasComposerScaleBar* ) ) );
	connect( mView, SIGNAL( composerLegendAdded( CCipasComposerLegend* ) ), this, SLOT( addComposerLegend( CCipasComposerLegend* ) ) );
	connect( mView, SIGNAL( composerPictureAdded( CCipasComposerPicture* ) ), this, SLOT( addComposerPicture( CCipasComposerPicture* ) ) );
	connect( mView, SIGNAL( composerShapeAdded( CCipasComposerShape* ) ), this, SLOT( addComposerShape( CCipasComposerShape* ) ) );
	connect( mView, SIGNAL( composerArrowAdded( CCipasComposerArrow* ) ), this, SLOT( addComposerArrow( CCipasComposerArrow* ) ) );
	//connect( mView, SIGNAL( composerTableAdded( QgsComposerAttributeTable* ) ), this, SLOT( addComposerTable( QgsComposerAttributeTable* ) ) );
	connect( mView, SIGNAL( actionFinished() ), this, SLOT( setSelectionTool() ) );
	connect( this, SIGNAL( updateImage(QImage*) ), this, SLOT( setSelectionTool() ) );
}

void CCipasComposer::refreshmCompositionNameComboBox()
{
	mCompositionNameComboBox->clear();
}

void CCipasComposer::open()
{
	if ( mFirstTime )
	{
		//mComposition->createDefault();
		mFirstTime = false;
		//show();
		showMaximized();

		zoomFull(); // zoomFull() does not work properly until we have called show()
	}
	else
	{
		//show(); //make sure the window is displayed - with a saved project, it's possible to not have already called show()
		showMaximized();
		//is that a bug?
		activate(); //bring the composer window to the front
	}
	//
	QFileInfo fi(mCurrentTemplateName);
	if (fi.fileName()==tr("ServerGrid.cpt"))
	{
		if (mView)
		{
			CCipasComposition *c= mView->composition();
			CComposerItem* item=c->getCurrentItem();
			if (item)
			{
				CCipasComposerGLW* composerGLW = dynamic_cast<CCipasComposerGLW *>( item);
				composerGLW->setGLExtent(mxmin,mymin,mxmax/*min(mxmax+77.01,282)*/,mymax);
			}
		}
	}
	
}

void CCipasComposer::activate()
{
	show();
	raise();
	setWindowState( windowState() & ~Qt::WindowMinimized );
	activateWindow();
}
void CCipasComposer::setTitle( const QString& title )
{
	mTitle = title;
	setWindowTitle( mTitle );
	if ( mWindowAction )
	{
		mWindowAction->setText( title );
	}
}

CCipasComposerView *CCipasComposer::view( void )
{
	return mView;
}

void CCipasComposer::zoomFull( void )
{
	if ( mView )
	{
		double	dw= mComposition->paperWidth();
		double	dh=mComposition->paperHeight();
		mView->fitInView( 0, 0, mComposition->paperWidth() +6, mComposition->paperHeight() +4, Qt::KeepAspectRatio );
	}
}
void CCipasComposer::on_mActionZoomAll_triggered()
{
	zoomFull();
	mView->update();
	emit zoomLevelChanged();
}

void CCipasComposer::on_mActionZoomIn_triggered()
{
	mView->scale( 1.2, 1.2 );
	mView->update();
	emit zoomLevelChanged();
}

void CCipasComposer::on_mActionZoomOut_triggered()
{
	mView->scale( .8, .8 );
	mView->update();
	emit zoomLevelChanged();
}
//////////////////////////////////////////////////////////////////////////
void CCipasComposer::on_mActionSelectMoveItem_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::Select );
	}
}
void CCipasComposer::on_mActionMoveItemContent_triggered()
{
	if (mView)
	{
		mView->setCurrentTool( CCipasComposerView::MoveItemContent );
	}
}
void CCipasComposer::on_mActionMapZoomAll_triggered()
{
	if (mView)
	{
		//地图操作或者选择操作要设置当前项目GLW为地图 待完善
		//当从模板加载而没有选择地图时候，可能会有问题
		CCipasComposition *c= mView->composition();
		CComposerItem* item=c->getCurrentItem();
		if (item)
		{
			emit eMapFull();
		}
		mView->update();
	}
}

void CCipasComposer::on_mActionMapZoomIn_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::MapZoomIn );
		
	}
}

void CCipasComposer::on_mActionMapZoomOut_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::MapZoomOut );
		
	}
}
//////////////////////////////////////////////////////////////////////////

void CCipasComposer::on_mActionRefreshView_triggered()
{
	if ( !mComposition )
	{
		return;
	}
	mComposition->update();
}

void CCipasComposer::ExportToImage(QString& saveFileName,int dpi)
{
	// Image size
	int width = ( int )( dpi * mComposition->paperWidth() / 25.4 );
	int height = ( int )( dpi * mComposition->paperHeight() / 25.4 );

	int memuse = width * height * 3 / 1000000;  // pixmap + image

	if ( memuse > 200 )   // about 4500x4500
	{
		int answer = QMessageBox::warning( 0, tr( "Big image" ),tr( "To create image %1x%2 requires about %3 MB of memory. Proceed?" ).arg( width ).arg( height ).arg( memuse ),QMessageBox::Ok | QMessageBox::Cancel,  QMessageBox::Ok );
		raise();
		if ( answer == QMessageBox::Cancel )
			return;
	}
	QSettings myQSettings;  
	QString lastSaveDir = myQSettings.value( "UI/lastSaveAsImageDir", "" ).toString();
	if ( saveFileName.isEmpty() )
	{
		return;
	}
	QFileInfo saveFileInfo( saveFileName );
	myQSettings.setValue( "UI/lastSaveAsImageDir", saveFileInfo.absolutePath() );

	QImage image( QSize( width, height ), QImage::Format_ARGB32 );
	if ( image.isNull() )
	{
		QMessageBox::warning( 0,	tr( "Image too big" ),tr( "Creation of image with %1x%2 pixels failed.  Export aborted." ).arg( width ).arg( height ),QMessageBox::Ok );
		return;
	}

	mComposition->setPlotStyle( CCipasComposition::Print );
	image.setDotsPerMeterX( dpi / 25.4 * 1000 );
	image.setDotsPerMeterY( dpi / 25.4 * 1000 );
	image.fill( 0 );
	QPainter p( &image );
	QRectF sourceArea( 0, 0, mComposition->paperWidth(), mComposition->paperHeight() );
	QRectF targetArea( 0, 0, width, height );
	mView->setPaintingEnabled( false );
	mComposition->render( &p, targetArea, sourceArea );
	p.end();
	mComposition->setPlotStyle( CCipasComposition::Preview );
	mView->setPaintingEnabled( true );
	if (image.save( saveFileName ))
	{
		statusBar()->showMessage(tr("保存图片成功"), 2000);
		statusBar()->showMessage(tr("就绪"));
	}
	else
	{
		statusBar()->showMessage(tr("保存图片失败"));
	}
}

void CCipasComposer::on_mActionExportAsImage_triggered()
{
	statusBar()->showMessage(QString::fromLocal8Bit("保存图片"),2000);
	CipasExportDlg dlg(mComposition,mCurrentTemplateName);
	if (dlg.exec()==QDialog::Accepted)
	{
		QString outps=dlg.SaveFileName();
		QFileInfo fi(dlg.SaveFileName());
		if (fi.exists())
		{
			int ret = QMessageBox::information(this
				,tr("信息提示")
				,tr("文件已存在，是否覆盖？")
				,QMessageBox::Ok
				| QMessageBox::Cancel);
			if (ret == 1024)
			{
				ExportToImage(dlg.SaveFileName(),dlg.SaveDpi());
			}
			else
			{
				return;
			}
		}
		else
			ExportToImage(dlg.SaveFileName(),dlg.SaveDpi());
	}
}

void CCipasComposer::on_mActionExport2Clipboard_triggered()
{
	// Image size
	int prsol=mComposition->printResolution();
	int width = ( int )( prsol * mComposition->paperWidth() / 25.4 );
	int height = ( int )( prsol * mComposition->paperHeight() / 25.4 );
	int memuse = width * height * 3 / 1000000;  // pixmap + image
	if ( memuse > 200 )   // about 4500x4500
	{
		int answer = QMessageBox::warning( 0, tr( "Big image" ),tr( "To create image %1x%2 requires about %3 MB of memory. Proceed?" ).arg( width ).arg( height ).arg( memuse ),QMessageBox::Ok | QMessageBox::Cancel,  QMessageBox::Ok );
		raise();
		if ( answer == QMessageBox::Cancel )
			return;
	}

	QImage image( QSize( width, height ), QImage::Format_ARGB32 );
	if ( image.isNull() )
	{
		QMessageBox::warning( 0,tr( "Image too big" ),tr( "Creation of image with %1x%2 pixels failed.  Export aborted." ).arg( width ).arg( height ),QMessageBox::Ok );
		return;
	}
	mComposition->setPlotStyle( CCipasComposition::Print );
	image.setDotsPerMeterX( prsol / 25.4 * 1000 );
	image.setDotsPerMeterY( prsol / 25.4 * 1000 );
	image.fill( 0 );
	QPainter p( &image );
	QRectF sourceArea( 0, 0, mComposition->paperWidth(), mComposition->paperHeight() );
	QRectF targetArea( 0, 0, width, height );
	mView->setPaintingEnabled( false );
	mComposition->render( &p, targetArea, sourceArea );
	p.end();
	mComposition->setPlotStyle( CCipasComposition::Preview );
	mView->setPaintingEnabled( true );
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setImage(image);
	QMessageBox::information( 0,tr( "信息提示" ),tr( "专题图内容成功复制到剪切板." ),QMessageBox::Ok );
}

void CCipasComposer::on_mActionExportAsEPS_triggered()
{
	QSettings myQSettings;  
	QString lastSaveDir = myQSettings.value( "UI/lastSaveAsPdfDir", "" ).toString();
	QString saveFileName = QFileDialog::getSaveFileName( 0, QString::fromLocal8Bit("保存为EPS格式..."), lastSaveDir, "EPS (*.eps)" );
	if ( saveFileName.isEmpty() )
	{
		return;
	}
	int prsol=mComposition->printResolution();
	int width = ( int )( prsol * mComposition->paperWidth() / 25.4 );
	int height = ( int )( prsol * mComposition->paperHeight() / 25.4 );
	int memuse = width * height * 3 / 1000000;  // pixmap + image
	if ( memuse > 200 )   // about 4500x4500
	{
		int answer = QMessageBox::warning( 0, tr( "Big image" ),tr( "To create image %1x%2 requires about %3 MB of memory. Proceed?" ).arg( width ).arg( height ).arg( memuse ),QMessageBox::Ok | QMessageBox::Cancel,  QMessageBox::Ok );
		raise();
		if ( answer == QMessageBox::Cancel )
			return;
	}

	QImage image( QSize( width, height ), QImage::Format_ARGB32 );
	if ( image.isNull() )
	{
		QMessageBox::warning( 0,tr( "Image too big" ),tr( "Creation of image with %1x%2 pixels failed.  Export aborted." ).arg( width ).arg( height ),QMessageBox::Ok );
		return;
	}
	mComposition->setPlotStyle( CCipasComposition::Print );
	image.setDotsPerMeterX( prsol / 25.4 * 1000 );
	image.setDotsPerMeterY( prsol / 25.4 * 1000 );
	image.fill( 0 );
	QPainter pimage( &image );
	QRectF sourceArea( 0, 0, mComposition->paperWidth(), mComposition->paperHeight() );
	QRectF targetArea( 0, 0, width, height );
	mView->setPaintingEnabled( false );
	mComposition->render( &pimage, targetArea, sourceArea );
	pimage.end();
	mComposition->setPlotStyle( CCipasComposition::Preview );
	mView->setPaintingEnabled( true );
	/////////////////////////////////////保存Image/////////////////////////////////////

	QPrinter psOut(QPrinter::PrinterResolution);
	QPainter peps;
	// making some definitions (papersize, output to file, filename):
	psOut.setCreator( "IDP " "IDPEPS"  );
	if ( psOut.outputFileName().isEmpty() )
		psOut.setOutputFileName( "untitled_printer_document" );

	// Extension must be .eps so that Qt generates EPS file
	QFile tmpFile(saveFileName);
	if ( !tmpFile.open( QIODevice::ReadWrite ) )
		return;

	psOut.setOutputFileName(tmpFile.fileName());
	psOut.setOutputFormat(QPrinter::PostScriptFormat);
	psOut.setFullPage(true);
	psOut.setPaperSize(image.size(), QPrinter::DevicePixel);

	// painting the pixmap to the "printer" which is a file
	peps.begin( &psOut );
	peps.drawImage( QPoint( 0, 0 ), image );
	peps.end();

	// Copy file to imageio struct
	QFile inFile(tmpFile.fileName());
	if ( !inFile.open( QIODevice::ReadOnly ) )
		return;

	QTextStream in( &inFile );
	in.setCodec( "ISO-8859-1" );
	QTextStream out( &tmpFile);//device() 
	out.setCodec( "ISO-8859-1" );

	QString szInLine = in.readLine();
	out << szInLine << '\n';

	while( !in.atEnd() ){
		szInLine = in.readLine();
		out << szInLine << '\n';
	}

	inFile.close();
	tmpFile.close();
}

void CCipasComposer::on_mActionExportAsPDF_triggered()
{
	QSettings myQSettings;  
	QString lastSaveDir = myQSettings.value( "UI/lastSaveAsPdfDir", "" ).toString();
	QString saveFileName = QFileDialog::getSaveFileName( 0, QString::fromLocal8Bit("保存为PDF格式..."), lastSaveDir, "PDF (*.pdf)" );
	if ( saveFileName.isEmpty() )
	{
		return;
	}
	QFileInfo saveFileInfo( saveFileName );
	myQSettings.setValue( "UI/lastSaveAsPdfDir", saveFileInfo.absolutePath() );


	if ( !saveFileName.endsWith( ".pdf", Qt::CaseInsensitive ) )
	{
		saveFileName.append( ".pdf" );
	}

	QPrinter printer;

	printer.setOutputFormat( QPrinter::PdfFormat );
	printer.setOutputFileName( saveFileName );
	printer.setPaperSize( QSizeF( mComposition->paperWidth(), mComposition->paperHeight() ), QPrinter::Millimeter );

	print( printer );
}

void CCipasComposer::on_mActionExportAsSVG_triggered()
{
	QSettings myQSettings;  
	QString lastSaveDir = myQSettings.value( "UI/lastSaveAsSVGDir", "" ).toString();
	QString saveFileName = QFileDialog::getSaveFileName( 0, QString::fromLocal8Bit("保存为SVG格式..."), lastSaveDir, "SVG (*.svg)" );
	if ( saveFileName.isEmpty() )
	{
		return;
	}
	QFileInfo saveFileInfo( saveFileName );
	myQSettings.setValue( "UI/lastSaveAsSVGDir", saveFileInfo.absolutePath() );


	if ( !saveFileName.endsWith( ".svg", Qt::CaseInsensitive ) )
	{
		saveFileName.append( ".svg" );
	}


	QSvgGenerator generator;
#if QT_VERSION >= 0x040500
	generator.setTitle( "Thematic Map" );
#endif
	generator.setFileName( saveFileName );
	//width in pixel
	int width = ( int )( mComposition->paperWidth() * mComposition->printResolution() / 25.4 );
	//height in pixel
	int height = ( int )( mComposition->paperHeight() * mComposition->printResolution() / 25.4 );
	generator.setSize( QSize( width, height ) );
#if QT_VERSION >= 0x040500
	generator.setViewBox( QRect( 0, 0, width, height ) );
#endif
	generator.setResolution( mComposition->printResolution() ); //because the rendering is done in mm, convert the dpi

	QPainter p( &generator );

	QRectF sourceArea( 0, 0, mComposition->paperWidth(), mComposition->paperHeight() );
	QRectF targetArea( 0, 0, width, height );
	mView->setPaintingEnabled( false );
	mComposition->render( &p, targetArea, sourceArea );
	p.end();
	mView->setPaintingEnabled( true );
}

void CCipasComposer::on_mActionPrint_triggered()
{
	//orientation and page size are already set to QPrinter in the page setup dialog
	QPrintDialog printDialog( &mPrinter, 0 );
	if ( printDialog.exec() != QDialog::Accepted )
	{
		return;
	}
	print( mPrinter );
}

void CCipasComposer::cleanupAfterTemplateRead()
{
	QMap<CComposerItem*, QWidget*>::const_iterator itemIt = mItemWidgetMap.constBegin();
	for ( ; itemIt != mItemWidgetMap.constEnd(); ++itemIt )
	{
		//update composer map extent if it does not intersect the full extent of all layers
		CCipasComposerMap* mapItem = dynamic_cast<CCipasComposerMap *>( itemIt.key() );
		if ( mapItem )
		{
			//test if composer map extent intersects extent of all layers
			bool intersects = false;
			CCipasRectangle composerMapExtent = mapItem->extent();

			//if not: apply current canvas extent
			if ( !intersects )
			{
				double currentWidth = mapItem->rect().width();
				double currentHeight = mapItem->rect().height();
				if ( currentWidth - 0 > 0.0 ) //don't divide through zero
				{
					CCipasRectangle canvasExtent = mapItem->mapRenderer()->extent();
					//adapt min y of extent such that the size of the map item stays the same
					double newCanvasExtentHeight = currentHeight / currentWidth * canvasExtent.width();
					canvasExtent.setYMinimum( canvasExtent.yMaximum() - newCanvasExtentHeight );
					mapItem->setNewExtent( canvasExtent );
				}
			}
		}
	}
}

void CCipasComposer::writeXML( QDomNode& parentNode, QDomDocument& doc )
{
	QDomElement composerElem = doc.createElement( "Composer" );
	composerElem.setAttribute( "title", mTitle );

	//store if composer is open or closed
	if ( isVisible() )
	{
		composerElem.setAttribute( "visible", 1 );
	}
	else
	{
		composerElem.setAttribute( "visible", 0 );
	}
	parentNode.appendChild( composerElem );

	//store composer items:
	QMap<CComposerItem*, QWidget*>::const_iterator itemIt = mItemWidgetMap.constBegin();
	for ( ; itemIt != mItemWidgetMap.constEnd(); ++itemIt )
	{
		itemIt.key()->writeXML( composerElem, doc );
	}

	//store composer view
	//store composition
	if ( mComposition )
	{
		mComposition->writeXML( composerElem, doc );
	}
}

void CCipasComposer::readXML( const QDomDocument& doc )
{
	QDomNodeList composerNodeList = doc.elementsByTagName( "Composer" );
	if ( composerNodeList.size() < 1 )
	{
		return;
	}
	readXML( composerNodeList.at( 0 ).toElement(), doc, true );
//	cleanupAfterTemplateRead();
}

void CCipasComposer::readXML( const QDomElement& composerElem, const QDomDocument& doc, bool fromTemplate )
{
	if ( !fromTemplate )
	{
		if ( composerElem.hasAttribute( "title" ) )
		{
			setTitle( composerElem.attribute( "title", tr( "Composer" ) ) );
		}
	}

	//delete composer view and composition
	delete mView;
	mView = 0;
	//delete every child of mViewFrame
	QObjectList viewFrameChildren = mViewFrame->children();
	QObjectList::iterator it = viewFrameChildren.begin();
	for ( ; it != viewFrameChildren.end(); ++it )
	{
		delete( *it );
	}

	//delete composition widget
	CCipasComposerGLW* oldCompositionWidget = qobject_cast<CCipasComposerGLW *>( mCompositionOptionsFrame->children().at( 0 ) );
	delete oldCompositionWidget;
	delete mCompositionOptionsLayout;
	mCompositionOptionsLayout = 0;

	//	mView = new CCipasComposerView(mViewFrame);
	mView = new CCipasComposerView(mViewFrame);
	connect( mView, SIGNAL( emitMapFull() ), this, SIGNAL( eMapFull() ) );
	connect( mView, SIGNAL( emitMapZoonIn() ), this, SIGNAL( eMapZoonIn() ) );
	connect( mView, SIGNAL( emitMapZoonOut() ), this, SIGNAL( eMapZoonOut() ) );
	connect( mView, SIGNAL( emitMapMove(QPoint,QPoint) ), this, SIGNAL( eMapMove(QPoint,QPoint) ) );

	connectSlots();

	//read composition settings
	//mComposition = new CCipasComposition( mQgis->mapCanvas()->mapRenderer() );
	mComposition = new CCipasComposition();
	QDomNodeList compositionNodeList = composerElem.elementsByTagName( "Composition" );
	if ( compositionNodeList.size() > 0 )
	{
		QDomElement compositionElem = compositionNodeList.at( 0 ).toElement();
		mComposition->readXML( compositionElem, doc );
	}

	QGridLayout *l = new QGridLayout( mViewFrame );
	l->setMargin( 0 );
	l->addWidget( mView, 0, 0 );

	//create compositionwidget
	CCipasCompositionWidget* compositionWidget = new CCipasCompositionWidget( mCompositionOptionsFrame, mComposition );
	QObject::connect( mComposition, SIGNAL( paperSizeChanged() ), compositionWidget, SLOT( displayCompositionWidthHeight() ) );
	compositionWidget->show();

	mCompositionOptionsLayout = new QGridLayout( mCompositionOptionsFrame );
	mCompositionOptionsLayout->setMargin( 0 );
	mCompositionOptionsLayout->addWidget( compositionWidget );

	//read and restore all the items

	//composer labels
	QDomNodeList composerLabelList = composerElem.elementsByTagName( "ComposerLabel" );
	for ( int i = 0; i < composerLabelList.size(); ++i )
	{
		QDomElement currentComposerLabelElem = composerLabelList.at( i ).toElement();
		CCipasComposerLabel* newLabel = new CCipasComposerLabel( mComposition );
		newLabel->readXML( currentComposerLabelElem, doc );
		addComposerLabel( newLabel );
		mComposition->addItem( newLabel );
		mComposition->update();

	}

	//composer GLW
	QDomNodeList composerGLWList = composerElem.elementsByTagName( "ComposerGLW" );
	for ( int i = 0; i < composerGLWList.size(); ++i )
	{
		QDomElement currentComposerMapElem = composerGLWList.at( i ).toElement();
		CCipasComposerGLW* newGLW = new CCipasComposerGLW( mComposition );
		QObject::connect(newGLW, SIGNAL(myImages(float, float)), m_osgViewers, SLOT(GiveYouFuckImage(float, float)));
		connect(m_osgViewers, SIGNAL(toUpdate()), newGLW, SLOT(updateFuckImage()));
		newGLW->SetOsgViewer(m_osgViewers);
		//如果地图截图允许变形请在readXML 之前先  setAdjustExtent setInputExtent
		newGLW->readXML( currentComposerMapElem, doc );
		addComposerGLW( newGLW );
		mComposition->addItem( newGLW );
		mComposition->update();
		mComposition->clearSelection();
		mComposition->setCurrentItem(newGLW);

		QFileInfo fi(mCurrentTemplateName);
		if (fi.fileName()==("ServerGrid.cpt")&&isExtentInput)
		{
			newGLW->setGLExtent(mxmin,mymin,mxmax,mymax);
		}
	}

	//composer scalebars
	QDomNodeList composerScaleBarList = composerElem.elementsByTagName( "ComposerScaleBar" );
	for ( int i = 0; i < composerScaleBarList.size(); ++i )
	{
		QDomElement currentScaleBarElem = composerScaleBarList.at( i ).toElement();
		CCipasComposerScaleBar* newScaleBar = new CCipasComposerScaleBar( mComposition );
		newScaleBar->readXML( currentScaleBarElem, doc );
		addComposerScaleBar( newScaleBar );
		mComposition->addItem( newScaleBar );
		mComposition->update();
	}

	//composer legends
	QDomNodeList composerLegendList = composerElem.elementsByTagName( "ComposerLegend" );
	for ( int i = 0; i < composerLegendList.size(); ++i )
	{
		QDomElement currentLegendElem = composerLegendList.at( i ).toElement();
		CCipasComposerLegend* newLegend = new CCipasComposerLegend( mComposition );
		newLegend->readXML( currentLegendElem, doc );
		goto_gis::LegendInterface* pLI = 0;
		goto_gis::Layer* pLayer = 0;
		goto_gis::Map* pMap = (goto_gis::Map*)m_osgOldViewers->property("Map").toLongLong();
		if (NULL == pMap)
		{
			return;
		}
		QString strLayerName;
		int iLayerID;
		newLegend->getAttachLayerName(strLayerName);

		if(strLayerName.size() <= 0)
		{
			iLayerID = newLegend->getAttachLayerID();
			if(iLayerID < 0)
				pLayer = pMap->CurrentLayer();
			else
				pLayer = pMap->GetLayer(iLayerID);
				
		}
		else
		{
			pLayer = pMap->GetLayer(strLayerName.toStdString());
		}

		if(pLayer)
		{
			goto_gis::SymbolLibraryInterface* pSI = pLayer->GetSymbolLibrary();
			if(pSI)
			{
				pLI = pSI->GetLegend();
				newLegend->SetLegendInterface(pLI);
				newLegend->initLegendMode();
			}
		}
		addComposerLegend( newLegend );
		mComposition->addItem( newLegend );
		mComposition->update();
	}
	
	//composer pictures
	QDomNodeList composerPictureList = composerElem.elementsByTagName( "ComposerPicture" );
	for ( int i = 0; i < composerPictureList.size(); ++i )
	{
		QDomElement currentPictureElem = composerPictureList.at( i ).toElement();
		CCipasComposerPicture* newPicture = new CCipasComposerPicture( mComposition );
		newPicture->readXML( currentPictureElem, doc );
		addComposerPicture( newPicture );
		mComposition->addItem( newPicture );
		mComposition->update();
		//mComposition->clearSelection();
		//newPicture->setSelected( true );
		//showItemOptions( newPicture );
		//mView->composition()->setCurrentItem(newPicture);
	}

	//composer shapes
	QDomNodeList composerShapeList = composerElem.elementsByTagName( "ComposerShape" );
	for ( int i = 0; i < composerShapeList.size(); ++i )
	{
		QDomElement currentShapeElem = composerShapeList.at( i ).toElement();
		CCipasComposerShape* newShape = new CCipasComposerShape( mComposition );
		newShape->readXML( currentShapeElem, doc );
		addComposerShape( newShape );
		mComposition->addItem( newShape );
		mComposition->update();
		//mComposition->clearSelection();
		//newShape->setSelected( true );
		//showItemOptions( newShape );
		//mView->composition()->setCurrentItem(newShape);
	}
	mView->setComposition( mComposition );

	if ( true/*mUndoView*/ )
	{
		//init undo/redo buttons
		//mActionUndo->setEnabled( false );
		//mActionRedo->setEnabled( false );
		//if ( mComposition->undoStack() )
		{
			//mUndoView->setStack( mComposition->undoStack() );
			//connect( mComposition->undoStack(), SIGNAL( canUndoChanged( bool ) ), mActionUndo, SLOT( setEnabled( bool ) ) );
			//connect( mComposition->undoStack(), SIGNAL( canRedoChanged( bool ) ), mActionRedo, SLOT( setEnabled( bool ) ) );
		}
	}
	setSelectionTool();
	//zoomFull();
}

void CCipasComposer::print( QPrinter &printer )
{
	if ( !mComposition || !mView )
		return;

	//set resolution based on composer setting
	printer.setFullPage( true );
	printer.setColorMode( QPrinter::Color );

	//set user-defined resolution
	printer.setResolution( mComposition->printResolution() );

	QPainter p( &printer );

	CCipasComposition::PlotStyle savedPlotStyle = mComposition->plotStyle();
	mComposition->setPlotStyle( CCipasComposition::Print );

	QApplication::setOverrideCursor( Qt::BusyCursor );

	QRectF paperRectMM = printer.pageRect( QPrinter::Millimeter );
	QRectF paperRectPixel = printer.pageRect( QPrinter::DevicePixel );

	mView->setPaintingEnabled( false );
	mComposition->render( &p, paperRectPixel, paperRectMM );
	mView->setPaintingEnabled( true );
	
	mComposition->setPlotStyle( savedPlotStyle );
	QApplication::restoreOverrideCursor();
}
void CCipasComposer::on_mActionAddNewLabel_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::AddLabel );
	}
}

void CCipasComposer::on_mActionAddImage_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::AddPicture );
	}
}
void CCipasComposer::on_mActionAddNewMap_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::AddMap );
	}
}

void CCipasComposer::on_mActionGroupItems_triggered()
{
	if ( mView )
	{
		mView->groupItems();
	}
}

void CCipasComposer::on_mActionUngroupItems_triggered()
{
	if ( mView )
	{
		mView->ungroupItems();
	}
}

void CCipasComposer::on_mActionAddGLW_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::AddGLW );
	}
}

void CCipasComposer::on_mActionAddBasicShape_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::AddShape );
	}
}

void CCipasComposer::on_mActionAddArrow_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::AddArrow );
	}
}

void CCipasComposer::on_mActionAddNewLegend_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::AddLegend );
	}
}
void CCipasComposer::on_mActionAddNewScalebar_triggered()
{
	if ( mView )
	{
		mView->setCurrentTool( CCipasComposerView::AddScalebar );
	}
}

void CCipasComposer::addComposerLabel( CCipasComposerLabel* label )
{
	if ( !label )
	{
		return;
	}
	QString currentLayerNames= label->text();//"雷达";
	CCipasComposerLabelWidget* labelWidget = new CCipasComposerLabelWidget( label ,currentLayerNames);
	//labelWidget->setLayerName(m_pDockMapView->GetMap()->CurrentLayer()->LayerName());
	mItemWidgetMap.insert( label, labelWidget );
}
void CCipasComposer::addComposerMap( CCipasComposerMap* map )
{
	if ( !map )
	{
		return;
	}
}
void CCipasComposer::addComposerPicture( CCipasComposerPicture* picture )
{
	if ( !picture )
	{
		return;
	}

	CCipasComposerPictureWidget* pWidget = new CCipasComposerPictureWidget( picture );
	mItemWidgetMap.insert( picture, pWidget );
}

void CCipasComposer::addComposerScaleBar( CCipasComposerScaleBar* scalebar )
{
	if ( !scalebar )
	{
		return;
	}

	CCipasComposerScaleBarWidget* sbWidget = new CCipasComposerScaleBarWidget( scalebar );
	mItemWidgetMap.insert( scalebar, sbWidget );
}


void CCipasComposer::addComposerShape( CCipasComposerShape* shape )
{
	if ( !shape )
	{
		return;
	}
	CCipasComposerShapeWidget* sWidget = new CCipasComposerShapeWidget( shape );
	mItemWidgetMap.insert( shape, sWidget );
}

void CCipasComposer::addComposerArrow( CCipasComposerArrow* arrow )
{
	if ( !arrow )
	{
		return;
	}

	CCipasComposerArrowWidget* arrowWidget = new CCipasComposerArrowWidget( arrow );
	mItemWidgetMap.insert( arrow, arrowWidget );
}

void CCipasComposer::LoadTemplatefromfile(const QString& filename)
{
	QFile templateFile( filename );
	if ( !templateFile.open( QIODevice::ReadOnly ) )
	{
		QMessageBox::warning( 0, QString::fromLocal8Bit("信息提示"), QString::fromLocal8Bit("不能读取模板") );
		return;
	}

	emit composerWillBeRemoved( mView );

	QDomDocument templateDocument;
	if ( !templateDocument.setContent( &templateFile, false ) )
	{
		QMessageBox::warning( 0, tr( "Read error" ), tr( "Content of template file is not valid" ) );
		return;
	}
	mCurrentTemplateName=filename;
	deleteItems();
	readXML( templateDocument );
//	emit composerAdded( mView );
	QFileInfo fi(filename);
	this->setTitle(tr("制图--")+fi.fileName());
	label_ct->setText(tr("%1").arg(fi.fileName()));
	
	selfAdaptLayout();
}

void CCipasComposer::on_mActionLoadFromTemplate_triggered()
{
	QSettings settings;
	QString openFileDir = settings.value( "UI/lastComposerTemplateDir", "" ).toString();
	QString openFileString = QFileDialog::getOpenFileName( 0,QString::fromLocal8Bit("加载模板"), openFileDir, "*.cpt" );

	if ( openFileString.isEmpty() )
	{
		return; //canceled by the user
	}

	QFileInfo openFileInfo( openFileString );
	settings.setValue( "UI/LastComposerTemplateDir", openFileInfo.absolutePath() );
	//按下了加载模板按钮
	setLoadFromTemplateIsClicked(true);
	LoadTemplatefromfile(openFileString);
	setLoadFromTemplateIsClicked(false);
}

void CCipasComposer::on_mActionSaveAsTemplate_triggered()
{
	QSettings settings;
	QString lastSaveDir = settings.value( "UI/lastComposerTemplateDir", "" ).toString();
	QString saveFileName = QFileDialog::getSaveFileName( 0, QString::fromLocal8Bit("保存模板"), lastSaveDir, "*.cpt" );
	if ( saveFileName.isEmpty() )
	{
		return;
	}

	QFileInfo saveFileInfo( saveFileName );
	//check if suffix has been added
	if ( saveFileInfo.suffix().isEmpty() )
	{
		QString saveFileNameWithSuffix = saveFileName.append( ".cpt" );
		saveFileInfo = QFileInfo( saveFileNameWithSuffix );
	}
	settings.setValue( "UI/LastComposerTemplateDir", saveFileInfo.absolutePath() );

	QFile templateFile( saveFileName );
	if ( !templateFile.open( QIODevice::WriteOnly ) )
	{
		return;
	}

	QDomDocument saveDocument;
	writeXML( saveDocument, saveDocument );

	if ( templateFile.write( saveDocument.toByteArray() ) == -1 )
	{
		QMessageBox::warning( 0, QString::fromLocal8Bit("写入错误"), QString::fromLocal8Bit("不能保存文件") );
	}
}

void CCipasComposer::on_mActionRaiseItems_triggered()
{
	if ( mComposition )
	{
		mComposition->raiseSelectedItems();
	}
}

void CCipasComposer::on_mActionLowerItems_triggered()
{
	if ( mComposition )
	{
		mComposition->lowerSelectedItems();
	}
}

void CCipasComposer::on_mActionMoveItemsToTop_triggered()
{
	if ( mComposition )
	{
		mComposition->moveSelectedItemsToTop();
	}
}

void CCipasComposer::on_mActionMoveItemsToBottom_triggered()
{
	if ( mComposition )
	{
		mComposition->moveSelectedItemsToBottom();
	}
}

void CCipasComposer::on_mActionAlignLeft_triggered()
{
	if ( mComposition )
	{
		mComposition->alignSelectedItemsLeft();
	}
}

void CCipasComposer::on_mActionAlignHCenter_triggered()
{
	if ( mComposition )
	{
		mComposition->alignSelectedItemsHCenter();
	}
}

void CCipasComposer::on_mActionAlignRight_triggered()
{
	if ( mComposition )
	{
		mComposition->alignSelectedItemsRight();
	}
}

void CCipasComposer::on_mActionAlignTop_triggered()
{
	if ( mComposition )
	{
		mComposition->alignSelectedItemsTop();
	}
}

void CCipasComposer::on_mActionAlignVCenter_triggered()
{
	if ( mComposition )
	{
		mComposition->alignSelectedItemsVCenter();
	}
}

void CCipasComposer::on_mActionAlignBottom_triggered()
{
	if ( mComposition )
	{
		mComposition->alignSelectedItemsBottom();
	}
}

void CCipasComposer::setSelectionTool()
{
	mActionSelectMoveItem->setChecked( true );
	on_mActionSelectMoveItem_triggered();
}

void CCipasComposer::on_mCompositionNameComboBox_activated(const QString& text)
{
	if ( text.isEmpty() )
	{
		return;
	}

	if (!mComposition)
	{
		return;
	}

	//extract id
	int id;
	bool conversionOk;
	QStringList textSplit = text.split( " " );
	if ( textSplit.size() < 1 )
	{
		return;
	}

	QString idString = textSplit.at( textSplit.size() - 1 );
	id = idString.toInt( &conversionOk );

	if ( !conversionOk )
	{
		return;
	}
}

void CCipasComposer::moveEvent( QMoveEvent *e )
{
	
}

void CCipasComposer::resizeEvent( QResizeEvent *e )
{
	mSizeGrip->move( rect().bottomRight() - mSizeGrip->rect().bottomRight() );
}

void CCipasComposer::closeEvent(QCloseEvent *event)
{
	saveWindowState();
	int ret = QMessageBox::information(this,tr("提示"),tr("专题图正在运行！\n是否确认关闭？"),QMessageBox::Ok | QMessageBox::Cancel);
	if (ret == 1024)
	{
		event->accept();
		//qApp->quit();
	}
	else
	{
		event->ignore();
	}
	//event->accept();
}

void CCipasComposer::showItemOptions( CComposerItem* item )
{
	QWidget* currentWidget = mItemStackedWidget->currentWidget();

	if ( !item )
	{
		mItemStackedWidget->removeWidget( currentWidget );
		mItemStackedWidget->setCurrentWidget( 0 );
		return;
	}

	QMap<CComposerItem*, QWidget*>::iterator it = mItemWidgetMap.find( item );
	if ( it == mItemWidgetMap.constEnd() )
	{
		return;
	}

	QWidget* newWidget = it.value();

	if ( !newWidget || newWidget == currentWidget ) //bail out if new widget does not exist or is already there
	{
		mOptionsTabWidget->setCurrentIndex(1);//显示当前属性设置页
		return;
	}

	mItemStackedWidget->removeWidget( currentWidget );
	mItemStackedWidget->addWidget( newWidget );
	mItemStackedWidget->setCurrentWidget( newWidget );
	mOptionsTabWidget->setCurrentIndex(1);//显示当前属性设置页
	//newWidget->show();
}

void CCipasComposer::deleteItem( CComposerItem* item )
{
	QMap<CComposerItem*, QWidget*>::iterator it = mItemWidgetMap.find( item );

	if ( it == mItemWidgetMap.end() )
	{
		return;
	}

	//the item itself is not deleted here (usually, this is done in the destructor of QgsAddRemoveItemCommand)
	delete( it.value() );
	mItemWidgetMap.remove( it.key() );
}

void CCipasComposer::deleteItems()
{
	//delete all the items
	QMap<CComposerItem*, QWidget*>::iterator it = mItemWidgetMap.begin();
	for ( ; it != mItemWidgetMap.end(); it++ )
	{
		delete it.value();
		delete it.key();
	}
	mItemWidgetMap.clear();
}

void  CCipasComposer::restoreWindowState()
{
	QSettings settings;
	
	if (isOnlineGraphic)		//在线分析
	{
		restoreState(settings.value("UI/stateOnline").toByteArray());
	}
	else
		restoreState(settings.value("UI/state").toByteArray());
}

void CCipasComposer::saveWindowState()
{
	QSettings settings;
	if (isOnlineGraphic)
	{
		settings.setValue("UI/stateOnline", saveState());
	}
	else
	{
		settings.setValue("UI/state", saveState());
	}
}

void CCipasComposer::on_mOpenPushButton_clicked()
{
	if (mCipasTemplateTree->ItemPath()!="")
	{
		QString selectItemPath=IDPApplication::TemplatePath()+ mCipasTemplateTree->ItemPath();
		setLoadFromTemplateIsClicked(true);
		statusBar()->showMessage(QString::fromLocal8Bit("加载模板"), 2000);
		LoadTemplatefromfile(selectItemPath);
		setLoadFromTemplateIsClicked(false);
		statusBar()->showMessage(QString::fromLocal8Bit("就绪"));
	}
	else
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("请先选择模板."));
	}
}

void CCipasComposer::on_mSavePushButton_clicked()
{
	mCipasTemplateTree->setCurrentItemDescription(textEditC_1->toPlainText());
	mCipasTemplateTree->updateDomElement(mCipasTemplateTree->currentItem(),0);
	mCipasTemplateTree->setCurrentItemDescription("");
	QString templatedb =IDPApplication::TemplatePath()+ "/ThemeTemplate/TemplateDB.xml";
	QFile file(templatedb);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Save Templates DB File"),
			tr("Cannot write file %1:\n%2.")
			.arg(templatedb)
			.arg(file.errorString()));
		return;
	}
	mCipasTemplateTree->write(&file);
	statusBar()->showMessage(QString::fromLocal8Bit("保存设置成功"), 2000);
}

void CCipasComposer::on_mCipasTemplateTree_itemClicked(QTreeWidgetItem *item, int column)
{
	if (item->text(0)==tr("浏览更多……"))
	{
		on_mActionLoadFromTemplate_triggered();
	}
	else
	{
		QString s=mCipasTemplateTree->ItemDescription();
		QByteArray ba = s.toLocal8Bit();
		const char *c_str2 = ba.data();
		textEditC_1->setPlainText(QString::fromLocal8Bit(c_str2));
	}
	
}

void CCipasComposer::on_mCipasTemplateTree_itemDoubleClicked( QTreeWidgetItem * item, int column )
{
	on_mOpenPushButton_clicked();
}

void CCipasComposer::on_saveT_clicked()
{
	QFile templateFile( mCurrentTemplateName );
	//FileMode:: Truncate  覆盖旧的内容
	if ( !templateFile.open( QIODevice::WriteOnly ) )
	{
		return;
	}

	QDomDocument saveDocument;
	writeXML( saveDocument, saveDocument );

	if ( templateFile.write( saveDocument.toByteArray() ) == -1 )
	{
		QMessageBox::warning( 0, QString::fromLocal8Bit("写入错误"), QString::fromLocal8Bit("不能保存文件") );
		statusBar()->showMessage(QString::fromLocal8Bit("保存模板失败"),2000);
	}
	else
	{
		QMessageBox::information( 0, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("保存模板成功") );
		statusBar()->showMessage(QString::fromLocal8Bit("保存模板成功"),2000);
	}
}

void CCipasComposer::on_saveAsT_clicked()
{
	statusBar()->showMessage(QString::fromLocal8Bit("模板另存为"), 2000);
	QDialog *mSaveAsDialog=new QDialog;
	QGridLayout *gdl=new QGridLayout(mSaveAsDialog);
	QLabel *lb1=new QLabel(mSaveAsDialog);
	lb1->setText(QString::fromLocal8Bit("文件名："));
	QLineEdit *mLineEdit=new QLineEdit(mSaveAsDialog);
	mLineEdit->setMinimumWidth(40);
	QPushButton *mPbtnOk=new QPushButton(mSaveAsDialog);
	QPushButton *mPbtnCancle=new QPushButton(mSaveAsDialog);
	mPbtnOk->setText(QString::fromLocal8Bit("确定"));
	mPbtnCancle->setText(QString::fromLocal8Bit("取消"));
	gdl->addWidget(lb1,0,0,1,1);
	gdl->addWidget(mLineEdit,0,1,1,3);
	gdl->addWidget(mPbtnOk,0,4,1,1);
	gdl->addWidget(mPbtnCancle,0,5,1,1);
	mSaveAsDialog->setLayout(gdl);
	mSaveAsDialog->setWindowTitle(QString::fromLocal8Bit("模板另存为"));
	mSaveAsDialog->setWindowIcon(QIcon(":/images/thematicMap/mComposer.png" ));
	connect(mPbtnCancle,SIGNAL(clicked()),mSaveAsDialog,SLOT(close()));
	connect(mPbtnOk,SIGNAL(clicked()),mSaveAsDialog,SLOT(accept()));
	mSaveAsDialog->exec();

	bool saveSuccess=false;
	QString newTs=IDPApplication::TemplatePath()+ "ThemeTemplate/";
	int iR = mSaveAsDialog->result();
	if(iR)
	{
		QString rs=mLineEdit->text().trimmed();
		newTs+=rs;
		QFile templateFile( newTs+".cpt");
		if ( !templateFile.open( QIODevice::WriteOnly ) )
		{
			return;
		}

		QDomDocument saveDocument;
		writeXML( saveDocument, saveDocument );

		if ( templateFile.write( saveDocument.toByteArray() ) == -1 )
		{
			QMessageBox::warning( 0, QString::fromLocal8Bit("写入错误"), QString::fromLocal8Bit("不能保存文件") );
		}
		else
		{
			statusBar()->showMessage(QString::fromLocal8Bit("就绪"));
			saveSuccess=true;
			//增加到模板库
			TemplateStruct ts;
			ts.Name=rs;
			ts.Path="/ThemeTemplate/"+rs+".cpt";
			ts.Description=textEditC_1->toPlainText();
			//ts.IsDefault;ts.IsDefault ;//使用缺省值
			mCipasTemplateTree->insertTemplate(&ts);
			on_mSavePushButton_clicked();
		}
	}
	if (saveSuccess)
	{
		//打开另存的模板
		setLoadFromTemplateIsClicked(true);
		statusBar()->showMessage(QString::fromLocal8Bit("加载模板"), 2000);
		LoadTemplatefromfile(newTs+".cpt");
		setLoadFromTemplateIsClicked(false);
		statusBar()->showMessage(QString::fromLocal8Bit("就绪"));
	}

}


void CCipasComposer::on_mRemoveToolButton_clicked()
{
	//mCipasTemplateTree->removeItemWidget(mCipasTemplateTree->currentItem(),0);
	QTreeWidgetItem *currentItem = mCipasTemplateTree->currentItem();
	if (currentItem)
	{
		mCipasTemplateTree->removeTemplate(currentItem);
	}
}

void CCipasComposer::on_mAddToolButton_clicked()
{
	mCipasTemplateTree->newGroup();
}

void CCipasComposer::on_mEditToolButton_clicked()
{
	QTreeWidgetItem *currentItem = mCipasTemplateTree->currentItem();
	if (currentItem)
	{
		QString Name= mCipasTemplateTree->currentItem()->text(0);
		if (Name==tr("系统模板")||Name==tr("用户模板")||Name==tr("浏览更多……"))
		{
			return;
		}
	}
	CCipasTemplateItemDialog dlg (mCipasTemplateTree->currentItem());
	if (dlg.exec()==QDialog::Accepted)
	{
		mCipasTemplateTree->currentItem()->setText(0,dlg.itemText());
	}
}

void CCipasComposer::on_newBlankT_clicked()
{
	deleteItems();
	//新建空白模板 不带有边框
	mComposition->SetIsDrawBounding(false);
	//"NewTemplate"
	QString rs=	"NewTemplate";
	rs+=QDateTime::currentDateTime().toString("yyMMdd");
	rs+=QDateTime::currentDateTime().toString("hhmmsszzz");
	QString newTs=IDPApplication::TemplatePath()+ "ThemeTemplate/"+rs ;
	bool createSuccess=false;
	if (true)
	{
		QFile templateFile( newTs+".cpt");
		if ( !templateFile.open( QIODevice::WriteOnly ) )
		{
			return;
		}

		QDomDocument saveDocument;
		writeXML( saveDocument, saveDocument );

		if ( templateFile.write( saveDocument.toByteArray() ) == -1 )
		{
			QMessageBox::warning( 0, QString::fromLocal8Bit("写入错误"), QString::fromLocal8Bit("不能创建文件") );
		}
		else
		{
			createSuccess=true;
			//增加到模板库
			TemplateStruct ts;
			ts.Name=rs;
			ts.Path="/ThemeTemplate/"+rs+".cpt";
			ts.Description=textEditC_1->toPlainText();
			//ts.IsDefault;ts.IsDefault ;//使用缺省值
			mCipasTemplateTree->insertTemplate(&ts);
			//仅仅添加到树上，不做保存，这是合理的
			//on_mSavePushButton_clicked();
		}
	}
	
	//
	if (createSuccess)
	{
		//打开另存的模板
		setLoadFromTemplateIsClicked(true);
		statusBar()->showMessage(QString::fromLocal8Bit("加载模板"), 2000);
		LoadTemplatefromfile(newTs+".cpt");
		setLoadFromTemplateIsClicked(false);
		statusBar()->showMessage(QString::fromLocal8Bit("就绪"));
	}
}

void CCipasComposer::setExtent(double xmin,double ymin,double xmax,double ymax)
{
	isExtentInput=true;
	mxmin=xmin;
	mymin=ymin;
	mxmax=xmax;
	mymax=ymax;
}


void CCipasComposer::on_mActionUndo_triggered()
{
	if ( mComposition && mComposition->undoStack() )
	{
		mComposition->undoStack()->undo();
	}
}

void CCipasComposer::on_mActionRedo_triggered()
{
	if ( mComposition && mComposition->undoStack() )
	{
		mComposition->undoStack()->redo();
	}
}

void CCipasComposer::addComposerLegend( CCipasComposerLegend* legend )
{
	CCipasComposerLegendWidget* lWidget = new CCipasComposerLegendWidget(legend);
	//这个很关键，如果 真 ，则是从工具栏按钮上添加图例，
	//这将更新图例的 model，否则不更新
	//不更新意味着model 中的数据来自模板
	//sender() 不够完善，如果通过 加载目标按钮打开模板也是 真 因此 增加一个loadFromTemplateIsClicked
	if ( sender() && !loadFromTemplateIsClicked()) //only update if created from GUI (not after XML read)
	{
		lWidget->updateLegend();
	}
	mItemWidgetMap.insert( legend, lWidget );
}


void CCipasComposer::addComposerGLW(CCipasComposerGLW* glw)
{
	if(!glw)
	{
		return;
	}
	CCipasComposerGLWidget *pWidget=new CCipasComposerGLWidget(glw);
	mItemWidgetMap.insert( glw, pWidget );
}

void CCipasComposer::setOsgView( ViewerQT* pView )
{
	//////////////////////////////////停靠一个OSG窗口////////////////////////////////////////
	ViewerQT* pViewerQT = new ViewerQT(0);
	QDockWidget* dockOSGWindow = new QDockWidget(QString::fromLocal8Bit("属性设置"), this);
	dockOSGWindow->setWidget(pViewerQT);
	//m_osgViewers->setParent(dockOSGWindow);
	//dockOSGWindow->setAllowedAreas(Qt::LeftDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, dockOSGWindow,Qt::Vertical);

	dockProperty->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockTemplate->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	addDockWidget(Qt::RightDockWidgetArea, dockProperty);
	addDockWidget(Qt::RightDockWidgetArea, dockTemplate);
	tabifyDockWidget(dockProperty,dockTemplate);

	osg::Node* pViewScene = pView->getViewer()->getSceneData();
	osg::Vec4 clearColor = pView->getViewer()->getCamera()->getClearColor();
	/*osg::MatrixTransform* pTransNode = dynamic_cast<osg::MatrixTransform*>(pViewScene);
	osg::Matrix ma;*/
	
	pViewerQT->getViewer()->getCamera()->setClearColor(clearColor);
	pViewerQT->getViewer()->setSceneData(pViewScene);
	pViewerQT->getViewer()->setCameraManipulator(pView->getViewer()->getCameraManipulator());
	pViewerQT->initOsgPoster(0);
	pViewerQT->SetImageSize(800, 600);
	m_osgViewers = pViewerQT;
	m_osgOldViewers = pView;
	///////////////////////////////////////////////////////////////////////////////////////
}

void CCipasComposer::UpdateView( ViewerQT* pView )
{
	m_osgViewers->getViewer()->setCameraManipulator(pView->getViewer()->getCameraManipulator());
}
