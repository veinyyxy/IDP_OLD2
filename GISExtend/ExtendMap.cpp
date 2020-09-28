#include <QCoreApplication>
#include <osgGA/TrackballManipulator>
#include <osgGA/GUIEventHandler>

#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <OsgEarth/ModelLayer>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>

#include <QtCore/QLineF>
#include "DefaultManipulator.h"
#include "ExtendMap.h"
#include "Proj4Transform.h"
#include "idpEarthManipulator.h"

void ScreenToWorld( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out )
{
	osg::Matrix VPW = pCamera->getViewMatrix() *
		pCamera->getProjectionMatrix() *
		pCamera->getViewport()->computeWindowMatrix();
	osg::Matrix inverseVPW;
	inverseVPW.invert(VPW);
	v3Out = v3In  * inverseVPW;

	v3Out.set(v3Out.x(),v3Out.z(),v3Out.y());
	//v3Out = m_pCoord->Transform(v3Out);
}

class ViewerEventHandle : public osgGA::GUIEventHandler
{
public:
	ViewerEventHandle(){m_Proj4 = new goto_gis::Proj4Transform;}
	virtual bool handle( const GUIEventAdapter& ea,GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor* )
	{
		osg::Vec3 currentPosition;
		ScreenToWorld(m_pCamera,osg::Vec3(ea.getX(),ea.getY(),0),currentPosition);
		currentPosition = m_Proj4->Transform(currentPosition);

		m_pExtendMap->SetCurrentLocation(currentPosition);
		return osgGA::GUIEventHandler::handle(ea,aa);
	}
	goto_gis::Proj4Transform* m_Proj4;
	ExtendMap* m_pExtendMap;
	osg::Camera* m_pCamera;
};

ExtendMap::ExtendMap(osgViewer::View* pAttchViewer) 
	:
	Map(pAttchViewer),
	//m_pKeySwitch(new osgGA::KeySwitchMatrixManipulator),
	m_b2DMap(true),
	m_2DMapManager(this)
{
	m_p2DManipulater = new DefaultManipulator;
	//m_p2DManipulater = new osgGA::OrbitManipulator;
	m_pEarthManipulator = new osgEarth::Util::EarthManipulator;

	if(pAttchViewer)
	{
		m_3DMapManager.SetViewer(pAttchViewer);
		//InitDisplayLatLong();
	}
	
	//pAttchViewer->setCameraManipulator(m_pKeySwitch);
	//m_pKeySwitch->addMatrixManipulator(0, "0", new DefaultManipulator);
	//m_pKeySwitch->addMatrixManipulator(1, "1", new osgEarth::Util::EarthManipulator);

	SetExtendInterface(std::string("IsoSurface"), (IsoSurface*)(&m_3DMapManager));
	SetExtendInterface(std::string("ClipPlaneInterface"), (ClipPlaneInterface*)(&m_3DMapManager));
}

ExtendMap::~ExtendMap(void)
{
}

void ExtendMap::Switch2DMap()
{
	if(!m_b2DMap)
	{
		m_b2DMap = true;

		osg::Matrix viewMatrix;
		//viewMatrix.makeRotate(3.1415926/180*90, 1, 0, 0);
		//this->GetRootNode()->setMatrix(viewMatrix);
		this->SetCoordinateTransform(m_pCoordinateRefer);//不需要重新设置投影
		//m_2DMapManager.CoordTans();
		this->TramsformMap(true);
		//m_pKeySwitch->selectMatrixManipulator(0);
		this->GetRootSwitch()->setChildValue(m_3DMapManager.GetOsgEarthMapNode(),false);

		m_pView->getCamera()->getProjectionMatrixAsPerspective(
			m_3dCamera.fovy, 
			m_3dCamera.aspectRatio, 
			m_3dCamera.zNear, 
			m_3dCamera.zFar);

		m_pView->getCamera()->setProjectionMatrixAsPerspective(
			m_2dCamera.fovy, 
			m_2dCamera.aspectRatio, 
			m_2dCamera.zNear, 
			m_2dCamera.zFar);

		m_pView->setCameraManipulator(m_p2DManipulater);
	}
}

void ExtendMap::Switch3DMap()
{
	if(m_b2DMap)
	{
		m_b2DMap = false;

		//osg::Matrix viewMatrix;
		//viewMatrix.identity();
		//this->GetRootNode()->setMatrix(viewMatrix);
		m_pCoordinateRefer = GetCoordinateTransform();
		this->SetCoordinateTransform(m_3DMapManager.GetCoord());
		TramsformMap(false);
		//m_pKeySwitch->selectMatrixManipulator(1);
		this->GetRootSwitch()->setChildValue(m_3DMapManager.GetOsgEarthMapNode(),true);

		m_pView->getCamera()->getProjectionMatrixAsPerspective(
			m_2dCamera.fovy, 
			m_2dCamera.aspectRatio, 
			m_2dCamera.zNear, 
			m_2dCamera.zFar);

		/*m_pView->getCamera()->setProjectionMatrixAsPerspective(
			m_3dCamera.fovy, 
			m_3dCamera.aspectRatio, 
			m_3dCamera.zNear, 
			m_3dCamera.zFar);*/

		m_pView->setCameraManipulator(m_pEarthManipulator);
	}
}

void ExtendMap::Load3DMapFromConfig( std::string str3dMap )
{
	m_3DMapManager.LoadEarthModel(str3dMap.c_str());
	m_3DMapManager.LoadSky(); //暂时去掉天空20140503by陈志学
	QString compasspng = QCoreApplication::applicationDirPath() + "/Config/Compass1.png";
	m_3DMapManager.AddCompass(compasspng);

	this->GetRootSwitch()->addChild(m_3DMapManager.GetOsgEarthMapNode(), false);
	m_pView->setCameraManipulator(m_p2DManipulater);
	//m_pKeySwitch->addMatrixManipulator(1, "1", new osgEarth::Util::EarthManipulator);
}

void ExtendMap::Load2DMapFromConfig( std::string str2dMap )
{
	m_2DMapManager.SetMap(this);
	m_2DMapManager.LoadBasicMap(QString::fromStdString(str2dMap));

	//m_pKeySwitch->addMatrixManipulator(0, "0", new osgGA::TrackballManipulator);
}

void ExtendMap::MapLocation( float fLon, float fLat )
{
	osg::Vec3 v3MapCenter(fLon, fLat, 0);

	osg::Vec3d v3Eye, v3Center, v3Up;
	m_p2DManipulater->getHomePosition(v3Eye, v3Center, v3Up);

	osg::Vec3 v3WorldPos;

	goto_gis::CoordinateTransform* pCT = GetCoordinateTransform();

	pCT->Transform(v3MapCenter, v3WorldPos);

	v3Eye.set(v3WorldPos.x(), v3Eye.y(), v3WorldPos.y());

	v3Center.set(v3WorldPos.x(), v3Center.y(), v3WorldPos.y());

	m_p2DManipulater->setHomePosition(v3Eye, v3Center, v3Up);
	m_p2DManipulater->home(0);
}
//115.97,40.47,116.67,39.92
void ExtendMap::MapScale( float fLeft, float fBottom, float fRight, float fTop )
{
	//osg::BoundingSphere boundingSphere;

	osg::Vec3 v3LeftTop(fLeft, fTop, 0);
	osg::Vec3 v3RightBottom(fRight, fTop, 0);

	osg::Vec3 v3LeftBottom(fLeft, fBottom, 0);
	osg::Vec3 v3RightTop(fRight, fTop, 0);

	goto_gis::CoordinateTransform* pCT = GetCoordinateTransform();

	pCT->Transform(v3LeftTop, v3LeftTop);
	pCT->Transform(v3RightBottom, v3RightBottom);

	pCT->Transform(v3LeftBottom, v3LeftBottom);
	pCT->Transform(v3RightTop, v3RightTop);

	QLineF LeftTop_RightBottom(QPointF(v3LeftTop.x(), v3LeftTop.y()), QPointF(v3RightBottom.x(), v3RightBottom.y()));
	QLineF LeftBottom_RightTop(QPointF(v3LeftBottom.x(), v3LeftBottom.y()), QPointF(v3RightTop.x(), v3RightBottom.y()));

	QPointF IntersectPoint;
	if(QLineF::BoundedIntersection != (LeftBottom_RightTop.intersect(LeftBottom_RightTop, &IntersectPoint)))
	{
		return;
	}
	
	double dRadius = sqrt(pow((IntersectPoint.y() - v3LeftTop.y()),2) + pow((IntersectPoint.x() - v3LeftTop.x()),2));

	double dist, right, left, top, bottom;

	left = v3LeftTop.x();
	top = v3LeftTop.y();
	right = v3RightBottom.x();
	bottom = v3RightBottom.y();

	double vertical2 = fabs(right - left)/* / zNear*/ / 2.;
	double horizontal2 = fabs(top - bottom)/* / zNear *// 2.;
	double dim = horizontal2 < vertical2 ? horizontal2 : vertical2;
	double viewAngle = atan2(dim,1.);
	dist = dRadius / sin(viewAngle);

	osg::Vec3d eye, center, up;

	m_p2DManipulater->getHomePosition(eye, center, up);
	eye.set(IntersectPoint.x(), IntersectPoint.y(), eye.y());
	center = eye;
	
	m_p2DManipulater->setHomePosition(eye + osg::Vec3d(0.0, -dist,0.0f), center, up, false);

	m_p2DManipulater->home(0);
}

void ExtendMap::UpdateMap()
{
	if(!m_pView) return;
	m_pView->getCamera()->setClearColor(m_v4BackGroundColor);
	MapLocation(m_v3MapCenter.x(), m_v3MapCenter.y());
	//MapScale(m_fMapLeft, m_fMapBottom, m_fMapRight, m_fMapTop);
	
}

void ExtendMap::ScaleToLayer( std::string strName )
{
	if(!m_pView) return;
	m_pView->setCameraManipulator(m_p2DManipulater);

	osg::Camera* pCamera = m_pView->getCamera();
	Layer* pCurrentLayer = GetLayer(strName);
	osg::Node* pN = 0;
	if(pCurrentLayer)
	{
		pN = pCurrentLayer->GetLayerNode();
	}
	else
	{
		pN = m_pRootNode;
	}

	if(!pCamera || !pN) return;
	osg::BoundingSphere boundingSphere = pN->getBound();

	double left, right, bottom, top, zNear, zFar;
	double dist;
	if(pCamera->getProjectionMatrixAsFrustum(left,right,bottom,top,zNear,zFar))
	{
		double vertical2 = fabs(right - left) / zNear / 2.;
		double horizontal2 = fabs(top - bottom) / zNear / 2.;
		double dim = horizontal2 < vertical2 ? horizontal2 : vertical2;
		double viewAngle = atan2(dim,1.);
		dist = boundingSphere.radius() / sin(viewAngle);
	}
	else
	{
		pCamera->getProjectionMatrixAsOrtho(left, right,bottom, top, zNear, zFar);
		double vertical2 = fabs(right - left);
		double horizontal2 = fabs(top - bottom);
		//double dscale = vertical2/horizontal2;
		double dim = horizontal2 < vertical2 ? horizontal2 : vertical2;
		double dscale = dim/(boundingSphere.radius() * 2);
		pCamera->setProjectionMatrixAsOrtho(left / dscale, right / dscale, bottom / dscale, top / dscale, zNear, zFar);
		dist = fabs(zFar - zNear) / 2;
	}
	osg::Vec3d vec3Zero(0,0,0);
	osg::Vec3d eye, center, up;

	eye.set(boundingSphere.center().x(), boundingSphere.center().y(), boundingSphere.center().z());
	center = eye;
	
	osgGA::CameraManipulator* pCM = m_pView->getCameraManipulator();
	pCM->setHomePosition(eye + osg::Vec3d(0.0, -dist,0.0f), center, osg::Vec3d(0.0f,0.0f,1.0f), false);
	pCM->home(0);

	m_pView->getCamera()->getProjectionMatrixAsPerspective(
		m_2dCamera.fovy, 
		m_2dCamera.aspectRatio, 
		m_2dCamera.zNear, 
		m_2dCamera.zFar);
}

void ExtendMap::Site( osg::Vec3 pos )
{
	osg::NodePath nodePath = m_3DMapManager.GetEarthNodePath();
	unsigned uMark = m_3DMapManager.GetEarthNodeMark();
	//idpEarthManipulator* pEM = new idpEarthManipulator(nodePath, uMark);
	idpEarthManipulator* pEM = dynamic_cast<idpEarthManipulator*>(m_pView->getCameraManipulator());
	
	//m_pView->getViewer()->setCameraManipulator(pEM);
	pEM->setViewpoint( osgEarth::Util::Viewpoint(pos.y(), pos.x(), 0, 45, 0, 100000.0), 5.0 );
}

osg::Vec3& ExtendMap::GetCurrentLocation()
{
	idpEarthManipulator *iem = dynamic_cast<idpEarthManipulator *>(m_pView->getCameraManipulator());
	if(iem == 0)
		return Map::GetCurrentLocation();
	else
	{
		osg::Vec3 v3;
		iem->GetCurrentPosition(v3);
		return v3;
	}
}

void ExtendMap::SetAttachView( osgViewer::View* pV )
{
	Map::SetAttachView(pV);
	m_3DMapManager.SetViewer(m_pView);
	//InitDisplayLatLong();
}

void ExtendMap::InitDisplayLatLong()
{
	ViewerEventHandle* PrintCurrentLonLat = new ViewerEventHandle;
	PrintCurrentLonLat->m_pCamera = m_pView->getCamera();
	m_pView->addEventHandler(PrintCurrentLonLat);
	PrintCurrentLonLat->m_pExtendMap = this;

	goto_gis::CoordinateTransform* pCT = GetCoordinateTransform();
	PrintCurrentLonLat->m_Proj4->setSourceCRS(pCT->getDesCRS());
	PrintCurrentLonLat->m_Proj4->setSourceProjName(pCT->getDesProjName());
	PrintCurrentLonLat->m_Proj4->setDestCRS(pCT->getSourceCRS());
	PrintCurrentLonLat->m_Proj4->setDesProjName(pCT->getSourceProjName());
}


