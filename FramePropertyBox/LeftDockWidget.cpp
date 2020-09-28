#include <stdafx.h>
#include <osgViewer/Viewer>
//#include <osgEarthUtil/EarthManipulator>
#include "MainControlFrame.h"
#include "MainFrameWindow.h"
#include "WindowFrameContext.h"
#include "ControllerFrameContext.h"
#include "Map.h"
#include "Layer.h"
#include "AdapterWidget.h"
using namespace goto_gis;

#include "DockWidgetInterface.h"
#include "FieldDialog.h"
#include "LeftDockWidget.h"

LeftDockWidget::LeftDockWidget(QWidget *parent /*= 0*/ ): QDockWidget(parent)
	, m_pTreeViewEventInterface(0), m_pActionGroup(new QActionGroup(this))
{
	ui.setupUi(this);
	//m_pMW = SysMainWindow::GetMainWindow();

	m_pRightMenu = NULL;
	m_pFieldDialog = NULL;
	m_pBasisLayerTreeItem = NULL;
	m_pProuductLayerTreeItem = NULL;

	m_pRightMenu = new QMenu(ui.treeWidget);

	//InitLeftDockWidget();
	ui.treeWidget->expandAll();
	ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);		//鼠标右键显示菜单
	ui.treeWidget->setRootIsDecorated(false);
	ui.treeWidget->setAlternatingRowColors(true);
	ui.treeWidget->setColumnWidth(0, 60);
	InitSlots();
}

LeftDockWidget::~LeftDockWidget()
{

}

void LeftDockWidget::InitSlots()
{
	connect(ui.treeWidget, SIGNAL(clicked(const QModelIndex&)), this, SLOT(Slot_ItemClick(const QModelIndex&)));//itemChanged  itemClicked
	connect(ui.treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(Slot_ShowContextMenu(const QPoint&)));
	connect(ui.treeWidget, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(Slot_itemDoubleClicked(const QModelIndex&)));
	connect(m_pActionGroup, SIGNAL(triggered( QAction *)), this, SLOT(Slot_ActionTriggered_dock(QAction*)));
	//connect(ui.pushButton_2, SIGNAL(clicked()), SLOT(Slot_MoveDown()));
	//connect(ui.pushButton, SIGNAL(clicked()), SLOT(Slot_MoveUp()));
	//connect(ui.pushButton_addbaseMap, SIGNAL(clicked()), this, SLOT(Slot_AddBaseMap()));
}

void LeftDockWidget::InitLeftDockWidget()
{
	//ui.gridLayout->setContentsMargins(1,0,0,0);
	//ui.gridLayout_2->setContentsMargins(0,0,0,0);
	//m_pBasisLayerTreeItem = new QTreeWidgetItem(ui.treeWidget);
	//m_pBasisLayerTreeItem->setText(0,tr("基础图层"));
	//m_pProuductLayerTreeItem = new QTreeWidgetItem(ui.treeWidget);
	//m_pProuductLayerTreeItem->setText(0,tr("产品图层"));

	//ui.treeWidget->addTopLevelItem(m_pBasisLayerTreeItem);
	//ui.treeWidget->addTopLevelItem(m_pProuductLayerTreeItem);
}


/** 
  * @brief    点击左侧树槽   
  * @author   易家祥  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   LeftDockWidget  点击左侧树槽
  * @note     易家祥
  * @date     2013年3月                               
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 

void LeftDockWidget::Slot_ItemClick(const QModelIndex& index)
{
	if(m_pTreeViewEventInterface)
		m_pTreeViewEventInterface->ItemClick(index);
}


void LeftDockWidget::Slot_ShowContextMenu( const QPoint& point )
{
	m_pRightMenu->exec(QCursor::pos());

	//int nIndex = ui.treeWidget->indexOfTopLevelItem(ui.treeWidget->currentItem());
	//if (ui.treeWidget->currentItem() == m_pBasisLayerTreeItem)
	//{
	//	//菜单//
	//	if(m_pRightMenu)
	//	{
	//		delete m_pRightMenu;
	//		m_pRightMenu = NULL;
	//	}
	//	m_pRightMenu = new QMenu(ui.treeWidget);

	//	QAction* actionAdd = m_pRightMenu->addAction(tr("添加底图"));
	//	connect(actionAdd, SIGNAL(triggered()), this, SLOT(Slot_TreeRightMenuAction()));

	//	QAction* actionMapBg = m_pRightMenu->addAction(tr("视口背景"));
	//	connect(actionMapBg, SIGNAL(triggered()), this, SLOT(Slot_TreeRightMenuAction()));

	//	m_pRightMenu->exec(QCursor::pos());
	//}
	//else
	//{
	//	if (ui.treeWidget->currentItem() != m_pProuductLayerTreeItem)
	//	{
	//		if (m_pBasisLayerTreeItem->indexOfChild(ui.treeWidget->currentItem()) == -1 || m_pProuductLayerTreeItem->indexOfChild(ui.treeWidget->currentItem()) == -1)
	//		{
	//			//菜单//
	//			if(m_pRightMenu)
	//			{
	//				delete m_pRightMenu;
	//				m_pRightMenu = NULL;
	//			}
	//			m_pRightMenu = new QMenu(ui.treeWidget);

	//			QAction *action = m_pRightMenu->addAction(tr("删除图层"));//显示图层字段信息
	//			connect(action, SIGNAL(triggered()), this, SLOT(Slot_TreeRightMenuAction()));

	//			QAction* actionFiled = m_pRightMenu->addAction(tr("显示图层字段信息"));
	//			connect(actionFiled, SIGNAL(triggered()), this, SLOT(Slot_TreeRightMenuAction()));

	//			m_pRightMenu->exec(QCursor::pos());
	//		}
	//	}
	//	
	//}
}

void LeftDockWidget::Slot_TreeRightMenuAction()
{
	try
	{
		/*QWidget* activateW = m_pMW->FromWidgetToQGLW();
		int wdt = 0;
		if (activateW)
		{
			wdt = activateW->property("DType").toInt();
		}
		QWidget* pViewer = 0;
		if (wdt == Win4)
		{
			pViewer = m_pMW->GetCentralMdiArea()->CurrentWidget();
		}
		else
		{
			pViewer = dynamic_cast<ViewerQT*>(activateW);
		}
		if (pViewer == NULL)
		{
			return;
		}*/

		//Map* pMap = (Map*)pViewer->property("Map").toLongLong();

		
		Map* pMap = GetCurentMap();

		if (pMap == NULL)
		{
			return;
		}
		
		QAction* action = dynamic_cast<QAction*>(sender());
		if (action->text() == tr("显示图层字段信息"))
		{
			//if (m_pFieldDialog)
			//{
			//	delete m_pFieldDialog;
			//	m_pFieldDialog = NULL;
			//}
			//QString layerName = ui.treeWidget->currentItem()->text(0);
			//Layer* layer = pMap->GetLayer(layerName.toStdString());
			//if (layer)
			//{
			//	//std::map<int, GeomField>* mapField = layer->GetFieldMap();
			//	m_pFieldDialog = new FieldDialog(layer,this);
			//	m_pFieldDialog->showMaximized();
			//}
		}
		else if (action->text() == tr("删除图层"))
		{
			/*QString layerName = ui.treeWidget->currentItem()->text(0);
			if (pMap->RemoveLayer(layerName.toStdString()))
			{
				if (ui.treeWidget->currentItem()->parent()->text(0) == tr("基础图层"))
				{
					m_pBasisLayerTreeItem->removeChild(ui.treeWidget->currentItem());
				}
				else if (ui.treeWidget->currentItem()->parent()->text(0) == tr("产品图层"))
				{
					m_pProuductLayerTreeItem->removeChild(ui.treeWidget->currentItem());
				}
			}*/
		}
		else if (action->text()== tr("添加底图"))
		{
			QString fileName = QFileDialog::getOpenFileName(this,tr("打开地图文件"), "/", tr("矢量文件 (*.shp);;TIFF文件 (*.tif)"));//
			if(fileName == "")
			{
				return;
			}
			/*GISMapManager* gisMapManager = m_pMW->GetViewManger()->GetGISMapManager();
			if (gisMapManager)
			{
			gisMapManager->AddLayer(pMap,fileName);
			pMap->BasisLayerList().append(fileName);
			QTreeWidgetItem* item = new QTreeWidgetItem(m_pBasisLayerTreeItem);
			item->setCheckState(0,Qt::Checked);
			item->setText(0,fileName);
			m_pBasisLayerTreeItem->addChild(item);
			}*/
		}
		else if (tr("视口背景") == action->text())
		{
			
			ViewerQT* activateW = GetCurentViewerQT();

			QColor color = QColorDialog::getColor(Qt::white,this);
			if (color.isValid())
			{
				dynamic_cast<ViewerQT*>(activateW)->getViewer()->getCamera()->setClearColor(osg::Vec4f(color.redF(),color.greenF(),color.blueF(),color.alphaF()));
				dynamic_cast<ViewerQT*>(activateW)->getViewer()->getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//QString qss = "background-color: rgb(" + QString::number(color.red())+"," + QString::number(color.green())+ ","+QString::number(color.blue())+");border:-1px;";
				//pViewer->setStyleSheet(qss);

			}
		}
			else if (action->text() == tr("显示图层"))
			{
				//QString layerName = 0;
				//layerName = ui.treeWidget->currentItem()->text(0);
				//if (layerName.isEmpty())
				//{
				//	return;
				//}
				//Layer* layer =0;
				//layer = pMap->GetLayer(layerName.toStdString());
				//if (layer == NULL)
				//{
				//	return;
				//}
				////m_pMW->Slot_ShowRaderAttributeInfo(activateW,layer);
				//pMap->SetVisibleLayer(layerName.toStdString(),true);
			}
			else if (action->text() == tr("隐藏图层"))
			{
				//QString layerName = 0;
				//layerName = ui.treeWidget->currentItem()->text(0);
				//if (layerName.isEmpty())
				//{
				//	return;
				//}
				//Layer* layer =0;
				//layer = pMap->GetLayer(layerName.toStdString());
				//if (layer == NULL)
				//{
				//	return;
				//}
				////m_pMW->Slot_ShowRaderAttributeInfo(activateW,layer);
				//pMap->SetVisibleLayer(layerName.toStdString(),false);
			}
	}
	catch (...)
	{

	}
}

void LeftDockWidget::Slot_itemDoubleClicked(const QModelIndex& index)
{
	if(m_pTreeViewEventInterface)
		m_pTreeViewEventInterface->ItemDoubleClicked(index);
//	try
//	{
//		QString winType = "";
//		
//		ViewerQT* pViewer = GetCurentViewerQT();//dynamic_cast<ViewerQT*>(m_pMW->FromWidgetToQGLW());
//		//if (pViewer == NULL)
//		//{
//		//	return;
//		//}
//		//winType = pViewer->property("Win2_3").toString();
//		//if ("2D" ==winType)
//		//{
//		//	//二维双击无效
//		//	return;
//		//}
//		
//		Map* pMap = GetCurentMap();
//		//Map* pMap = ;(Map*)pViewer->property("Map").toLongLong();
//		/*if (pMap == NULL)
//		{
//		return;
//		}*/
//
//		
//		//int wdt = pViewer->property("DType").toInt();
//		
//
//
//		if(m_pProuductLayerTreeItem == item->parent())
//		{
//			QString layerName = 0;
//			layerName = item->text(0);
//			if (layerName.isEmpty() || layerName.isNull())
//			{
//				return;
//			}
//			Layer* pLayer =0;
//			pLayer = pMap->GetLayer(layerName.toStdString());
//			if (pLayer == NULL)
//			{
//				return;
//			}
//#if 0
//				//Layer* pLayer = m_pMap->GetLayer(strName.toStdString());
//				Layer::P_LAYER_ATTRIBUTE_MAP pLayerAttributeMap = pLayer->GetLayerAttribteTable();
//				//////////////////////////////////////////////////////////////////////////
//				P_FIELD_MAP pFiledMap = pLayer->GetLayerAttributeMap();
//				FIELD_MAP::iterator be = pFiledMap->begin();
//				for(; be != pFiledMap->end(); be++)
//				{
//					QString strName = QString::fromStdString(be->second.name());
//					if(strName == QString::fromLocal8Bit("中心："))
//					{
//						Layer::LAYER_ATTRIBUTE_MAP::iterator findIt = pLayerAttributeMap->find(be->first);
//
//						if(findIt != pLayerAttributeMap->end())
//						{
//							if (findIt->second.type() == typeid(osg::Vec3))
//							{
//								osg::Vec3 v3Central = boost::any_cast<osg::Vec3>(findIt->second);
//								QMdiSubWindow* pMSB = SysMainWindow::GetMainWindow()->GetCentralMdiArea()->activeSubWindow();
//								ViewManager* pVM = 0;
//								int iIndex = -1;
//								if(pMSB)
//								{
//									iIndex = pMSB->widget()->property("windowID").toInt();
//									pVM =  SysMainWindow::GetMainWindow()->GetViewManger();
//								}
//								osgViewer::Viewer* pV = dynamic_cast<osgViewer::Viewer*>(pVM->GetGLWidgetFromIndex(iIndex));
//								//osgEarth::Util::EarthManipulator* pEM = new osgEarth::Util::EarthManipulator;
//								//pV->setCameraManipulator(pEM);
//								//pEM->setViewpoint( osgEarth::Util::Viewpoint(v3Central.y(), v3Central.x(), 0, 45, 0, 100000.0), 5.0 );
//						
//							}
//						}
//					}
//				}
//				//////////////////////////////////////////////////////////////////////////
//			}
//#else
//			boost::any AttriValue;
//			if(!(pLayer->GetLayerAtrributeFromName(QString::fromLocal8Bit("中心：").toStdString(), AttriValue))) return;
//			osg::Vec3 v3Central = boost::any_cast<osg::Vec3>(AttriValue);
//			
//			//OsgEarthBridge* pSEB = extMap->GetOsgEarthBridge();//->GetOsgEarthBridge();
//			pMap->Site(v3Central);
//		}
//#endif
//	}
//	catch(...)
//	{
//
//	}
}

void LeftDockWidget::ClearTreeWidgetItem()
{
	if (m_pBasisLayerTreeItem)
	{
		int count = m_pBasisLayerTreeItem->childCount();
		for (int i = 0 ; i<count ;i++)
		{
			QTreeWidgetItem* rItem = m_pBasisLayerTreeItem->child(0);
			m_pBasisLayerTreeItem->removeChild(rItem);
			if (rItem)
			{
				delete rItem;
				rItem= NULL;
			}
		}
	}
	if (m_pProuductLayerTreeItem)
	{
		int count = m_pProuductLayerTreeItem->childCount();
		for (int i = 0 ; i<count ;i++)
		{
			QTreeWidgetItem* rItem = m_pProuductLayerTreeItem->child(0);
			m_pProuductLayerTreeItem->removeChild(rItem);
			if (rItem)
			{
				delete rItem;
				rItem= NULL;
			}
		}
	}
}

void LeftDockWidget::AddTreeWidgetItem( QWidget* viewerWidget )
{
	try 
	{
		QWidget* activateW = GetCurentViewerQT();
		
		Map* pMap = GetCurentMap();

		/*int wdt = 0;
		if (activateW)
		{
			wdt = activateW->property("DType").toInt();
		}
		QWidget* pViewer = 0;
		if (wdt == Win4)
		{
			pViewer = m_pMW->GetCentralMdiArea()->CurrentWidget();
		}
		else
		{
			pViewer = dynamic_cast<ViewerQT*>(activateW);
		}
		if (pViewer == NULL)
		{
			return;
		}*/

		//Map* pMap = (Map*)pViewer->property("Map").toLongLong();
		/*if (pMap == NULL)
		{
			return;
		}*/

		ClearTreeWidgetItem();
	
		std::map<std::string, Layer*> mapLayers = pMap->GetMapLayers();
		std::map<std::string, Layer*>::iterator layerIter = mapLayers.begin();
		QStringList blist = pMap->BasisLayerList();
	
		while(layerIter != mapLayers.end())
		{
			QString name = QString::fromStdString(layerIter->first);
			if (blist.contains(name))
			{
				QTreeWidgetItem* item = new QTreeWidgetItem(m_pBasisLayerTreeItem);
				if (layerIter->second->Visible())
				{
					item->setCheckState(0,Qt::Checked);
				}
				else
				{
					item->setCheckState(0,Qt::Unchecked);
				}
				const char* name = layerIter->first.c_str();
				item->setText(0,QString::fromStdString(name));
				m_pBasisLayerTreeItem->addChild(item);
			}
			else
			{
				QTreeWidgetItem* item = new QTreeWidgetItem(m_pProuductLayerTreeItem);
				if (layerIter->second->Visible())
				{
					item->setCheckState(0,Qt::Checked);
				}
				else
				{
					item->setCheckState(0,Qt::Unchecked);
				}
				const char* name = layerIter->first.c_str();
				item->setText(0,QString::fromStdString(name));
				m_pProuductLayerTreeItem->addChild(item);
			}
			layerIter++;
		}
	}
	catch (...)
	{

	}
}


void LeftDockWidget::Slot_MoveUp()
{
	goto_gis::Map* pMap = GetCurentMap();
	if(pMap)
		pMap->MoveUpByZ(m_strLayerName.toStdString());
}

void LeftDockWidget::Slot_MoveDown()
{
	goto_gis::Map* pMap = GetCurentMap();
	if(pMap)
		pMap->MoveDownByZ(m_strLayerName.toStdString());
}


void LeftDockWidget::setVisible( bool visible )
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
	QDockWidget::setVisible(visible);
}

void LeftDockWidget::Slot_AddBaseMap()
{
	/*QWidget* activateW = GetCurentViewerQT();
	int wdt = 0;
	if (activateW)
	{
		wdt = activateW->property("DType").toInt();
	}*/
	ViewerQT* pViewer = GetCurentViewerQT();
	/*if (wdt == Win4)
	{
	pViewer = dynamic_cast<ViewerQT*>(m_pMW->GetCentralMdiArea()->CurrentWidget());
	}
	else
	{
	pViewer = dynamic_cast<ViewerQT*>(activateW);
	}
	if (pViewer == NULL)
	{
	return;
	}*/

	Map* pMap = GetCurentMap();
	if (pMap == NULL)
	{
		return;
	}
	QString fileName = QFileDialog::getOpenFileName(this,tr("打开地图文件"), "/", tr("矢量文件 (*.shp);;TIFF文件 (*.tif)"));//
	if(fileName == "")
	{
		return;
	}
	/*GISMapManager* gisMapManager = m_pMW->GetViewManger()->GetGISMapManager();
	if (gisMapManager)
	{
		gisMapManager->AddLayer(pMap,fileName);
		pMap->BasisLayerList().append(fileName);
		QTreeWidgetItem* item = new QTreeWidgetItem(m_pBasisLayerTreeItem);
		item->setCheckState(0,Qt::Checked);
		item->setText(0,fileName);
		m_pBasisLayerTreeItem->addChild(item);
	}*/
}

ViewerQT* LeftDockWidget::GetCurentViewerQT()
{
	QString winType = "";
	MainFrameWindow* pMFW = MainFrameWindow::GetUIMainFramInstance();
	WindowFrameContext* pWFC = pMFW->GetWindowFrameContext();
	ViewerQT* pViewer = pWFC->GetCurrentQGLWidget();
	return pViewer;
}

goto_gis::Map* LeftDockWidget::GetCurentMap()
{
	MainControlFrame* pMCF = MainControlFrame::GetMainControlFrameInstance();
	ControllerFrameContext* pCFC = pMCF->GetCurrentContext();
	Map* pMap = pCFC->GetCurrentMap();
	return pMap;
}

void LeftDockWidget::AddContextMenuAction( MyActionInterface* pAction)
{
	m_pRightMenu->addAction(pAction);
	m_pActionGroup->addAction(pAction);
}

void LeftDockWidget::AddToolsBarAction( MyActionInterface* pAction)
{
	ui.toolBar->addAction(pAction);
	m_pActionGroup->addAction(pAction);
}

void LeftDockWidget::Slot_ActionTriggered_dock( QAction* action )
{
	MyActionInterface* pMAI = dynamic_cast<MyActionInterface*>(action);
	if(pMAI != 0)
	{
		pMAI->MyAction();
		m_pDockWidgetInterface->Update();
	}
}

void LeftDockWidget::SetModel( QAbstractItemModel * model )
{
	ui.treeWidget->setModel(model);
}

