/*============================================================ 
文件名：statustoolbar.cpp
类 名： StatusToolBar
父 类： QToolBar
子 类： 无 
功能说明：状态工具栏
调用说明：
1、；
2、；

----------------------------版本更新-------------------------
V 1.0 
原作者 ：易家祥
完成日期：2012年 6月11日
修改作者：
完成日期：2012年 6月11日
============================================================*/
#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QAction>
#include <QStatusBar>
#include <QComboBox>
#include <QSpinBox>
#include <QDockWidget>
#include <QLineEdit>
#include <QUuid>
#include <QFileInfo>
#include <QPushButton>
#include <QGLWidget>
#include <stdlib.h>
#include <osgGA/TrackballManipulator>

#include "Map.h"
#include "Layer.h"

#include "ExtendMap.h"

#include "AdapterWidget.h"

#include "ViewManager.h"

#include "AnimaticControler.h"
#include "MyActionInterface.h"
#include "AnimaticAction.h"
#include "statustoolbar.h"
#include "mainwindow.h"
#include "sysmainwindow.h"


StatusToolBar::StatusToolBar(QWidget *parent): QToolBar(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("状态工具栏"));
	setToolButtonStyle(Qt::ToolButtonIconOnly);
	setIconSize(QSize(15,15));   
	m_optionModel = NoneModel;

	//ViewManager* pViewManager = SysMainWindow::GetMainWindow()->GetViewManger();
	//m_AnimaitcControler = new AnimaticControler(pViewManager);
	QToolBar* toolBar = (QToolBar*)SysMainWindow::GetMainWindow()->property("MapToolBar").toLongLong();
	if (toolBar)
	{
		this->addActions(toolBar->actions());
	}
	//QLabel* label = new QLabel("当前投影：兰伯特           ",this);
	//addWidget(label);
	//initStatusToolBar();
	//m_pSpinBox= new QSpinBox(this);
	//addWidget(m_pSpinBox);

	//connect(m_pSpinBox,SIGNAL(valueChanged ( int )),this,SLOT(slot_changLayer(int)));
	//connect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(slot_statusToolBarEvent(QAction*)));
	//connect(m_pTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_currentIndexChanged(int)));
	/*MouseManipulator * m = dynamic_cast<MouseManipulator* >(GraphDisplayManager::GetCurrentDispalyManager()->GetCurrentmainpulator());
	connect(m,SIGNAL(RefreshCoordInfo(QString )),this,SLOT(slot_RefreshCoordInfo(QString)));*/
}

StatusToolBar::~StatusToolBar()
{

}

/************************************************************************
 功能说明：初始化“状态工具栏”
 作    者：易家祥
 完成日期：2011年06月11日
************************************************************************/
void StatusToolBar::initStatusToolBar()
{
	//m_pTypeBox = new QComboBox(this);
	//m_pTypeBox->addItem(tr("导航检索"));
	//m_pTypeBox->addItem(tr("产品列表"));
	//m_pTypeBox->addItem(tr("本地文件"));
	//m_pTypeBox->addItem(tr("关注数据"));

	m_pZoomInAction = new QAction(QIcon(":/images/statusToolBar/zoomIn.png"),tr("放大"),this);
	m_pZoomOutAction = new QAction(QIcon(":/images/statusToolBar/zoomOut.png"),tr("缩小"),this);
	m_pRoamAction = new QAction(QIcon(":/images/statusToolBar/roam.png"),tr("漫游"),this);

	m_pZoomInAction->setEnabled(false);
	m_pZoomOutAction->setEnabled(false);
	m_pRoamAction->setEnabled(false);

	m_pPlayAction = new PlayAction(QIcon(":/images/statusToolBar/play.png"),tr("播放"),this);
	//m_pPlayAction->m_pAnimaticControler = m_AnimaitcControler;
	m_pPauseAction = new PauseAction(QIcon(":/images/statusToolBar/pause.png"),tr("暂停"),this);
	//m_pPauseAction->m_pAnimaticControler = m_AnimaitcControler;
	m_pStopAction = new StopAction(QIcon(":/images/statusToolBar/stop.png"),tr("停止"),this);
	//m_pStopAction->m_pAnimaticControler = m_AnimaitcControler;
	m_pPre1Action = new PreAction(QIcon(":/images/statusToolBar/pre1.png"),tr("上一帧"),this);
	//m_pPre1Action->m_pAnimaticControler = m_AnimaitcControler;
	m_pPreAction = new speeddownAction(QIcon(":/images/statusToolBar/pre.png"),tr("减速"),this);
	//m_pPreAction->m_pAnimaticControler = m_AnimaitcControler;
	m_pNextAction = new speedupAction(QIcon(":/images/statusToolBar/next.png"),tr("加速"),this);
	//m_pNextAction->m_pAnimaticControler = m_AnimaitcControler;
	m_pNext1Action = new NextAction(QIcon(":/images/statusToolBar/next1.png"),tr("下一帧"),this);
	//m_pNext1Action->m_pAnimaticControler = m_AnimaitcControler;
	m_pRefreshAction = new LoopAction(QIcon(":/images/statusToolBar/refresh.png"),tr("循环"),this);
	//m_pRefreshAction->m_pAnimaticControler = m_AnimaitcControler;

	m_pWin4Action = new QAction(QIcon(":/images/statusToolBar/win4.png"),tr("四联屏"),this);
	m_pMeAction = new QAction(QIcon(":/images/statusToolBar/me.png"),tr("尺子"),this);

	m_pWin4Action->setEnabled(false);
	m_pMeAction->setEnabled(false);

	m_pAttentionAction = new QAction(QIcon(":/images/statusToolBar/attention.png"),tr("关注"),this);
	m_pShareAction = new QAction(QIcon(":/images/statusToolBar/share.png"),tr("共享"),this);
	m_pAreaAction = new QAction(QIcon(":/images/statusToolBar/area.png"),tr("面积统计"),this);
	m_pMaxAction = new QAction(QIcon(":/images/statusToolBar/max.png"),tr("最大化"),this);

	m_pAttentionAction->setEnabled(false);
	m_pShareAction->setEnabled(false);
	m_pAreaAction->setEnabled(false);
	m_pMaxAction->setEnabled(false);
	/*addWidget(m_pTypeBox);*/

	addAction(m_pZoomInAction);
	addAction(m_pZoomOutAction);
	addAction(m_pRoamAction);
	addSeparator();

	addAction(m_pPlayAction);
	addAction(m_pPauseAction);
	addAction(m_pStopAction);
	addAction(m_pPre1Action);
	addAction(m_pPreAction);
	addAction(m_pNextAction);
	addAction(m_pNext1Action);
	addAction(m_pRefreshAction);
	addSeparator();

	addAction(m_pWin4Action);
	addAction(m_pMeAction);
	addSeparator();

	addAction(m_pAttentionAction);
	addAction(m_pShareAction);
	addAction(m_pAreaAction);
	addAction(m_pMaxAction);
	addSeparator();

	m_pSpinBox= new QSpinBox(this);
	addWidget(m_pSpinBox);

	connect(m_pSpinBox,SIGNAL(valueChanged ( int )),this,SLOT(slot_changLayer(int)));
}

/************************************************************************
 功能说明：状态工具栏槽函数
 作    者：易家祥
 完成日期：2011年06月12日
************************************************************************/
void StatusToolBar::slot_statusToolBarEvent( QAction* action )
{
	try
	{
			MainWindow* mw = SysMainWindow::GetMainWindow();
			QMdiSubWindow * subW = mw->GetCentralMdiArea()->activeSubWindow();
			int iIndex = -1;
			if (subW)
			{
				QTabWidget* tabW	= dynamic_cast<QTabWidget*>(subW->widget());
				if (tabW->widget(0)->property("DType").toInt() == RadarData)
				{
					MyActionInterface* pAction = dynamic_cast<MyActionInterface*>(action);
					if(pAction)
						pAction->MyAction();
				}
			}
	}
	catch (...)
	{
		QMessageBox::critical(0,tr("提示"),tr("播放雷达动画异常！"));
	}
	//MainWindow* mw = SysMainWindow::GetMainWindow();
	//if (m_pAttentionAction == action)
	//{
	//	SearchWidget* sw = dynamic_cast<SearchWidget*>(mw->getSysMdiArea()->activeSubWindow()->widget());
	//	MapViewWidget* mv = dynamic_cast<MapViewWidget*>(mw->getSysMdiArea()->activeSubWindow()->widget());
	//	
	//	if (sw != NULL)
	//	{
	//		if (sw->windowTitle() == tr("我的关注"))//当前激活窗口为 “我的关注”
	//		{
	//			if (sw->property("guid").toString() != tr(""))
	//			{
	//				QFileInfo temDir(sw->property("guid").toString());	
	//				QString name = temDir.fileName();        
	//				bool bsuccess =  QFile::remove(SysResourcePath::resourcePath()+"/AttentionData/"+name);
	//				QString pa = SysResourcePath::resourcePath()+"/AttentionData/"+temDir.completeBaseName()+tr(".png");
	//				QFile::remove(SysResourcePath::resourcePath()+"/AttentionData/"+temDir.completeBaseName()+tr(".png"));
	//				RadarProductDB::instance()->deleteRadarDBTb(tr("attentionTb")," where guid='"+temDir.fileName().toLower().trimmed()+"'");
	//				sw->initRadarDataList(tr("我的关注"));
	//				sw->setProperty("guid","");
	//			}
	//			else
	//			{
	//				return;
	//			}
	//		}
	//	}
	//	
	//	if (mv != NULL)
	//	{
	//		QString path = mv->property("guid").toString();
	//		if (path != tr(""))
	//		{
	//			QFileInfo temDir(path);
	//			QString name = temDir.fileName();        
	//			bool bsuccess = QFile::copy(path,SysResourcePath::resourcePath()+"/AttentionData/"+name);
	//			if (bsuccess)
	//			{
	//				RadarProductDB::instance()->insertAttentionTb(temDir.fileName().toLower().trimmed(),SysResourcePath::resourcePath()+"/AttentionData/"+name);
	//			}
	//		}
	//	}
	//	//MainWindow* mw = SysMainWindow::GetMainWindow();
	//	//SearchWidget* mv = 0;
	//	//foreach(QMdiSubWindow* subWin ,mw->getSysMdiArea()->subWindowList())
	//	//{
	//	//	SearchWidget* m =  dynamic_cast<SearchWidget*>(subWin->widget());
	//	//	if (m && m->windowTitle() == tr("我的关注"))
	//	//	{
	//	//		mv = m;
	//	//		break;
	//	//	}
	//	//}

	//	//if (mv && mv == dynamic_cast<SearchWidget*>(mw->getSysMdiArea()->activeSubWindow()->widget()))
	//	//{
	//	//	if (mv->property("guid").toString() != tr(""))
	//	//	{
	//	//		QFileInfo temDir(mv->property("guid").toString());	
	//	//		QString name = temDir.fileName();        
	//	//		bool bsuccess =  QFile::remove(SysResourcePath::resourcePath()+"/AttentionData/"+name);
	//	//		QString pa = SysResourcePath::resourcePath()+"/AttentionData/"+temDir.completeBaseName()+tr(".png");
	//	//		QFile::remove(SysResourcePath::resourcePath()+"/AttentionData/"+temDir.completeBaseName()+tr(".png"));
	//	//		//if (bsuccess)
	//	//		//{
	//	//			RadarProductDB::instance()->deleteRadarDBTb(tr("attentionTb")," where guid='"+temDir.fileName().toLower().trimmed()+"'");
	//	//		//}

	//	//	}
	//	//	mv->initRadarDataList(tr("我的关注"));
	//	//}
	//
	//	//else if (mv == NULL)
	//	//{
	//	//	if (!mw->getSysMdiArea()->activeSubWindow())
	//	//	{
	//	//		return;
	//	//	}
	//	//	if (dynamic_cast<MapViewWidget*>(mw->getSysMdiArea()->activeSubWindow()->widget()))
	//	//	{
	//	//		QString path = dynamic_cast<MapViewWidget*>(mw->getSysMdiArea()->activeSubWindow()->widget())->property("guid").toString();
	//	//		if (path != tr(""))
	//	//		{
	//	//			QFileInfo temDir(path);

	//	//			QString name = temDir.fileName();        
	//	//			bool bsuccess = QFile::copy(path,SysResourcePath::resourcePath()+"/AttentionData/"+name);
	//	//			if (bsuccess)
	//	//			{
	//	//				RadarProductDB::instance()->insertAttentionTb(temDir.fileName().toLower().trimmed(),SysResourcePath::resourcePath()+"/AttentionData/"+name);
	//	//			}
	//	//		}
	//	//	}

	//	//	SearchWidget* searchWidget = new SearchWidget(tr("我的关注"),this);
	//	//	mw->getSysMdiArea()->addSubWindow(searchWidget);
	//	//	searchWidget->initRadarDataList(tr("我的关注"));
	//	//	searchWidget->showMaximized();
	//	//}

	//	//else
	//	//{
	//	//	if (dynamic_cast<MapViewWidget*>(mw->getSysMdiArea()->activeSubWindow()->widget()))
	//	//	{
	//	//		QString path = dynamic_cast<MapViewWidget*>(mw->getSysMdiArea()->activeSubWindow()->widget())->property("guid").toString();
	//	//		if (path != tr(""))
	//	//		{
	//	//			QFileInfo temDir(path);

	//	//			QString name = temDir.fileName();        
	//	//			bool bsuccess = QFile::copy(path,SysResourcePath::resourcePath()+"/AttentionData/"+name);
	//	//			if (bsuccess)
	//	//			{
	//	//				 RadarProductDB::instance()->insertAttentionTb(temDir.fileName().toLower().trimmed(),SysResourcePath::resourcePath()+"/AttentionData/"+name);
	//	//			}
	//	//		}
	//	//	}
	//	//}
	//}
}

void StatusToolBar::slot_currentIndexChanged( int index )
{
	MainWindow* mw = SysMainWindow::GetMainWindow();
	//initVisible();
	//closeALLMapView();
	//m_pTypeBox->setCurrentIndex(index);
	//if (index == 0)//导航检索
	//{
	//	m_optionModel = SearchWidgetModel;

	//	mw->getFileToolBar()->getSearchEdit()->show();
	//	mw->getFileToolBar()->getSearchButton()->show();

	//	MainWindow* mw = SysMainWindow::GetMainWindow();
	//	SearchWidget* mv = 0;
	//	foreach(QMdiSubWindow* subWin ,mw->getSysMdiArea()->subWindowList())
	//	{
	//		SearchWidget* m =  dynamic_cast<SearchWidget*>(subWin->widget());
	//		if (m && m->windowTitle() == tr("检索"))
	//		{
	//			mv = m;
	//			mv->setFocus();
	//		}
	//	}
	//	if (mv)
	//	{
	//		//mv->initRadarDataList(mw->getFileToolBar()->getSearchEdit()->text());
	//	}
	//	else
	//	{
	//		SearchWidget* searchWidget = new SearchWidget(tr("检索"),this);
	//		mw->getSysMdiArea()->addSubWindow(searchWidget);
	//		searchWidget->showMaximized();
	//	}

	//	QList<QString> pathList = AttentionLog::instance()->getCloseRadarDbList("SearchWidget");
	//	if (pathList.count()>0)
	//	{
	//		mw->getMapTreeWidgetDockWidget()->show();
	//	}

	//	foreach(QString dataPath,pathList)
	//	{
	//		mw->getFileToolBar()->setEmitForm(dataPath);
	//		mw->getFileToolBar()->EmitOpenSigal(tr("SearchWidget"));
	//	}
	//	mw->getFileToolBar()->getOpenAction()->setVisible(false);


	//}
	//else if (index == 1)//产品列表
	//{
	//	m_optionModel = RadProductWidgetModel;
	//	if (mw->getSRadProductDockWidget())//单站产品
	//	{
	//		mw->getSRadProductDockWidget()->show();
	//	}
	//	if (mw->getMRadProductDockWidget())//组网产品
	//	{
	//		mw->getMRadProductDockWidget()->show();
	//	}

	//	QList<QString> pathList = AttentionLog::instance()->getCloseRadarDbList("RadProductWidget");
	//	if (pathList.count()>0)
	//	{
	//		mw->getMapTreeWidgetDockWidget()->show();
	//	}

	//	foreach(QString dataPath,pathList)
	//	{
	//		mw->getFileToolBar()->setEmitForm(dataPath);
	//		mw->getFileToolBar()->EmitOpenSigal("RadProductWidget");
	//	}


	//}
	//else if (index == 2)//本地文件
	//{
	//	m_optionModel = LoadModel;
	//	if (mw->getRighTreeWidgetDockWidget())//右边本地文件
	//	{
	//		mw->getRighTreeWidgetDockWidget()->show();
	//	}
	//	if (mw->getFileToolBar()->getOpenAction())
	//	{
	//		mw->getFileToolBar()->getOpenAction()->setVisible(true);
	//	}

	//	QList<QString> pathList = AttentionLog::instance()->getCloseRadarDbList("LocalDb");
	//	if (pathList.count()>0)
	//	{
	//		mw->getMapTreeWidgetDockWidget()->show();
	//	}

	//	foreach(QString dataPath,pathList)
	//	{
	//		mw->getFileToolBar()->setEmitForm(dataPath);
	//		mw->getFileToolBar()->EmitOpenSigal("LocalDb");
	//	}
	//	mw->getFileToolBar()->getOpenAction()->setVisible(true);


	//}


	//else if (index == 3)//关注数据
	//{
	//	m_optionModel = AttentionModel;
	//	MainWindow* mw = SysMainWindow::GetMainWindow();
	//	SearchWidget* mv = 0;
	//	foreach(QMdiSubWindow* subWin ,mw->getSysMdiArea()->subWindowList())
	//	{
	//		SearchWidget* m =  dynamic_cast<SearchWidget*>(subWin->widget());
	//		if (m && m->windowTitle() == tr("我的关注"))
	//		{
	//			mv = m;
	//		}
	//	}
	//	if (mv)
	//	{
	//		mv->initRadarDataList(tr("我的关注"));
	//		mv->setFocus();
	//	}
	//	else
	//	{
	//		SearchWidget* searchWidget = new SearchWidget(tr("我的关注"),this);
	//		mw->getSysMdiArea()->addSubWindow(searchWidget);
	//		searchWidget->initRadarDataList(tr("我的关注"));
	//		searchWidget->showMaximized();
	//	}


	//	QList<QString> pathList = AttentionLog::instance()->getCloseRadarDbList("AttentionWidget");
	//	if (pathList.count()>0)
	//	{
	//		mw->getMapTreeWidgetDockWidget()->show();
	//	}

	//	foreach(QString dataPath,pathList)
	//	{
	//		mw->getFileToolBar()->setEmitForm(dataPath);
	//		mw->getFileToolBar()->EmitOpenSigal(tr("AttentionWidget"));
	//	}
	//}

}

void StatusToolBar::initVisible()
{
		//MainWindow* mw = SysMainWindow::GetMainWindow();
		//mw->getFileToolBar()->getSearchEdit()->hide();
		//mw->getFileToolBar()->getSearchButton()->hide();
		//if (mw->getMapTreeWidgetDockWidget())//图层控制
		//{
		//	mw->getMapTreeWidgetDockWidget()->hide();
		//}
		//if (mw->getSRadProductDockWidget())//单站产品
		//{
		//	mw->getSRadProductDockWidget()->hide();
		//}
		//if (mw->getMRadProductDockWidget())//组网产品
		//{
		//	mw->getMRadProductDockWidget()->hide();
		//}
		//if (mw->getRightPicWidgetDockWidget())//右边缩略图
		//{
		//	mw->getRightPicWidgetDockWidget()->hide();
		//}
		//if (mw->getRighTreeWidgetDockWidget())//右边本地文件
		//{
		//	mw->getRighTreeWidgetDockWidget()->hide();
		//}
		//if (mw->getFileToolBar()->getOpenAction())
		//{
		//	mw->getFileToolBar()->getOpenAction()->setVisible(false);
		//}
}

void StatusToolBar::closeALLMapView()
{
	/*MainWindow* mw = SysMainWindow::GetMainWindow();
	QList<QString> dataPathList;
	foreach (QMdiSubWindow* window , mw->getSysMdiArea()->subWindowList())
	{
	MapViewWidget* dv = dynamic_cast<MapViewWidget*>(window->widget());
	if (dv != 0)
	{
	if (tr("") != dv->whatsThis())
	{
	dataPathList.push_back(dv->whatsThis()+"|"+dv->property("guid").toString());
	}
	dv->setProperty("IndexChangedClose",true);
	}
	window->close();
	}
	AttentionLog::instance()->writeCloseRadarDbFrom(dataPathList);
	dataPathList.clear();*/
}

void StatusToolBar::slot_RefreshCoordInfo( QString mess )
{
	/*dynamic_cast<QStatusBar*>(this->parentWidget())->showMessage(mess);*/
}

void StatusToolBar::slot_changLayer( int index )
{
	try
	{
		MainWindow* mw = SysMainWindow::GetMainWindow();
		QMdiSubWindow * subW = mw->GetCentralMdiArea()->activeSubWindow();
		if (subW)
		{
			ViewerQT* pAdapterWidget = 0;
			QTabWidget* tabW	= dynamic_cast<QTabWidget*>(subW->widget());

			int wdt = tabW->widget(0)->property("DType").toInt();

			if (wdt != RadarData && wdt != Win4)
			{
				return;
			}
			//////////////////////////////////////////////////////////////////////////

			if (wdt == Win4)
			{
				pAdapterWidget = dynamic_cast<ViewerQT*>(mw->GetCentralMdiArea()->CurrentWidget());
			}
			else
			{
				pAdapterWidget = dynamic_cast<ViewerQT*>(tabW->widget(0));
			}
			//ViewerQT* pAdapterWidget = dynamic_cast<ViewerQT*>(subW->widget());
			ExtendMap* pMap = (ExtendMap*)pAdapterWidget->property("Map").toLongLong();;
			Map * mp= pMap;
			std::string layername = "第" + QString::number(index).toStdString()+"仰角层";

			std::map<std::string, Layer*> m_mapLayer = mp->GetMapLayers();
			std::map<std::string, Layer*>::iterator layerIter = m_mapLayer.begin();

			QStringList blist = mp->BasisLayerList();
			//blist<<tr("中国地图")<<tr("中国省会")<<tr("中国首都")<<tr("国界线");

			while(layerIter != m_mapLayer.end())
			{
				if (blist.contains(QString::fromStdString(layerIter->first)))
				{
					layerIter++;
					continue;
				}
				Layer* layer = layerIter->second;
				if(layer)
				{
					if (layerIter->first == layername)
					{
						mp->SetVisibleLayer(layerIter->first,true);
					}
					else
					{
						mp->SetVisibleLayer(layerIter->first,false);
					}
				}
				layerIter++;
			}

		}
	}
	
	catch (...)
	{
	}
	
}
