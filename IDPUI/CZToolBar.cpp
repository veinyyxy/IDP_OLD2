#include <QtCore/QMap>
#include <QtCore/QThread>
#include <QtOpenGL/QGLWidget>
#include <osgUtil/Tessellator>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>

#include "DefaultManipulator.h"
#include "ManualDrawRect.h"
#include "Proj4Transform.h"
#include "goQuadrilateral.h"
#include "VectorLayer.h"

#include "ViewManager.h"
#include "AdapterWidget.h"

#include "EarthquakeFileDisplay.h"
#include "GISMapManager.h"

#include "IDPApplication.h"
#include "CipasComposer.h"

#include "mainwindow.h"
#include "sysmainwindow.h"
#include "CentralMdiArea.h"

#include "CZToolBar.h"

CZToolBar::CZToolBar(QWidget *parent)
	: QToolBar(parent),m_CZActionGroup(0)
{
	ui.setupUi(this);
	_mw = SysMainWindow::GetMainWindow();
	foreach(QAction* action,this->actions())
	{
		m_CZActionGroup.addAction(action);
	}
	InitSlots();
}

void CZToolBar::InitSlots()
{
	connect(&m_CZActionGroup, SIGNAL(triggered(QAction*)),this,  SLOT(Slots_ActionTriggered(QAction*)));
}

CZToolBar::~CZToolBar()
{
		
}

void CZToolBar::Slots_ActionTriggered( QAction* action )
{
	double dr = 0;
	try 
	{
		ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(_mw->FromWidgetToQGLW());
		if (viewerWindow == NULL)
		{
			return;
		}

		if (action == ui.actionDIY)
		{
	#pragma region  自选地图区域
			float fZ = 10;
				DefaultManipulator* dm = dynamic_cast<DefaultManipulator*>(viewerWindow->getViewer()->getCameraManipulator());
				if(!dm)
				{
					osgGA::KeySwitchMatrixManipulator* pKeyMapnipulator = dynamic_cast<osgGA::KeySwitchMatrixManipulator*>(viewerWindow->getViewer()->getCameraManipulator());
					osgGA::CameraManipulator* pCamMaintor = pKeyMapnipulator->getMatrixManipulatorWithKey('1');
					dm = dynamic_cast<DefaultManipulator*>(pCamMaintor);
				}

				frameRect rect = dm->GetManualDrawRect()->GetFrameRect();
				dm->GetManualDrawRect()->RemoveRect();
				dm->GetManualDrawRect()->AddBlankRect();
				osg::Vec3Array* vec3arr = dm->GetManualDrawRect()->GetWorldPoint();
				if(vec3arr->size() <=0)
				{
					QMessageBox::information(this,tr("提示"),tr("请按住<Ctrl>键,选择区域！"));
					return;
				}
				goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();;
				Map* map = pMap;

				Proj4Transform* pProj4 = new Proj4Transform();
				pProj4->setSourceCRS(map->GetCoordinateTransform()->getDesCRS());
				pProj4->setDestCRS(map->GetCoordinateTransform()->getSourceCRS());
				pProj4->setDesProjName(map->GetCoordinateTransform()->getSourceProjName());
				double x1, y1, x2, y2;

				osg::Vec3 dst1,dst2;
				dst1 = pProj4->Transform(osg::Vec3(vec3arr->at(0).x(),vec3arr->at(0).y(),0.5));
				x1 = dst1.x();
				y1 = dst1.y();

				dst2 = pProj4->Transform(osg::Vec3(vec3arr->at(2).x(),vec3arr->at(2).y(),0.5));
				x2 = dst2.x();
				y2 = dst2.y();

				if (x1 > x2)
				{
					float tx = x1;
					x1 = x2;
					x2 = tx;
				}
				if (y1 < y2)
				{
					float ty = y1;
					y1 = y2;
					y2 = ty;
				}

				//眼膜边框
				double latframestart = -10;
				double latframeend =  70;
				double lonframestart = 50;
				double lonframeend = 170;
				double foots = abs(x1-x2)/10;

				Layer * newlayer = new VectorLayer();
				newlayer->LayerZ(1);
				newlayer->CreateLayer("eyelayer");
				newlayer->SetParentMap(map);
				newlayer->CoordTrans(map->GetCoordinateTransform());
				Layer::MgoGeometry* pMG = newlayer->GetMgoGeometry();

				//眼膜四边形1
				goQuadrilateral *poly1 = new goQuadrilateral;
				osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
				osg::Vec3Array *nrms = new osg::Vec3Array;
				osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组
				poly1->setGlenum(osg::PrimitiveSet::POLYGON);

				temp_array->push_back(osg::Vec3(50,-10,fZ));
				temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));

				for (int i = 1 ; i< 11;i++)
				{
					temp_array->push_back(osg::Vec3(50 + i*(170-50)/12,-10,fZ));
					temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));
				}
				temp_array->push_back(osg::Vec3(170,-10,fZ));
				temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));

				temp_array->push_back(osg::Vec3(170,70,fZ));
				temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));

				temp_array->push_back(osg::Vec3(50,70,fZ));
				temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));

				temp_array->push_back(osg::Vec3(x1,y2,fZ));
				temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));

				temp_array->push_back(osg::Vec3(x1,y1,fZ));
				temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));

				for (int ii = 1; ii< 10;ii++)
				{
					temp_array->push_back(osg::Vec3(x1 + ii*abs(x2-x1)/11.0,y1,fZ));
					temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));
				}
				temp_array->push_back(osg::Vec3(x2,y1,fZ));
				temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));

				temp_array->push_back(osg::Vec3(x2,y2,fZ));
				temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));

				for (int jj = 1; jj< 10;jj++)
				{
					temp_array->push_back(osg::Vec3(x2 - jj*abs(x2-x1)/11.0,y2,fZ));
					temp_color_array->push_back(osg::Vec4(255, 255, 255, 1));
				}

				poly1->SetCoordinateTransform(newlayer->CoordTrans());
				poly1->SetOsgArray(temp_array);
				poly1->SetOsgColorArray(*temp_color_array);
				poly1->SetParentLayer(newlayer);
				poly1->setTess(true);

				pMG->insert(Layer::MgoGeometryPair(pMG->size(),poly1));			
				newlayer->CreateLayerData();
				map->AddLayer("eyelayer",newlayer);
				QString layername1;
				layername1 = QString("地理网格");
				map->RemoveLayer(layername1.toStdString());
				map->AddFishNet(x1,y1,x2,y2,foots,foots,fZ+10);

				Layer* layer = map->GetLayer("地理网格");
				if (layer)
				{
					layer->CreateLayerData();
				}
				vec3arr->clear();
				_mw->AddLeftDockW(viewerWindow);
	#pragma endregion
		}
	}
	catch (...)
	{
	}
}

void CZToolBar::LoadData( QString fileName )
{
	bool bOpenedFile = false;
	if("meca" == QFileInfo(fileName).suffix() || "in" == QFileInfo(fileName).suffix())
	{
		QString strMapConfig = QCoreApplication::applicationDirPath()+tr("/Config/Default.prj");
		ViewManager* viewManger = _mw->GetViewManger();
		QWidget* pGLWidget = 0;
		QList<QMdiSubWindow *> subWinList = _mw->GetCentralMdiArea()->subWindowList();
		foreach(QMdiSubWindow* subWin , subWinList)
		{
			if (subWin->widget()->property("DType").toInt() == CZData)
			{
				bOpenedFile = true;
				pGLWidget = subWin->widget();
				break;
			}
		}
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if(bOpenedFile == false)
		{
			ViewManager* viewManger = _mw->GetViewManger();
			ViewerQT* viewerWindow =dynamic_cast<ViewerQT*>(viewManger->CreateViewport(CZData));
			goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();//Map2D();
			Map* m_pBasicMap = viewManger->GetMap2D();
			pMap->SetCoordinateTransform(m_pBasicMap->GetCoordinateTransform());
			//*pMap = *m_pBasicMap;

			EarthquakeFileDisplay* m_EarthquakeDisp = new EarthquakeFileDisplay(viewerWindow);
			viewerWindow->setUserData(0, m_EarthquakeDisp);

			m_EarthquakeDisp->OpenEarthquakeFile(strMapConfig, fileName);
			_mw->CreateMDISubWindow(viewerWindow);
			viewerWindow->showMaximized();
		}
		else
		{
			if (pGLWidget)
			{
				EarthquakeFileDisplay* pEarthquakeDisp = dynamic_cast<EarthquakeFileDisplay*>(pGLWidget->userData(0));

				if(pEarthquakeDisp)
				{
					pEarthquakeDisp->OpenEarthquakeFile(fileName);
					pGLWidget->setFocus();
				}
				_mw->AddLeftDockW(pGLWidget);
			}

		}
		QApplication::restoreOverrideCursor();

	}
	else if("tif" == QFileInfo(fileName).suffix())
	{
		QString strMapConfig = QCoreApplication::applicationDirPath()+tr("/Config/Default.prj");
		ViewManager* viewManger = SysMainWindow::GetMainWindow()->GetViewManger();
		QWidget* pGLWidget = 0;
		QList<QMdiSubWindow *> subWinList = SysMainWindow::GetMainWindow()->GetCentralMdiArea()->subWindowList();
		foreach(QMdiSubWindow* subWin , subWinList)
		{
			if (subWin->widget()->property("DType").toInt() == CZData)
			{
				bOpenedFile = true;
				pGLWidget = subWin->widget();
				break;
			}
		}
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if(bOpenedFile == false)
		{
			ViewManager* viewManger = _mw->GetViewManger();
			ViewerQT* viewerWindow =dynamic_cast<ViewerQT*>(viewManger->CreateViewport(CZData));
			Map* m_pBasicMap = viewManger->GetMap2D();
			goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();//Map2D();
			pMap->SetCoordinateTransform(m_pBasicMap->GetCoordinateTransform());
			*pMap = *m_pBasicMap;
			EarthquakeFileDisplay* m_EarthquakeDisp = new EarthquakeFileDisplay(viewerWindow);
			viewerWindow->setUserData(0, m_EarthquakeDisp);

			m_EarthquakeDisp->OpenTIFFFile(fileName,strMapConfig);
			_mw->CreateMDISubWindow(viewerWindow);
			viewerWindow->showMaximized();
		}
		else
		{
			if (pGLWidget)
			{
				EarthquakeFileDisplay* pEarthquakeDisp = dynamic_cast<EarthquakeFileDisplay*>(pGLWidget->userData(0));

				if(pEarthquakeDisp)
				{
					pEarthquakeDisp->OpenTIFFFile(fileName);
					pGLWidget->setFocus();
				}
				_mw->AddLeftDockW(pGLWidget);

			}

		}
		QApplication::restoreOverrideCursor();
	}
}
