#include <QEvent>
#include <QGLWidget>
#include <QThread>

#include <osg/ClipNode>
#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>

#include "IDPApplication.h"
#include "DataProvider.h"
#include "IsoSurface.h"

#include "../GDataTypes/cDrawProduct.h"
#include "InterpolationRadarDataInterface.h"
#include "ClipBoard.h"

#include "ViewManager.h"
#include "DrawRect.h"

#include "DrawLrmImage.h"
#include "radarsingledataprovider.h"
#include "InterpolationRadarData.h"
#include "VolumeImage.h"

#include "ManualDrawLine.h"
#include "AdapterWidget.h"
#include "CompositeViewerQT.h"
#include "mainwindow.h"
#include "sysmainwindow.h"

#include "DrawGeometryActions.h"
//#include "LeftDockWidget.h"
#include "LegendWidget.h"

#include "AreaDialog.h"
#include "LrmDlg.h"
#include "statustoolbar.h"
#include "AnimaticWidow.h"
#include "CipasComposer.h"
#include "RadarGraphicsView.h"
#include "RadarDataAccess.h"
#include "RadarToolBar.h"

class RadarRectHandler : public RectResult
{
public:
	QVector<QImage> ImageVector;
	RadarRectHandler() : m_pDataProvier(0){};

	osg::Geometry* GetParallelogram(const osg::Vec3& pos, osg::Image* pImage)
	{
		osg::Vec3 p1, p2, p3, p4;
		float fHeight = 40.f;
		float fAdd = 20.f;
		/*
		p4_________________p3
		|							|
		|							|
		|							|
		p1__________________p2
		*/
#if 0
		float fEdge = fHeight * cos(3.1415926/180 * 45);
		p1 = pos;

		p2.set(p1.x() + fEdge + fAdd, p1.y(), p1.z());
		p3.set(p2.x() + fEdge + fAdd, p2.y(), p2.z() + fEdge);
		p4.set(p1.x() + fEdge, p1.y(), p1.z() + fEdge);
#else
		p1 = pos;
		p2.set(p1.x() + fHeight, p1.y(), p1.z());
		p3.set(p2.x(), p2.y(), p2.z() + fHeight);
		p4.set(p1.x(), p1.y(), p1.z() + fHeight);
#endif
		osg::Vec3Array* vertexs = new osg::Vec3Array;
		osg::Vec4Array* colors = new osg::Vec4Array;
		osg::Vec2Array* textureCoords = new osg::Vec2Array;

		vertexs->push_back(p1);
		vertexs->push_back(p2);
		vertexs->push_back(p3);
		vertexs->push_back(p4);

		colors->push_back(osg::Vec4(1, 1, 1, 1));

		textureCoords->push_back(osg::Vec2(0, 0));
		textureCoords->push_back(osg::Vec2(0, 1));
		textureCoords->push_back(osg::Vec2(1, 1));
		textureCoords->push_back(osg::Vec2(1, 0));

		osg::Geometry* pGeometry = new osg::Geometry;
		pGeometry->setVertexArray(vertexs);
		pGeometry->setColorArray(colors);
		pGeometry->setColorBinding(Geometry::BIND_OVERALL);
		pGeometry->setTexCoordArray(0, textureCoords);

		pGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

		osg::StateSet* pSS = pGeometry->getOrCreateStateSet();
		osg::Texture2D* t2d = new osg::Texture2D(pImage);
		pSS->setTextureAttributeAndModes(0, t2d, osg::StateAttribute::ON);

		return pGeometry;
		
	};

	osg::Node* MakeLayout(osg::Image** pImage, int iC)
	{
		float iJG = 60.0f;

		osg::Group* pGroup = new osg::Group;
		osg::Geode* pGeode = new osg::Geode; 
		pGroup->addChild(pGeode);
		
		for(int i = 0; i < iC; i++)
		{
			osg::Image* pI = *(pImage+i);
			osg::Vec3 pos;
#if 0
			if(i % 2 == 0)
			{
				pos.set(iJG, 0, i * iJG);
			}
			else
			{
				pos.set(0, 0, i * iJG);
			}
#else
			pos.set(0, 0, i * iJG);
#endif
			pGeode->addDrawable(GetParallelogram(pos, pI));
		}
		/*pGeode->addDrawable(GetParallelogram(osg::Vec3(0, 0, 0), pImage));
		pGeode->addDrawable(GetParallelogram(osg::Vec3(60, 0, 0), pImage));
		pGeode->addDrawable(GetParallelogram(osg::Vec3(0, 0, -60), pImage));
		pGeode->addDrawable(GetParallelogram(osg::Vec3(60, 0, -60), pImage));*/

		return pGroup;
	};

	virtual void FinishDraw( const osg::Vec3Array* pVec3Array )
	{
		LrmDlg* dlg = new LrmDlg(0);
		
		QStringList strList;
		dlg->SetLayHigh(&strList);
		int iR = dlg->exec();
		if(iR == QDialog::Rejected) return;
		if(!m_pDataProvier)
		{

		}

		MainWindow* mw = SysMainWindow::GetMainWindow();
		
		ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>( mw->FromWidgetToQGLW());

		DrawLrmImage* pDrawLrmImage = (DrawLrmImage*)(m_pDataProvier->GetExtendInterface(2));

		if(!viewerWindow)
		{

		}
		osg::Vec3 p1, p2, pp1, pp2;

		/*goto_gis::Proj4Transform proj4;
		proj4.setSourceCRS(viewerWindow->Map2D()->GetCoordinateTransform()->getDesCRS());
		proj4.setDestCRS(viewerWindow->Map2D()->GetCoordinateTransform()->getSourceCRS());*/
		//pp1 = proj4.Transform(pVec3Array->at(2)/*, pp1*/);
		//pp2 = proj4.Transform(pVec3Array->at(3)/*, pp2*/);

		pp1 = pVec3Array->at(2)/1000.0;
		pp2 = pVec3Array->at(3)/1000.0;

		if(pDrawLrmImage)
		{
			p1 = pVec3Array->at(0);
			p2 = pVec3Array->at(1);

			pDrawLrmImage->SetLayerList(strList);
			pDrawLrmImage->SetPoints(p1.x(), p1.y(), p2.x(), p2.y(), pp1.x(), pp1.y(), pp2.x(), pp2.y());

			pDrawLrmImage->GetImages(&ImageVector);

			QMainWindow* pMainWindow = new QMainWindow;
			
			ViewerQT* pViewer = new ViewerQT(0);
			int iSize = ImageVector.size();

			if(!iSize) return;
			osg::Image* (*pImageArray) = new osg::Image*[iSize];

			for(int i =0; i < iSize; i++)
			{
				//ImageVector.at(0).save("Image" + QString("%1").arg(i) + ".bmp");

				QImage img = QGLWidget::convertToGLFormat(ImageVector.at(0)); 

				unsigned char* data = img.bits(); 
				osg::Image* sym = new osg::Image(); 
				sym->setImage(img.width(), 
					img.height(), 
					1, 
					4, 
					GL_RGBA, 
					GL_UNSIGNED_BYTE, 
					data, 
					osg::Image::USE_NEW_DELETE, 
					1); 

				*(pImageArray + i) = sym;
				//osg::Image* pI = new osg::Image;

			}
			/*osg::Image* ImageArray[5] = {0};
			osg::Image* pImage = osgDB::readImageFile("Image0.bmp");
			ImageArray[0] = pImage;
			ImageArray[1] = pImage;
			ImageArray[2] = pImage;
			ImageArray[3] = pImage;
			ImageArray[4] = pImage;*/
			
			osg::Node* pNode = MakeLayout(pImageArray, iSize);
			pViewer->getViewer()->setSceneData(pNode);
			pViewer->getViewer()->setCameraManipulator(new osgGA::TrackballManipulator);
			pMainWindow->setCentralWidget(pViewer);
			pMainWindow->setGeometry(100, 200, 800, 600);
			pMainWindow->show();
		}
	}


	DataProvider* m_pDataProvier;

};

//class VolumeImage
//{
//public:
//	virtual void GetImages(QVector<QImage>* imageList) = 0;
//};

#include <osgDB/ReadFile>


RadarToolBar::RadarToolBar(QWidget *parent): QToolBar(parent),m_DefaultActionGroup(0)
{
	ui.setupUi(this);
	m_pAreaDialog = NULL;
	m_pPlotDialog = NULL;
	m_pDB2Dialog = NULL;

	m_pMW = SysMainWindow::GetMainWindow();
	m_pRadarDataAccess = RadarDataAccess::Instance(m_pMW->GetViewManger(),m_pMW);

	//foreach(QAction* action,this->actions())
	{
		m_DefaultActionGroup.addAction(ui.action2D);
		m_DefaultActionGroup.addAction(ui.action3D);
		m_DefaultActionGroup.addAction(ui.action_VTK);
		m_DefaultActionGroup.addAction(ui.actionAnimatic);
		m_DefaultActionGroup.addAction(ui.actionArea);
		m_DefaultActionGroup.addAction(ui.actionClipPlane);
		m_DefaultActionGroup.addAction(ui.actionDB2);
		m_DefaultActionGroup.addAction(ui.actionFuzzy);
		m_DefaultActionGroup.addAction(ui.actionPlot);
		m_DefaultActionGroup.addAction(ui.actionVolumeRender);
		m_DefaultActionGroup.addAction(ui.actionWin4);


		
		//////////////////////////////////////////////////////////////////////////
		m_pMW->GetSysGroup().push_back(ui.action2D);
		m_pMW->GetSysGroup().push_back(ui.action3D);
		m_pMW->GetSysGroup().push_back(ui.action_VTK);
		m_pMW->GetSysGroup().push_back(ui.actionAnimatic);
		m_pMW->GetSysGroup().push_back(ui.actionArea);
		m_pMW->GetSysGroup().push_back(ui.actionClipPlane);
		m_pMW->GetSysGroup().push_back(ui.actionDB2);
		m_pMW->GetSysGroup().push_back(ui.actionFuzzy);
		m_pMW->GetSysGroup().push_back(ui.actionPlot);
		m_pMW->GetSysGroup().push_back(ui.actionVolumeRender);
		m_pMW->GetSysGroup().push_back(ui.actionWin4);
		//////////////////////////////////////////////////////////////////////////
	}
	InitSlots();

}

RadarToolBar::~RadarToolBar()
{

}

void RadarToolBar::InitSlots()
{
	connect(&m_DefaultActionGroup, SIGNAL(triggered(QAction*)),this,  SLOT(Slots_ActionTriggered(QAction*)));
}

void RadarToolBar::SetEnabledList(QStringList actionList,bool endabled)
{
	/*foreach(QAction* action , this->actions())
	{
	if (actionList.contains(action->text()))
	{
	action->setEnabled(endabled);
	}
	}*/
}

/** 
  * @brief    初始化雷达工具栏   
  * @author   易家祥  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   RadarToolBar  初始化雷达工具栏
  * @note     易家祥
  * @date     2013年3月                               
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void RadarToolBar::Slots_ActionTriggered( QAction* action )
{
	if (action == ui.actionAnimatic)
	{
		AnimaticWidow* aw = new AnimaticWidow(0,this);
		aw->setWindowModality(Qt::ApplicationModal);
		aw->showMaximized();
		return;
	}
	ViewerQT*  viewerWindow = dynamic_cast<ViewerQT*>(m_pMW->FromWidgetToQGLW());

	if (viewerWindow == NULL)
	{
		QMessageBox::information(this,"提示","没有激活视口！");
		return;
	}
	try 
	{
			int wdt = viewerWindow->property("DType").toInt();
			if (wdt != RadarData)
			{
				QMessageBox::information(this,"提示","获取视口类型失败！");
				return;
			}
			
			goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();
			if (pMap == NULL)
			{
				QMessageBox::information(this,"提示","获取视口Map失败！");
				return;
			}
			QString winType = viewerWindow->property("Win2_3").toString();

			//////////////////////////////////////////////////////////////////////////
			if (action== ui.actionWin4)//四联屏
			{
				if ("3D" ==winType)
				{
					QMessageBox::information(this,tr("提示"),tr("三维状态下不支持该功能！"));
					return;
				}
				QApplication::setOverrideCursor(Qt::WaitCursor);
				QWidget* win4 = m_pRadarDataAccess->Win4Viewer(pMap);
				if (win4)
				{
					connect(win4, SIGNAL(SetCurrentWidget(QWidget*)),this,  SLOT(Slot_ActivateWindow(QWidget*)));
					m_pMW->CreateMDISubWindow(win4);
					win4->showMaximized();
				}
				QApplication::restoreOverrideCursor();
			}
			
			else if (action == ui.actionDB2)
			{
				if ("2D" ==winType)
				{
					QMessageBox::information(this,tr("提示"),tr("二维状态下无法绘制等值面！"));
					return;
				}
				QApplication::setOverrideCursor(Qt::WaitCursor);
				m_pRadarDataAccess->DB2Widget(pMap);
				QApplication::restoreOverrideCursor();
			}
			else if (action == ui.actionPlot)//剖面 OK
			{
					m_pRadarDataAccess->PlotWidget(viewerWindow);
			}
				else if (action == ui.actionArea)//面积统计 OK
				{
					m_pRadarDataAccess->AreaWidget(pMap);
					AreaDialog* pAreaDialog = new AreaDialog(pMap, m_pMW);
					pAreaDialog->show();
				}

				else if (action == ui.action2D)
				{
					Map* pMap = (Map*)viewerWindow->property("Map").toLongLong();;
					pMap->Switch2DMap();
					//m_pRadarDataAccess->Switch2DMap(viewerWindow);

					viewerWindow->setProperty("Win2_3","2D");
					ui.action2D->setEnabled(false);
					ui.action3D->setEnabled(true);
					ui.action_VTK->setEnabled(true);
					ui.actionClipPlane->setEnabled(false);
					ui.actionVolumeRender->setEnabled(false);
					m_pMW->RadarControlDockWidgetGroupEnd(false);
					m_pMW->GetCentralMdiArea()->Slot_SubWindowActivated(dynamic_cast<QMdiSubWindow*>(viewerWindow->parentWidget()));
				}
				else if (action == ui.action3D)
				{
					Map* pMap = (Map*)viewerWindow->property("Map").toLongLong();;
					pMap->Switch3DMap();
					//m_pRadarDataAccess->Switch3DMap(viewerWindow);

					ui.action3D->setEnabled(false);
					ui.action2D->setEnabled(true);
					ui.actionClipPlane->setEnabled(true);
					ui.actionVolumeRender->setEnabled(true);
					viewerWindow->setProperty("Win2_3","3D");
					m_pMW->RadarControlDockWidgetGroupEnd(true);
					m_pMW->GetCentralMdiArea()->Slot_SubWindowActivated(dynamic_cast<QMdiSubWindow*>(viewerWindow->parentWidget()));
				}
				else if (action == ui.actionVolumeRender)
				{
					m_pRadarDataAccess->VolumeRender(pMap);
				}
#if 0
				else if (action == ui.action_VTK)
				{
					ViewerQT* view = dynamic_cast<ViewerQT*>(SysMainWindow::GetMainWindow()->FromWidgetToQGLW());//->GetViewManger();
					if (!view)
					{
						return;
					}
					goto_gis::Layer* pRadarLayer = pMap->GetLayer("第1仰角层");
					if(!pRadarLayer) return;
					goto_gis::DataProvider* pDP = pRadarLayer->GetDataProvider();
					if(!pDP) return;
					std::string filename = pDP->GetFileName();
					RadarOnVTK * vtkView = dynamic_cast<RadarOnVTK *>( SysMainWindow::GetMainWindow()->GetViewManger()->CreateViewport(5,0));
					SysMainWindow::GetMainWindow()->CreateMDISubWindow(vtkView);
					vtkView->showMaximized();

					ui.action3D->setEnabled(true);
					ui.action2D->setEnabled(true);
					vtkView->OnFileOpen(filename);
					SysMainWindow::GetMainWindow()->GetCentralMdiArea()->Slot_SubWindowActivated(dynamic_cast <QMdiSubWindow *>(vtkView->parentWidget()));
				}

#endif
				
				else if (action == ui.actionFuzzy)
				{
					//m_pRadarDataAccess->Fuzzy(pMap);
					DrawRectAction* pRectAction = new DrawRectAction(0);
					pRectAction->MyAction();
					RadarRectHandler* pRadarHandler = new RadarRectHandler;
					goto_gis::Layer* pLayer = pMap->GetLayer("第1仰角层");
					pRadarHandler->m_pDataProvier = pLayer->GetDataProvider();
					pRectAction->GetDrawRect()->SetResultHandle(pRadarHandler);

					delete pRectAction;
				}
				else if(action == ui.actionClipPlane)
				{
					m_pRadarDataAccess->ClipPlane(pMap);
				}
		}
		catch (...)
		{

		}
}

void RadarToolBar::Slot_ActivateWindow( QWidget* widget )
{
	SysMainWindow::GetMainWindow()->GetCentralMdiArea()->CurrentWidget(widget);
	SysMainWindow::GetMainWindow()->AddLeftDockW(widget);
}

void RadarToolBar::LoadData( QString filename )
{
		QApplication::setOverrideCursor(Qt::WaitCursor);

		//LegendWidget* tabW = new LegendWidget(RadarData,m_pMW->centralWidget());
		ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(m_pRadarDataAccess->ReadRadarData(filename,RadarData,m_pMW->centralWidget()));
		if (!viewerWindow)
		{
			QMessageBox::information(this,"提示","创建视口失败！");
			return;
		}
		goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();
		if (pMap == 0)
		{
			QMessageBox::information(this,"提示","Map为空！");
			return;
		}
		Layer* layer = pMap->GetLayer("第1仰角层");
		if (layer == 0)
		{
			QMessageBox::information(this,"提示","第1仰角层为空！");
			return;
		}
		m_pMW->GetViewManger()->ShowRaderAttributeInfo(layer,viewerWindow);

		//CCipasComposer* pCipasComposer = new CCipasComposer(this);
		//pCipasComposer->setOsgView(viewerWindow);
		//pCipasComposer->setWindowFlags(Qt::Widget);
		////pCipasComposer->setWindowModality(Qt::WindowModal);
		//pCipasComposer->setExtent(0,-90,180,90);
		////pCipasComposer->setExtent(x_min,y_mim,x_max,y_max);

		//pCipasComposer->statusBar()->showMessage(tr("就绪"));
		//QString templateString =IDPApplication::TemplatePath()+ "/ThemeTemplate/ServerGrid.cpt";
		//pCipasComposer->LoadTemplatefromfile(templateString);
		//pCipasComposer->open();
		//pCipasComposer->menuBar()->hide();//菜单栏
		//pCipasComposer->statusBar()->hide();//状态栏

		//pCipasComposer->toolBar->setIconSize(QSize(20,20));
		//pCipasComposer->toolBarGen->setIconSize(QSize(20,20));
		////pCipasComposer->toolBarMap->setIconSize(QSize(20,20));

		//pCipasComposer->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
		//pCipasComposer->toolBarGen->setToolButtonStyle(Qt::ToolButtonIconOnly);
		////pCipasComposer->toolBarMap->setToolButtonStyle(Qt::ToolButtonIconOnly);

		int maxValue = 0;
		std::map<std::string, Layer*> mapLayers = pMap->GetMapLayers();
		std::map<std::string, Layer*>::iterator layerIter = mapLayers.begin();
		while(layerIter != mapLayers.end())
		{
			QString name = QString::fromStdString(layerIter->first);
			if (name.contains("layer") && !name.contains("距离圈"))
			{
				maxValue++;
			}
			layerIter++;
		}
		//m_pMW->GetStatusToolBar()->GetSpinBox()->setRange(0,maxValue);

		//Map* extMap = dynamic_cast<Map*>(pMap);
		m_pRadarDataAccess->SetRadarCenter(pMap, (IsoSurface*)pMap->GetExtendInterface(std::string("IsoSurface")));

		m_pRadarDataAccess->InitMachCubeData(pMap, (IsoSurface*)pMap->GetExtendInterface(std::string("IsoSurface")));

		//tabW->addTab(viewerWindow,QIcon(":/images/logo.png"),tr("数据"));
		//tabW->addTab(pCipasComposer,QIcon(":/images/logo.png"),tr("专题"));

		m_pMW->CreateMDISubWindow(viewerWindow);
		viewerWindow->showMaximized();

		//////////////////////////////////////////////////////////////////////////
	
		RadarGraphicsView* gview = new RadarGraphicsView(viewerWindow);
		gview->setWindowOpacity(0.45);

		gview->setAttribute(Qt::WA_NativeWindow, true);
		gview->setAutoFillBackground(true);
		gview->setStyleSheet(tr("border:-1px;"));
		gview->setObjectName("RadarGraphicsView");
		gview->centerOn(0,0);

		QGridLayout* gLayout = dynamic_cast<QGridLayout*>(viewerWindow->layout());
		gLayout->addWidget(gview,0,1,1,1,Qt::AlignRight);
		QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
		gview->setSizePolicy(sizePolicy);
		gview->setMinimumWidth(280);

		m_pMW->Slot_ShowRaderAttributeInfo(viewerWindow,layer);
		//////////////////////////////////////////////////////////////////////////
		QApplication::restoreOverrideCursor();
}

void RadarToolBar::ActionTrigger( QAction* action )
{
	foreach(QAction* ac , m_DefaultActionGroup.actions())
	{
		if (action->objectName() == ac->objectName())
		{
			ac->trigger();
		}
	}
}
