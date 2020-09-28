/*============================================================ 
�ļ�����statustoolbar.cpp
�� ���� StatusToolBar
�� �ࣺ QToolBar
�� �ࣺ �� 
����˵����״̬������
����˵����
1����
2����

----------------------------�汾����-------------------------
V 1.0 
ԭ���� ���׼���
������ڣ�2012�� 6��11��
�޸����ߣ�
������ڣ�2012�� 6��11��
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
	setWindowTitle(tr("״̬������"));
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
	//QLabel* label = new QLabel("��ǰͶӰ��������           ",this);
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
 ����˵������ʼ����״̬��������
 ��    �ߣ��׼���
 ������ڣ�2011��06��11��
************************************************************************/
void StatusToolBar::initStatusToolBar()
{
	//m_pTypeBox = new QComboBox(this);
	//m_pTypeBox->addItem(tr("��������"));
	//m_pTypeBox->addItem(tr("��Ʒ�б�"));
	//m_pTypeBox->addItem(tr("�����ļ�"));
	//m_pTypeBox->addItem(tr("��ע����"));

	m_pZoomInAction = new QAction(QIcon(":/images/statusToolBar/zoomIn.png"),tr("�Ŵ�"),this);
	m_pZoomOutAction = new QAction(QIcon(":/images/statusToolBar/zoomOut.png"),tr("��С"),this);
	m_pRoamAction = new QAction(QIcon(":/images/statusToolBar/roam.png"),tr("����"),this);

	m_pZoomInAction->setEnabled(false);
	m_pZoomOutAction->setEnabled(false);
	m_pRoamAction->setEnabled(false);

	m_pPlayAction = new PlayAction(QIcon(":/images/statusToolBar/play.png"),tr("����"),this);
	//m_pPlayAction->m_pAnimaticControler = m_AnimaitcControler;
	m_pPauseAction = new PauseAction(QIcon(":/images/statusToolBar/pause.png"),tr("��ͣ"),this);
	//m_pPauseAction->m_pAnimaticControler = m_AnimaitcControler;
	m_pStopAction = new StopAction(QIcon(":/images/statusToolBar/stop.png"),tr("ֹͣ"),this);
	//m_pStopAction->m_pAnimaticControler = m_AnimaitcControler;
	m_pPre1Action = new PreAction(QIcon(":/images/statusToolBar/pre1.png"),tr("��һ֡"),this);
	//m_pPre1Action->m_pAnimaticControler = m_AnimaitcControler;
	m_pPreAction = new speeddownAction(QIcon(":/images/statusToolBar/pre.png"),tr("����"),this);
	//m_pPreAction->m_pAnimaticControler = m_AnimaitcControler;
	m_pNextAction = new speedupAction(QIcon(":/images/statusToolBar/next.png"),tr("����"),this);
	//m_pNextAction->m_pAnimaticControler = m_AnimaitcControler;
	m_pNext1Action = new NextAction(QIcon(":/images/statusToolBar/next1.png"),tr("��һ֡"),this);
	//m_pNext1Action->m_pAnimaticControler = m_AnimaitcControler;
	m_pRefreshAction = new LoopAction(QIcon(":/images/statusToolBar/refresh.png"),tr("ѭ��"),this);
	//m_pRefreshAction->m_pAnimaticControler = m_AnimaitcControler;

	m_pWin4Action = new QAction(QIcon(":/images/statusToolBar/win4.png"),tr("������"),this);
	m_pMeAction = new QAction(QIcon(":/images/statusToolBar/me.png"),tr("����"),this);

	m_pWin4Action->setEnabled(false);
	m_pMeAction->setEnabled(false);

	m_pAttentionAction = new QAction(QIcon(":/images/statusToolBar/attention.png"),tr("��ע"),this);
	m_pShareAction = new QAction(QIcon(":/images/statusToolBar/share.png"),tr("����"),this);
	m_pAreaAction = new QAction(QIcon(":/images/statusToolBar/area.png"),tr("���ͳ��"),this);
	m_pMaxAction = new QAction(QIcon(":/images/statusToolBar/max.png"),tr("���"),this);

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
 ����˵����״̬�������ۺ���
 ��    �ߣ��׼���
 ������ڣ�2011��06��12��
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
		QMessageBox::critical(0,tr("��ʾ"),tr("�����״ﶯ���쳣��"));
	}
	//MainWindow* mw = SysMainWindow::GetMainWindow();
	//if (m_pAttentionAction == action)
	//{
	//	SearchWidget* sw = dynamic_cast<SearchWidget*>(mw->getSysMdiArea()->activeSubWindow()->widget());
	//	MapViewWidget* mv = dynamic_cast<MapViewWidget*>(mw->getSysMdiArea()->activeSubWindow()->widget());
	//	
	//	if (sw != NULL)
	//	{
	//		if (sw->windowTitle() == tr("�ҵĹ�ע"))//��ǰ�����Ϊ ���ҵĹ�ע��
	//		{
	//			if (sw->property("guid").toString() != tr(""))
	//			{
	//				QFileInfo temDir(sw->property("guid").toString());	
	//				QString name = temDir.fileName();        
	//				bool bsuccess =  QFile::remove(SysResourcePath::resourcePath()+"/AttentionData/"+name);
	//				QString pa = SysResourcePath::resourcePath()+"/AttentionData/"+temDir.completeBaseName()+tr(".png");
	//				QFile::remove(SysResourcePath::resourcePath()+"/AttentionData/"+temDir.completeBaseName()+tr(".png"));
	//				RadarProductDB::instance()->deleteRadarDBTb(tr("attentionTb")," where guid='"+temDir.fileName().toLower().trimmed()+"'");
	//				sw->initRadarDataList(tr("�ҵĹ�ע"));
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
	//	//	if (m && m->windowTitle() == tr("�ҵĹ�ע"))
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
	//	//	mv->initRadarDataList(tr("�ҵĹ�ע"));
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

	//	//	SearchWidget* searchWidget = new SearchWidget(tr("�ҵĹ�ע"),this);
	//	//	mw->getSysMdiArea()->addSubWindow(searchWidget);
	//	//	searchWidget->initRadarDataList(tr("�ҵĹ�ע"));
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
	//if (index == 0)//��������
	//{
	//	m_optionModel = SearchWidgetModel;

	//	mw->getFileToolBar()->getSearchEdit()->show();
	//	mw->getFileToolBar()->getSearchButton()->show();

	//	MainWindow* mw = SysMainWindow::GetMainWindow();
	//	SearchWidget* mv = 0;
	//	foreach(QMdiSubWindow* subWin ,mw->getSysMdiArea()->subWindowList())
	//	{
	//		SearchWidget* m =  dynamic_cast<SearchWidget*>(subWin->widget());
	//		if (m && m->windowTitle() == tr("����"))
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
	//		SearchWidget* searchWidget = new SearchWidget(tr("����"),this);
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
	//else if (index == 1)//��Ʒ�б�
	//{
	//	m_optionModel = RadProductWidgetModel;
	//	if (mw->getSRadProductDockWidget())//��վ��Ʒ
	//	{
	//		mw->getSRadProductDockWidget()->show();
	//	}
	//	if (mw->getMRadProductDockWidget())//������Ʒ
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
	//else if (index == 2)//�����ļ�
	//{
	//	m_optionModel = LoadModel;
	//	if (mw->getRighTreeWidgetDockWidget())//�ұ߱����ļ�
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


	//else if (index == 3)//��ע����
	//{
	//	m_optionModel = AttentionModel;
	//	MainWindow* mw = SysMainWindow::GetMainWindow();
	//	SearchWidget* mv = 0;
	//	foreach(QMdiSubWindow* subWin ,mw->getSysMdiArea()->subWindowList())
	//	{
	//		SearchWidget* m =  dynamic_cast<SearchWidget*>(subWin->widget());
	//		if (m && m->windowTitle() == tr("�ҵĹ�ע"))
	//		{
	//			mv = m;
	//		}
	//	}
	//	if (mv)
	//	{
	//		mv->initRadarDataList(tr("�ҵĹ�ע"));
	//		mv->setFocus();
	//	}
	//	else
	//	{
	//		SearchWidget* searchWidget = new SearchWidget(tr("�ҵĹ�ע"),this);
	//		mw->getSysMdiArea()->addSubWindow(searchWidget);
	//		searchWidget->initRadarDataList(tr("�ҵĹ�ע"));
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
		//if (mw->getMapTreeWidgetDockWidget())//ͼ�����
		//{
		//	mw->getMapTreeWidgetDockWidget()->hide();
		//}
		//if (mw->getSRadProductDockWidget())//��վ��Ʒ
		//{
		//	mw->getSRadProductDockWidget()->hide();
		//}
		//if (mw->getMRadProductDockWidget())//������Ʒ
		//{
		//	mw->getMRadProductDockWidget()->hide();
		//}
		//if (mw->getRightPicWidgetDockWidget())//�ұ�����ͼ
		//{
		//	mw->getRightPicWidgetDockWidget()->hide();
		//}
		//if (mw->getRighTreeWidgetDockWidget())//�ұ߱����ļ�
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
			std::string layername = "��" + QString::number(index).toStdString()+"���ǲ�";

			std::map<std::string, Layer*> m_mapLayer = mp->GetMapLayers();
			std::map<std::string, Layer*>::iterator layerIter = m_mapLayer.begin();

			QStringList blist = mp->BasisLayerList();
			//blist<<tr("�й���ͼ")<<tr("�й�ʡ��")<<tr("�й��׶�")<<tr("������");

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
