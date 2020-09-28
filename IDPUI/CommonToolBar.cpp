

#include "IDPdb.h"

#include "IDPApplication.h"
#include "AdapterWidget.h"

#include "DataProvider.h"

#include "mainwindow.h"
#include "sysmainwindow.h"
#include "CipasComposer.h"

#include "DrawGeometryActions.h"
#include "CommonDataAccess.h"
#include "CommonToolBar.h"

 

CommonToolBar::CommonToolBar(QWidget *parent): QToolBar(parent),m_CommonActionGroup(0)
{
	ui.setupUi(this);
	m_pCipasComposer = 0;

	_mw = SysMainWindow::GetMainWindow();

	QAction* actionMapProj = new QAction(QIcon(":/images/commonToolBar/mapproj.png"),tr("投影"),this);
	actionMapProj->setObjectName(QString::fromUtf8("actionMapProj"));
	QMenu* menu = new QMenu();

	m_pActionL = new  QAction(tr("兰伯特投影"),menu);
	m_pActionL->setObjectName("m_pActionL");
	m_pActionM = new QAction(tr("墨卡托投影"),menu);
	m_pActionM->setObjectName("m_pActionM");

	menu->addAction(m_pActionL);
	menu->addAction(m_pActionM);

	actionMapProj->setMenu(menu);

	m_CommonActionGroup.addAction(m_pActionL);
	m_CommonActionGroup.addAction(m_pActionM);
	
	QAction* pAction = 0;
	pAction = new DrawLineAction(ui.actionVLine->icon(), ui.actionVLine->text(), this);
	this->removeAction(ui.actionVLine);
	this->addAction(pAction);

	pAction = new DrawRectAction(ui.actionRect->icon(), ui.actionRect->text(), this);
	this->removeAction(ui.actionRect);
	this->addAction(pAction);

	pAction = new DrawCircleAction(ui.actionRound->icon(), ui.actionRound->text(), this);
	this->removeAction(ui.actionRound);
	this->addAction(pAction);

	pAction = new DrawPolygonalLineAction(ui.actionFLine->icon(), ui.actionFLine->text(), this);
	this->removeAction(ui.actionFLine);
	this->addAction(pAction);

	pAction = new HandAction(ui.actionMouse->icon(), ui.actionMouse->text(), this);
	this->removeAction(ui.actionMouse);
	this->addAction(pAction);

	pAction = new DrawColdFrontAction(QIcon(":/images/commonToolBar/t_lf.png"), "冷锋线", this);
	this->addAction(pAction);

	pAction = new DrawWarmFrontAction(QIcon(":/images/commonToolBar/t_nf.png"), "暖锋线", this);
	this->addAction(pAction);

	pAction = new DrawOcclusionFrontAction(QIcon(":/images/commonToolBar/t_gqf.png"), "固囚锋线", this);
	this->addAction(pAction);

	DrawWindAction* pWindAction  = new DrawWindAction(QIcon(":/images/commonToolBar/P_12Feng.png"), "风羽", this);
	pWindAction->SetWindSpeed(2);
	this->addAction(pWindAction);

	DrawSymbol1Action* pDrawSymbolAction = new DrawSymbol1Action(QIcon(":/images/commonToolBar/eidt.png"), "绘制符号", this);
	this->addAction(pDrawSymbolAction);

	DeleteSymbolAction* pDeleteSymbolAction = new DeleteSymbolAction(QIcon(":/images/commonToolBar/delete.png"), "删除符号", this);
	this->addAction(pDeleteSymbolAction);

	MoveSymbolAction* pMoveSymbolAction = new MoveSymbolAction(ui.actionMouse->icon(), "移动符号", this);
	this->addAction(pMoveSymbolAction);

	this->insertAction(ui.actionThematic,actionMapProj);
	
	foreach(QAction* action,this->actions())
	{
		m_CommonActionGroup.addAction(action);
	}
	
	InitSlots();
}

CommonToolBar::~CommonToolBar()
{
	
}

void CommonToolBar::InitSlots()
{
	connect(&m_CommonActionGroup, SIGNAL(triggered(QAction*)),this,  SLOT(Slots_ActionTriggered(QAction*)));
}

void CommonToolBar::Slots_ActionTriggered( QAction* action )
{
	MyActionInterface* pMyAction = dynamic_cast<MyActionInterface*>(action);
	if(pMyAction) pMyAction->MyAction();
	try 
	{
		ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(_mw->FromWidgetToQGLW());
		if (viewerWindow == NULL)
		{
			return;
		}

		QSettings setting2(IDPApplication::ConfigPath()+"ProjectionType.ini", QSettings::IniFormat);
		//setting2 = QSettings("ProjectionType.ini", QSettings::IniFormat);
		setting2.setIniCodec(QTextCodec::codecForName("UTF-8"));	
		if (action == ui.actionThematic)
		{
			m_pCipasComposer = new CCipasComposer(this);
			m_pCipasComposer->setOsgView(viewerWindow);
			m_pCipasComposer->setWindowModality(Qt::ApplicationModal);
			
			//m_pCCipasComposer->setWindowModality(Qt::WindowModal);
			m_pCipasComposer->setExtent(0,-90,180,90);
			//m_pCipasComposer->setExtent(x_min,y_mim,x_max,y_max);

			m_pCipasComposer->statusBar()->showMessage(tr("就绪"));
			QString templateString =IDPApplication::TemplatePath()+ "/ThemeTemplate/ServerGrid.cpt";
			m_pCipasComposer->LoadTemplatefromfile(templateString);
			m_pCipasComposer->open();
			m_pCipasComposer->showMaximized();
		}
		else if (action == m_pActionL)
		{
				int wdt = viewerWindow->property("DType").toInt();
				goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();;
				if (pMap == NULL)
				{
					//QMessageBox::information(this,"提示","获取视口Map失败！");
					return;
				}
				SnapImageDrawCallback* pImageDrawCallback = new SnapImageDrawCallback;
				pImageDrawCallback->setFileName((IDPApplication::TempPath()+"Thematic.png").toStdString());
				viewerWindow->getViewer()->getCamera()->setFinalDrawCallback(pImageDrawCallback);

				CreateImage();
				pMap->GetCoordinateTransform()->setSourceCRS(setting2.value("/GeographicCoordinate_CONFIG/WGS84").toString().toStdString());
				pMap->GetCoordinateTransform()->setDestCRS(setting2.value("/ProjectionCoordinate_CONFIG/LAMBERT").toString().toStdString());
				pMap->GetCoordinateTransform()->setDesProjName("兰伯特投影");
				pMap->TramsformMap();
				pMap->ScaleToLayer();
		}
		else if (action == m_pActionM)
		{
				int wdt = viewerWindow->property("DType").toInt();
				goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();//Map2D();
				if (pMap == NULL)
				{
					//QMessageBox::information(this,"提示","获取视口Map失败！");
					return;
				}
				pMap->GetCoordinateTransform()->setSourceCRS(setting2.value("/GeographicCoordinate_CONFIG/WGS84").toString().toStdString());
				pMap->GetCoordinateTransform()->setDestCRS(setting2.value("/ProjectionCoordinate_CONFIG/MERCATOR").toString().toStdString());
				pMap->GetCoordinateTransform()->setDesProjName("墨卡托投影");
				pMap->TramsformMap();
				pMap->ScaleToLayer();
		}
	}
	catch (...)
	{
	}
}

DefaultManipulator* CommonToolBar::GetDM( QWidget* viewerWidget )
{
	DefaultManipulator* dm = 0;
	if (viewerWidget == NULL)
	{
		//QMessageBox::information(this,"提示","获取视口失败！");
		return 0;
	}
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(viewerWidget);
	int wdt = viewerWindow->property("DType").toInt();
	if (wdt == RadarData)
	{
		dm = dynamic_cast<DefaultManipulator*>(viewerWindow->getViewer()->getCameraManipulator());
	}
	else if (wdt == CZData)
	{
		osgGA::KeySwitchMatrixManipulator* keym = dynamic_cast<osgGA::KeySwitchMatrixManipulator*>(viewerWindow->getViewer()->getCameraManipulator());
		if (keym)
		{
			keym->selectMatrixManipulator('1');
			dm = dynamic_cast<DefaultManipulator*>(keym->getCurrentMatrixManipulator());
		}
	}
	return dm;
}

void CommonToolBar::eMapMove( QPoint beginPoint , QPoint endPoint )
{
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(_mw->FromWidgetToQGLW());
	DefaultManipulator* pDM = GetDM(viewerWindow);
	if(pDM)
	{
		if(beginPoint.x() - endPoint.x() < 0)
		{
			 pDM->MapRight();
		}
		if(beginPoint.y() - endPoint.y() < 0)
		{
			pDM->MapBottom();
		}
		if(beginPoint.x() - endPoint.x() > 0)
		{
			pDM->MapLeft();
		}
		if(beginPoint.y() - endPoint.y() > 0)
		{
			pDM->MapTop();
		}
	}
	CreateImage();
}

void CommonToolBar::MapZoonOut()
{
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(_mw->FromWidgetToQGLW());
	DefaultManipulator* pDM = GetDM(viewerWindow);

	if(pDM) pDM->MapMZoomOut();

	CreateImage();
}

void CommonToolBar::MapZoonIn()
{
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(_mw->FromWidgetToQGLW());
	DefaultManipulator* pDM = GetDM(viewerWindow);

	if(pDM) pDM->MapMZoomIn();
	CreateImage();
}

void CommonToolBar::MapFull()
{
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(_mw->FromWidgetToQGLW());
	DefaultManipulator* pDM = GetDM(viewerWindow);

	if(pDM) pDM->MapALLMap();
	CreateImage();
}

void CommonToolBar::CreateImage()
{
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(_mw->FromWidgetToQGLW());
	if (viewerWindow == NULL)
	{
		QMessageBox::information(this,"提示","获取视口失败！");
		return;
	}
	SnapImageDrawCallback* pImageDrawCallback = dynamic_cast<SnapImageDrawCallback*>(viewerWindow->getViewer()->getCamera()->getFinalDrawCallback());
	if(!pImageDrawCallback)
		return;
	pImageDrawCallback->setSnapImageOnNextFrame(true);
	viewerWindow->getViewer()->frame();
}

void CommonToolBar::setVisible( bool visible )
{
	//if (visible)
	{
		QAction* action = (QAction*)this->property("Action").toLongLong();
		if (action)
		{
			action->setCheckable(visible);
			action->setChecked(visible);
		}
	}
	QToolBar::setVisible(visible);
}
