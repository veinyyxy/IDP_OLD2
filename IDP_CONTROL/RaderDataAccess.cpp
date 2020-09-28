#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtOpenGL/QGLWidget>
#include <osgGA/TrackballManipulator>
#include <osgEarth/SpatialReference>
#include <osgEarthUtil/SkyNode>
#include "DataProvider.h"
#include "RadarVectorLayer.h"
#include "AdapterWidget.h"
#include "DefaultManipulator.h"
#include "ViewManager.h"
#include "GISMapManager.h"
#include "OsgEarthBridge.h"
#include "OsgEarthTransform.h"
#include "..\RadarSingleDataProvider\DrawLrmImage.h"
#include "ManualDrawRect.h"
#include "RaderDataAccess.h"
#include "..\GraphicsControler\GeneratedFiles\Debug\moc_AdapterWidget.cpp"
using namespace goto_gis;
RaderDataAccess::RaderDataAccess(void) : m_DrawLrmImage(0)
{
}

RaderDataAccess::RaderDataAccess( ViewManager* pVM ) : m_pViewManager(pVM)
{

}


RaderDataAccess::~RaderDataAccess(void)
{
}

QWidget* RaderDataAccess::ReadRadarData( const QString& filename, int wdt)
{
	typedef DataProvider* (*DataProviderPluginMain)();
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(m_pViewManager->CreateViewport(wdt, 0));
	Map* m_p2DBasicMap = m_pViewManager->GetMap2D();
	viewerWindow->stopRender();
	viewerWindow->Map2D()->SetCoordinateTransform(m_p2DBasicMap->GetCoordinateTransform());
	viewerWindow->CopyMap(m_p2DBasicMap);
	//pKSMapControl->Map2D() =  *m_p2DBasicMap;

	osg::ref_ptr<osg::Group> rootNode = viewerWindow->GetRootNode();
	
	LoadRadarData(filename,viewerWindow->Map2D());

	//pKSMapControl->Map2D().GetRootSwitchNode()->addChild(m_p3DBasicNode,false);

	viewerWindow->getViewer()->setSceneData(rootNode); //加载场景
	viewerWindow->getViewer()->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	viewerWindow->getViewer()->realize();
	viewerWindow->getViewer()->addEventHandler(new osgGA::StateSetManipulator(rootNode->getOrCreateStateSet()));
	viewerWindow->getViewer()->addEventHandler(new osgViewer::StatsHandler);
	viewerWindow->getViewer()->addEventHandler(new osgViewer::WindowSizeHandler);
	viewerWindow->getViewer()->addEventHandler(new osgGA::GUIEventHandler);

	DefaultManipulator* dm = new DefaultManipulator;
	dm->CoordTrans(m_p2DBasicMap->GetCoordinateTransform());
	viewerWindow->getViewer()->setCameraManipulator(dm);

	Layer* pCircleLayer = viewerWindow->Map2D()->GetLayer("layer0");

	if (pCircleLayer != 0)
	{
		osg::Node* pLayerNode = pCircleLayer->GetLayerNode();
		osg::Matrixd homeMatrix;
		osg::Vec3d eye, center, up;
		boost::any anyR;
		QString strR;
		std::string stdR;
		double dR = -1;

		if(pCircleLayer->GetLayerAtrributeFromName("有效范围：", anyR))
		{
			stdR = boost::any_cast<std::string>(anyR);
			strR = QString::fromStdString(stdR).remove("Km");
			dR = strR.toDouble()  * 1000;
		}

		dm->GoToNodeCenter(viewerWindow->getViewer()->getCamera(), pLayerNode, dR);
	}
	viewerWindow->startRender();
	return viewerWindow;
}


void RaderDataAccess::LoadRadarData( QString filename ,Map* map)
{
	typedef DataProvider* (*DataProviderPluginMain)();
	DataProvider* radarDProvider = 0;
	if (filename.contains(QString::fromLocal8Bit("MVIL"))||filename.contains(QString::fromLocal8Bit("MVILD"))||filename.contains(QString::fromLocal8Bit("BREF"))||filename.contains(QString::fromLocal8Bit("CREF"))) 
	{
		QLibrary myLib("RadarMosaicDataProvider.dll");
		DataProviderPluginMain GetRadarMosaicDataProvider = (DataProviderPluginMain) myLib.resolve("DataProviderPluginMain");
		radarDProvider = GetRadarMosaicDataProvider();
	}
	else if (filename.contains(QString::fromLocal8Bit("VWP"))||filename.contains(QString::fromLocal8Bit("VAD"))||filename.contains(QString::fromLocal8Bit("SBT"))
		||filename.contains(QString::fromLocal8Bit("EVP"))||filename.contains(QString::fromLocal8Bit("_M_"))||filename.contains(QString::fromLocal8Bit("SCI"))||filename.contains(QString::fromLocal8Bit("HI")))
	{
		//return ;

		QLibrary myLib("RadarStructDataProvider.dll");
		DataProviderPluginMain GetRadarStructDataProvider = (DataProviderPluginMain) myLib.resolve("DataProviderPluginMain");
		radarDProvider = GetRadarStructDataProvider();

	}else
	{
		QLibrary myLib("RadarSingleDataProvider.dll");
		typedef DataProvider* (*DataProviderPluginMain)();
		DataProviderPluginMain GetRadarSingleDataProvider = (DataProviderPluginMain) myLib.resolve("DataProviderPluginMain");
		radarDProvider = GetRadarSingleDataProvider();
	}
	//////////////////////////////////////////////////////////////////////////
	{
		CoordinateTransform* coord = m_pViewManager->GetMap2D()->GetCoordinateTransform();//得到投影
		RadarVectorLayer* vLayer0 = new RadarVectorLayer(radarDProvider,coord);
		vLayer0->SetParentMap(map);
		map->CurrentLayer(vLayer0);
		vLayer0->CoordTrans(coord);
		vLayer0->LayerZ(0.5f);
		radarDProvider->ParentLayer(vLayer0);
		vLayer0->CreateLayer(filename.toStdString());
		vLayer0->CreateLayerData();
		vLayer0->TransformLayer();
		QFileInfo info(filename);
		QString layerName = "layer" + QString::number(0);
		map->AddLayer(layerName.toStdString(),vLayer0);
		map->TramsformMap();

		m_DrawLrmImage = (DrawLrmImage*)(radarDProvider->GetExtendInterface(2));
	}
}

void RaderDataAccess::LoadRadarClipPlane(Map* p3DMap, OsgEarthBridge* pOEB )
{
	Layer* pNamedLayer = p3DMap->GetLayer("Layer_Circle");
	boost::any anyR;
	QString strR;
	std::string stdR;
	double dR = -1;
	osg::Vec3 v3Central;
	QRectF rectf;
	if(pNamedLayer)
	{
		if(pNamedLayer->GetLayerAtrributeFromName("有效范围：", anyR))
		{
			stdR = boost::any_cast<std::string>(anyR);
			strR = QString::fromStdString(stdR).remove("Km");
			dR = strR.toDouble()  * 1000;
		}
		if(pNamedLayer->GetLayerAtrributeFromName(QString::fromLocal8Bit("中心：").toStdString(), anyR))
		{
			v3Central = boost::any_cast<osg::Vec3>(anyR);
		}
		if(pNamedLayer->GetLayerAtrributeFromName("边界", anyR))
		{
			rectf = boost::any_cast<QRectF>(anyR);
		}
	}
	
	pOEB->SetRadarCenter(v3Central);
	osg::Vec4 osgRect(rectf.left(), rectf.top(), rectf.right(), rectf.bottom());
	pOEB->AddClipPlane(p3DMap->GetRootNode(), p3DMap->GetRootSwitch(), osgRect);
}

void RaderDataAccess::InitMachCubeData( Map* p3DMap, OsgEarthBridge* pOEB )
{
	Layer* pLayer0 = p3DMap->GetLayer("layer0");
	if (pLayer0)
	{
		DataProvider* pDataProvider = pLayer0->GetDataProvider();
		void* pRDI = pDataProvider->GetExtendInterface(1);

		RadarMarchingCubes* pRMC = (RadarMarchingCubes*)pRDI;

		pOEB->SetRadarMarchingCubes(pRMC);
	}
}

void RaderDataAccess::SetRadarCenter(Map* p3DMap, OsgEarthBridge* pOEB )
{
	Layer* pNamedLayer = p3DMap->GetLayer("Layer_Circle");
	boost::any anyR;
	QString strR;
	osg::Vec3 v3Central;
	if(pNamedLayer)
	{
		if(pNamedLayer->GetLayerAtrributeFromName(QString::fromLocal8Bit("中心：").toStdString(), anyR))
		{
			v3Central = boost::any_cast<osg::Vec3>(anyR);
		}
	}

	pOEB->SetRadarCenter(v3Central);
}

void RaderDataAccess::MakeFenCengZuHeFanSheLuRect(DefaultManipulator* pDrawRect)
{
	ManualDrawRect* pManualDrawRect  = pDrawRect->GetManualDrawRect();
	pManualDrawRect->GetScreenPoint();
	pManualDrawRect->GetWorldPoint();
}
