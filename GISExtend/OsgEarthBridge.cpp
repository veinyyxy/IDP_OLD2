#include <osgDB/ReadFile>
#include <osg/Node>
#include <osg/Material>
#include <osg/Notify>
#include <osg/ClipNode>
#include <osg/LineWidth>
#include <osgText/Text>
#include <osg/StateAttribute>
#include <osg/ShadeModel>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgSim/DOFTransform>
#include <osgUtil/SmoothingVisitor>
#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <OsgEarth/ModelLayer>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osg/ImageUtils>

#include "Compass.h"
#include "PlaneInfo.h"
#include "InterpolationRadarDataInterface.h"
#include "RadarDataFromMarchingCubes.h"
#include "MarchingCubes.h"
#include "ClipBoard.h"
#include "idpEarthManipulator.h"
#include "VolumeAlgorithmFactory.h"
#include "VolumeDraw.h"
#include "OsgEarthTransform.h"
#include "OsgEarthBridge.h"

#define M_PI       3.14159265358979323846

//////////////////////////////////////////////////////////////////////////
class ProjectCallBack : public osg::NodeCallback
{
public:
	ProjectCallBack() : m_pViewer(0){}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::Projection* pProjection = dynamic_cast<osg::Projection*>(node);
		if(pProjection && m_pViewer)
		{
			osg::Viewport* vp = m_pViewer->getCamera()->getViewport();
			pProjection->setMatrix(osg::Matrix::ortho2D(0,vp->width(),0, vp->height()));
		}
		traverse(node,nv);
	}
	inline void setViewer(osg::View* pV){m_pViewer = pV;}
private:
	osg::View* m_pViewer;
};

class CameraCallBack : public osg::NodeCallback
{
public:
	CameraCallBack() : m_pViewer(0){}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::Camera* pC = dynamic_cast<osg::Camera*>(node);
		if(pC)
		{
			osg::Viewport* vp = m_pViewer->getCamera()->getViewport();
			if(vp)
				pC->setProjectionMatrixAsOrtho2D(0,vp->width(),0, vp->height());
		}
		traverse(node,nv);
	}
	inline void setViewer(osg::View* pV){m_pViewer = pV;}
private:
	osg::View* m_pViewer;
};

class CompassCallBack : public osg::Drawable::UpdateCallback
{
public:
	void update(osg::NodeVisitor* nv, osg::Drawable* da)
	{
		/*osg::Viewport* pViewPort = m_pViewer->getCamera()->getViewport();
		double dWidth = pViewPort->width();
		double dHeight = pViewPort->height();
		osg::Geometry* pGeo = dynamic_cast<osg::Geometry*>(da);
		osg::Array* pA = pGeo->getVertexArray();

		osg::Vec3Array* pVec3Array = dynamic_cast<osg::Vec3Array*>(pA);

		pVec3Array->clear();*/

		/*osg::Matrix rotateMatrix;
		rotateMatrix.makeRotate(M_PI/180 * 30, osg::Vec3(0,0,1));
		osg::Geometry* pGeo = dynamic_cast<osg::Geometry*>(da);
		osg::Array* pA = pGeo->getVertexArray();

		osg::Vec3Array* pVec3Array = dynamic_cast<osg::Vec3Array*>(pA);

		(*pVec3Array)[0] = (*pVec3Array)[0] * rotateMatrix;
		(*pVec3Array)[1] = (*pVec3Array)[1] * rotateMatrix;
		(*pVec3Array)[2] = (*pVec3Array)[2] * rotateMatrix;
		(*pVec3Array)[3] = (*pVec3Array)[3] * rotateMatrix;*/

		/*float x(0), y(0);
		osgUtil::LineSegmentIntersector::Intersections secs;
		if(!m_pViewer) return;
		if(m_pViewer->computeIntersections(x, y, secs))
		{
		printf("%s", "asdfasdfasdfasdf");
		}*/
	}
	inline void SetViewer(osgViewer::View* pV){m_pViewer = pV;}
private:
	osgViewer::View* m_pViewer;
};

class TransformCallBack : public osg::NodeCallback
{
public:
	TransformCallBack() : m_pSRS(0), m_pViewer(0){}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::MatrixTransform* pMT = dynamic_cast<osg::MatrixTransform*>(node);
		if(pMT && m_pSRS && m_pViewer)
		{
			osg::Viewport* pViewPort = m_pViewer->getCamera()->getViewport();
			osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(m_pViewer);
			idpEarthManipulator* pIEM = 0;
			osgEarth::Viewpoint viewPoint;
			if(pViewer)
				pIEM = dynamic_cast<idpEarthManipulator*>(pViewer->getCameraManipulator());

			if(pIEM)
				viewPoint = pIEM->getViewpoint();
			double dAngle = viewPoint.getPitch() + 90;

			double dWidth = pViewPort->width();
			double dHeight = pViewPort->height();

			osg::Vec3d vec3(0.0f, 89.999999f, 0), earthVec3, transedEarthVec3, zAix(0, 0, 1), xAix(1, 0, 0), yAix(0, 1, 0);
			m_pSRS->transformToWorld(vec3, earthVec3);
			//osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(m_pViewer);
			osg::Camera* pC = m_pViewer->getCamera();
			osg::Matrix viewMatrix = pC->getViewMatrix();
			osg::Matrix projMatrix = pC->getProjectionMatrix();
			
			transedEarthVec3 = earthVec3 * viewMatrix * projMatrix;
			transedEarthVec3.normalize();

			double dCOS = 0;
			double dACOS = 0;// = acos(dCOS);
			if(dAngle < 45.00)
			{
				transedEarthVec3.set(transedEarthVec3.x(),transedEarthVec3.y(),0);
				dCOS = yAix * transedEarthVec3;
				if((transedEarthVec3.x() < 0 && transedEarthVec3.y() >0) || (transedEarthVec3.x() < 0 && transedEarthVec3.y() < 0))
					dACOS = acos(dCOS);
				else
					dACOS = -acos(dCOS);
			}
			else
			{
				transedEarthVec3.set(transedEarthVec3.x(), 0,transedEarthVec3.z());
				dCOS = zAix * transedEarthVec3;
				if((transedEarthVec3.x() < 0 && transedEarthVec3.z() >0) || (transedEarthVec3.x() < 0 && transedEarthVec3.z() < 0))
					dACOS = acos(dCOS);
				else
					dACOS = -acos(dCOS);
			}
			
			
			osg::Matrix roMaZ, roMaX, traMa, traMa2, endMa, centerMa;
			traMa.makeTranslate(osg::Vec3(-256/2, -256/2, 0));
			traMa2.makeTranslate(osg::Vec3(256/2, 256/2, 0));
			roMaZ.makeRotate(dACOS, osg::Vec3(0,0,1));
			centerMa.makeTranslate(osg::Vec3(dWidth/2 - 256/2, dHeight/2 - 256 / 2, 0));
			
			roMaX.makeRotate(M_PI/180 * dAngle, xAix);
			endMa = traMa * roMaZ * roMaX * traMa2 * centerMa;

			pMT->setMatrix(endMa);
		}
		traverse(node,nv);
	}
	inline void SetSRS(const osgEarth::Util::SpatialReference* pSRS){m_pSRS = pSRS;}
	inline void setViewer(osg::View* pV){m_pViewer = pV;}
private:
	const osgEarth::Util::SpatialReference* m_pSRS;
	osg::View* m_pViewer;
};

OsgEarthBridge::OsgEarthBridge(void) : m_pView(0), s_sky(0), m_earthNode(0), HUDNode(0)
	, pCompassTransform(0), m_pRadarMarchingCubes(0)
	, m_pVolumeNode(new osg::Group)
	, clipnode(new osg::ClipNode)
{

	m_pGeo = new osg::Geode;
	m_earthNode = new osg::Group();
	m_EarthNodePath.push_back(m_earthNode);
	m_pGeometry = 0;

	m_NormalLineVertexArray = new osg::Vec3Array;
	m_NormalLineColorArray = new osg::Vec4Array;
	iPrePointCount = 300;
	iVertexCount = 3;

	bStransformed = false;

	m_earthNode->addChild(clipnode);
	//m_osgEarthMap = new osgEarth::Map;
}


OsgEarthBridge::~OsgEarthBridge(void)
{
}

void OsgEarthBridge::LoadEarthModel( const char* earthFileName )
{
	osg::Node* pNode = osgDB::readNodeFile(earthFileName);
	/*osg::StateSet* pStateSet = pNode->getOrCreateStateSet();
	osg::ref_ptr<osg::Material> mat = new osg::Material;
	mat->setDiffuse(osg::Material::FRONT,osg::Vec4(1.0,1.0,1.0,1.0));
	mat->setSpecular(osg::Material::FRONT,osg::Vec4(1.0,1.0,1.0,1.0));
	mat->setShininess(osg::Material::FRONT,90);
	mat->setAmbient(osg::Material::FRONT,osg::Vec4(1.0,1.0,1.0,1.0));
	mat->setEmission(osg::Material::FRONT,osg::Vec4(1.0,1.0,1.0,1.0));
	pStateSet->setAttribute(mat.get());*/
	
	//////////////////////////////////////////////////////////////////////////
	m_earthNode->addChild(pNode);
	osg::StateSet* pEarthState = m_earthNode->getOrCreateStateSet();
	
	//pEarthState->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );
	pEarthState->setRenderBinDetails(-1000000, "earthNode");
	//pEarthState->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
	//pEarthState->setMode(GL_BLEND, osg::StateAttribute::ON);
	osgEarth::MapNode* pMapNode = osgEarth::MapNode::findMapNode(pNode);
	pMapNode->setNodeMask(0x00000F0F);
	m_EarthNodeMake = pMapNode->getNodeMask();
	m_EarthNodePath.push_back(pMapNode);
	m_osgEarthMap = pMapNode->getMap();
	m_EarthMapNode = pMapNode;

	osgEarth::SpatialReference* pSRF = const_cast<osgEarth::SpatialReference* >(pMapNode->getMap()->getProfile()->getSRS());
	m_pCoordinateTransform = new OsgEarthTransform(pSRF);
	//LoadSky(pMapNode, m_earthNode);
	//AddCompass(m_earthNode);
}

void OsgEarthBridge::OpenSky( bool bO )
{

}

void OsgEarthBridge::LoadSky()
{
	if(!m_pView || !m_earthNode) return;

	if (m_EarthMapNode)
	{
		const osgEarth::Config& externals = m_EarthMapNode->externalConfig();

		if (m_EarthMapNode->getMap()->isGeocentric())
		{
			// Sky model.
			osgEarth::Config skyConf = externals.child("sky");

			double hours = skyConf.value("hours", 13);
			s_sky = new osgEarth::Util::SkyNode(m_osgEarthMap);
			s_sky->setDateTime(2014, 6, 6, hours);
			s_sky->attach(m_pView, 2);
			m_earthNode->addChild(s_sky);

			//m_pView->setLightingMode( osg::View::HEADLIGHT );
			//s_sky->getDateTime
			//m_pView->setLight( data._light.get() );
		}
	}
}

void OsgEarthBridge::LoadGeoTiff(const char* layerName, const char* tifFileName )
{
	osgEarth::Drivers::OGRFeatureOptions ogrOptions;
	ogrOptions.url() = tifFileName;

	osgEarth::Drivers::FeatureGeomModelOptions fgm_opt;
	fgm_opt.featureOptions() = ogrOptions;
	osgEarth::ModelLayer* pModelLayer = new osgEarth::ModelLayer(layerName
		, fgm_opt);

	m_osgEarthMap->addModelLayer(pModelLayer);
}

void OsgEarthBridge::LoadAicShp(const char* layerName, const char* shpFileName )
{

}

osg::Geometry* OsgEarthBridge::createCompass(const QString& compasspng)
{
	osg::Geometry* pGeom  = new osg::Geometry;
	///////////////////////////////////////////////////////////////////////
	osg::Vec3Array* pVertexArray = new osg::Vec3Array;
	/*pVertexArray->push_back(osg::Vec3(400,2,-300));
	pVertexArray->push_back(osg::Vec3(400,2,300));
	pVertexArray->push_back(osg::Vec3(-400,2,300));
	pVertexArray->push_back(osg::Vec3(-400,2,-300));*/
	/*pVertexArray->push_back(osg::Vec3(300,-300,0));
	pVertexArray->push_back(osg::Vec3(300,300,0));
	pVertexArray->push_back(osg::Vec3(-300,300,0));
	pVertexArray->push_back(osg::Vec3(-300,-300,0));*/
	/*pVertexArray->push_back(osg::Vec3(0,0,0));
	pVertexArray->push_back(osg::Vec3(200,0,0));
	pVertexArray->push_back(osg::Vec3(200,150,0));
	pVertexArray->push_back(osg::Vec3(0,150,0));*/
	pVertexArray->push_back(osg::Vec3(256,0, 0));
	pVertexArray->push_back(osg::Vec3(256,256,0));
	pVertexArray->push_back(osg::Vec3(0,256,0));
	pVertexArray->push_back(osg::Vec3(0,0,0));
	osg::Vec2Array* texcoords = new osg::Vec2Array;
	texcoords->push_back(osg::Vec2(1.0f,0.0f));
	texcoords->push_back(osg::Vec2(1.0f,1.0f));
	texcoords->push_back(osg::Vec2(0.0f,1.0f));
	texcoords->push_back(osg::Vec2(0.0f,0.0f));
	osg::Vec4Array* pColorArray = new osg::Vec4Array;
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,0.45f));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,0.45f));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,0.45f));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,0.45f));
	osg::Vec3Array* pNormalArray = new osg::Vec3Array;
	pNormalArray->push_back(osg::Vec3(0, -1, 0));
	pNormalArray->push_back(osg::Vec3(0, -1, 0));
	pNormalArray->push_back(osg::Vec3(0, -1, 0));
	pNormalArray->push_back(osg::Vec3(0, -1, 0));

	pGeom->setVertexArray(pVertexArray);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	pGeom->setTexCoordArray(0, texcoords);
	//pGeom->setVertexAttribBinding(0, osg::Geometry::BIND_PER_VERTEX);
	pGeom->setColorArray(pColorArray);
	pGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	//pGeom->setNormalArray(pNormalArray);
	//pGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::StateSet* pStateSet = new osg::StateSet;
	///////////////////////////////////设置纹理///////////////////////////////////////
	osg::Texture2D* pTexture2D = new osg::Texture2D;
	pTexture2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
	pTexture2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
	pTexture2D->setDataVariance(osg::Object::DYNAMIC);
	//QString compasspng = QCoreApplication::applicationDirPath() + "/Config/Compass1.png";
	osg::Image* pImage = osgDB::readImageFile(compasspng.toStdString());
	pTexture2D->setImage(pImage);
	//pStateSet->setAttribute(pTexture2D);
	pStateSet->setTextureAttributeAndModes(0,pTexture2D,osg::StateAttribute::ON);
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	pGeom->setStateSet(pStateSet);
	return pGeom;
}

void OsgEarthBridge::AddCompass(const QString& strCompassFile)
{
	//osg::Geode* pGeode = new osg::Geode;
	osg::Geometry* pCompass = createCompass(strCompassFile);
	CompassCallBack* pCompassGeodeCallback = new CompassCallBack;
	pCompassGeodeCallback->SetViewer((osgViewer::View*)m_pView);
	pCompass->setUpdateCallback(pCompassGeodeCallback);
	//pGeode->addDrawable(pCompass);
	osg::Geode* pCompassGeode = new osg::Geode;
	pCompassTransform = new osg::MatrixTransform;
	//osg::Camera* pCompassCamera = new osg::Camera;
#if 0
	HUDNode = new osg::Projection;
	osg::StateSet* pStateSet = HUDNode->getOrCreateStateSet();
	pStateSet->setRenderBinDetails(1000000, "CompassHUD");
	//pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	ProjectCallBack* pPCB = new ProjectCallBack;
	pPCB->setViewer(m_pView);
	HUDNode->setUpdateCallback(pPCB);
	HUDNode->addChild(pCompassTransform);
	HUDNode->setMatrix(osg::Matrix::ortho2D(0,1027,0,639));
	pG->addChild(HUDNode);
#else
	m_pHUDCamera = new osg::Camera;
	osg::StateSet* pStateSet = m_pHUDCamera->getOrCreateStateSet();
	pStateSet->setRenderBinDetails(1000000, "CompassHUD");
	//pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	CameraCallBack* pPCB = new CameraCallBack;
	pPCB->setViewer(m_pView);
	m_pHUDCamera->setUpdateCallback(pPCB);
	m_pHUDCamera->addChild(pCompassTransform);
	m_pHUDCamera->setProjectionMatrixAsOrtho2D(0,1027,0,639);
	//m_pHUDCamera->setClearDepth(1.0);
	m_pHUDCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
	m_pHUDCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	m_pHUDCamera->setViewMatrix(osg::Matrix::identity());
	m_pHUDCamera->setRenderOrder(osg::Camera::POST_RENDER);
	m_earthNode->addChild(m_pHUDCamera);
#endif
	TransformCallBack* pTCB = new TransformCallBack;
	pTCB->setViewer(m_pView);
	pTCB->SetSRS(m_osgEarthMap->getProfile()->getSRS());
	pCompassTransform->setUpdateCallback(pTCB);
	pCompassGeode->addDrawable(pCompass);
	pCompassTransform->addChild(pCompassGeode);
	pCompassTransform->setMatrix(osg::Matrix::identity());
	pCompassTransform->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	osg::StateSet* pTransState = pCompassTransform->getOrCreateStateSet();
	pTransState->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
	pCompassTransform->setCullingActive(false);
	//pCompassCamera->setRenderOrder(osg::Camera::POST_RENDER); 
}

void OsgEarthBridge::UpdateCompass( float fw, float fh, float fangle )
{
	osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(m_pView);
	osgGA::CameraManipulator* pCM = pViewer->getCameraManipulator();
}

ClipBoard* OsgEarthBridge::AddClipPlane(osg::Node* pClipedNode, osg::Group* pMapRootNode, const osg::Vec4& rect )
{
	/*
	left x
	top y
	right z
	bottom w
	*/
	double dStationHeight = 200;
	double dHeight = 20000;
	//osg::ClipNode* clipnode = new osg::ClipNode;
	//osg::Group* rootnode = pMapRootNode;
	osg::Vec3 p11, p12, p13, p14, p21, p22, p23, p24;

	p11.set(rect.x(), rect.y(), dStationHeight);
	p12.set(rect.x(), rect.w(), dStationHeight);
	p13.set(rect.x(), rect.w(), dHeight + dStationHeight);
	p14.set(rect.x(), rect.y(), dHeight + dStationHeight);
	
	p21.set(rect.x(), rect.w(), dStationHeight);
	p22.set(rect.z(), rect.w(), dStationHeight);
	p23.set(rect.z(), rect.w(), dHeight + dStationHeight);
	p24.set(rect.x(), rect.w(), dHeight + dStationHeight);
	
	osg::ref_ptr<ClipBoard> pClipBoard = new ClipBoard(m_osgEarthMap->getProfile()->getSRS());
	pClipBoard->SetFourPointAndInitClipBoard(p11, p12, p13, p14);
	pClipBoard->SetPlaneVisible(true);
	//ClipBoard* pGeode2 = new ClipBoard(m_osgEarthMap->getProfile()->getSRS());
	//pGeode2->SetFourPointAndInitClipBoard(p21, p22, p23, p24);
	//pGeode2->SetPlaneVisible(false);

	//clipnode->addClipPlane(m_pClipBoard->GetClipPlane());
	//clipnode->addClipPlane(pGeode2->GetClipPlane());

	osg::Group* pGroup = dynamic_cast<osg::Group*>(pClipedNode);

	if(pGroup)
	{
		pGroup->addChild(pClipBoard);
		//pGroup->addChild(pGeode2);
	}
	else
	{
		delete pClipBoard;
		return 0;
	}
	//rootnode->addChild(clipnode);
	//rootnode->setStateSet(clipnode->getStateSet());

	//m_pClipNode = rootnode;
	/*if(m_pRadarInterface)
	{
	double x1, y1, x2, y2;
	x1 = rect.left(), y1 = rect.top(), x2 = rect.right(), y2 = rect.top();
	m_pRadarInterface->SetStartEnd(&x1, &y1, &x2, &y2);
	m_pRadarInterface->Profiles();
	}*/

	//ComputerIso(36);
	//m_pClipNode->addChild(m_pGeo);

	//Computer3DClouds();

	return pClipBoard;
}

class DebugDraw : public osg::Drawable::DrawCallback
{
public:
	virtual void drawImplementation( osg::RenderInfo& renderInfo, const osg::Drawable* drawable) const
	{
		unsigned id = renderInfo.getContextID();
		int i = 0;
		i++;
	}

	virtual osg::Object* clone( const osg::CopyOp& ) const
	{
		return 0;
	}

};

void OsgEarthBridge::ComputerIso( int iDp, bool bDel )
{
	if(!m_pRadarMarchingCubes) return;

	m_pRadarMarchingCubes->SetRadarCentral(m_RadarCenter.x(), m_RadarCenter.y(), m_RadarCenter.z());

	m_CubeDataBuffer = m_pRadarMarchingCubes->GetCubes();

	if(!bStransformed)
	{
		//变换坐标
		for(size_t i = 0; i < m_CubeDataBuffer->size(); i++)
		{
			osg::Vec3d v3Temp;
			v3Temp.set(m_CubeDataBuffer->at(i).x, m_CubeDataBuffer->at(i).y, m_CubeDataBuffer->at(i).z);
			m_osgEarthMap->getProfile()->getSRS()->transformToWorld(v3Temp, v3Temp);
			m_CubeDataBuffer->at(i).x = v3Temp.x();
			m_CubeDataBuffer->at(i).y = v3Temp.y();
			m_CubeDataBuffer->at(i).z = v3Temp.z();
		}

		bStransformed = true;
	}

	osg::Geometry* pGeom = new osg::Geometry;
	
	osg::Vec3Array* vertexArray = new osg::Vec3Array;
	osg::Vec3Array* normalArray = new osg::Vec3Array;
	osg::Vec4Array* colorArray = new osg::Vec4Array;
	RadarDataFromMarchingCubes* pRDFMC = new RadarDataFromMarchingCubes(m_pRadarMarchingCubes, 0, m_CubeDataBuffer);
	pRDFMC->SetVertexColor(vertexArray, colorArray, normalArray);
	
	MarchingCubes lMarchingcubes(pRDFMC);

	lMarchingcubes.MarchingCubesMain(iDp);
	
	pGeom->setVertexArray(vertexArray);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertexArray->size()));
#if 0
	std::ofstream outfile("AAA.txt");
	size_t iC = 0;
	while(iC < vertexArray->size())
	{
		outfile<<vertexArray->at(iC).x()<<"		"<<vertexArray->at(iC).y()<<"		"<<vertexArray->at(iC).z()<<std::endl;
		++iC;
	}
#endif
	
	//pGeom->setNormalArray(normalArray);
	//pGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	pGeom->setColorArray(colorArray);
	pGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osgUtil::SmoothingVisitor::smooth(*pGeom, osg::PI);

	//pGeom->setDrawCallback(new DebugDraw);

	//pGeom->setUseDisplayList(false);

	m_pGeo->addDrawable(pGeom);
	
	//////////////////////////////////////////////////////////////////////////
	m_earthNode->addChild(m_pGeo);
	//////////////////////////////////////////////////////////////////////////
	m_pIsoValueMapToGeometry.insert(std::pair<int, osg::Geometry*>(iDp, pGeom));
	delete pRDFMC;
}

void OsgEarthBridge::Computer3DClouds()
{
	if(m_pRadarMarchingCubes)
	{
		if(m_pGeometry)
			m_pGeo->removeDrawable(m_pGeometry);

		osg::Vec3d v3WorldRadarCenter;

		osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array(/*2000000*/);
		osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array(/*2000000*/);
		osg::ref_ptr<osg::Vec3Array> normalArray = new osg::Vec3Array(/*2000000*/);

		m_pRadarMarchingCubes->SetRadarCentral(m_RadarCenter.x(), m_RadarCenter.y(), m_RadarCenter.z());

		m_CubeDataBuffer = m_pRadarMarchingCubes->GetCubes();

		if(!bStransformed)
		{
			//变换坐标
			for(size_t i = 0; i < m_CubeDataBuffer->size(); i++)
			{
				osg::Vec3d v3Temp;
				v3Temp.set(m_CubeDataBuffer->at(i).x, m_CubeDataBuffer->at(i).y, m_CubeDataBuffer->at(i).z);
				m_osgEarthMap->getProfile()->getSRS()->transformToWorld(v3Temp, v3Temp);
				m_CubeDataBuffer->at(i).x = v3Temp.x();
				m_CubeDataBuffer->at(i).y = v3Temp.y();
				m_CubeDataBuffer->at(i).z = v3Temp.z();
			}
			bStransformed = true;
		}

		for(size_t iCubeNum = 0; iCubeNum < m_CubeDataBuffer->size(); iCubeNum+=8)
		{
			osg::Vec3 v3P0, v3P1, v3P2, v3P3, v3P4, v3P5, v3P6, v3P7, v3Normal;
			osg::Vec4 v4Color;

			v3P0.set(m_CubeDataBuffer->at(iCubeNum + 0).x, m_CubeDataBuffer->at(iCubeNum + 0).y, m_CubeDataBuffer->at(iCubeNum + 0).z);
			v3P1.set(m_CubeDataBuffer->at(iCubeNum + 1).x, m_CubeDataBuffer->at(iCubeNum + 1).y, m_CubeDataBuffer->at(iCubeNum + 1).z);
			v3P2.set(m_CubeDataBuffer->at(iCubeNum + 2).x, m_CubeDataBuffer->at(iCubeNum + 2).y, m_CubeDataBuffer->at(iCubeNum + 2).z);
			v3P3.set(m_CubeDataBuffer->at(iCubeNum + 3).x, m_CubeDataBuffer->at(iCubeNum + 3).y, m_CubeDataBuffer->at(iCubeNum + 3).z);
			v3P4.set(m_CubeDataBuffer->at(iCubeNum + 4).x, m_CubeDataBuffer->at(iCubeNum + 4).y, m_CubeDataBuffer->at(iCubeNum + 4).z);
			v3P5.set(m_CubeDataBuffer->at(iCubeNum + 5).x, m_CubeDataBuffer->at(iCubeNum + 5).y, m_CubeDataBuffer->at(iCubeNum + 5).z);
			v3P6.set(m_CubeDataBuffer->at(iCubeNum + 6).x, m_CubeDataBuffer->at(iCubeNum + 6).y, m_CubeDataBuffer->at(iCubeNum + 6).z);
			v3P7.set(m_CubeDataBuffer->at(iCubeNum + 7).x, m_CubeDataBuffer->at(iCubeNum + 7).y, m_CubeDataBuffer->at(iCubeNum + 7).z);

			vertexArray->push_back(v3P0);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 0).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P1);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 1).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P2);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 2).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P3);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 3).value, v4Color);colorArray->push_back(v4Color);

			vertexArray->push_back(v3P0);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 0).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P1);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 1).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P5);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 5).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P4);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 4).value, v4Color);colorArray->push_back(v4Color);

			vertexArray->push_back(v3P0);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 0).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P3);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 3).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P7);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 7).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P4);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 4).value, v4Color);colorArray->push_back(v4Color);

			//vertexArray->push_back(v3P5);

			vertexArray->push_back(v3P6);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 6).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P5);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 5).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P4);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 4).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P7);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 7).value, v4Color);colorArray->push_back(v4Color);

			vertexArray->push_back(v3P6);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 6).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P5);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 5).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P1);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 1).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P2);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 2).value, v4Color);colorArray->push_back(v4Color);

			vertexArray->push_back(v3P6);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 6).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P7);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 7).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P3);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 3).value, v4Color);colorArray->push_back(v4Color);
			vertexArray->push_back(v3P2);ValueToColor(m_CubeDataBuffer->at(iCubeNum + 2).value, v4Color);colorArray->push_back(v4Color);
#if 1
			//////////////////////////////////normal////////////////////////////////////////
			v3Normal = (v3P3 - v3P0)^(v3P1 - v3P0);
			v3Normal.normalize();

			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);

			v3Normal = (v3P4 - v3P0)^(v3P1 - v3P0);
			v3Normal.normalize();

			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);

			v3Normal = (v3P4 - v3P0)^(v3P3 - v3P0);
			v3Normal.normalize();

			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);

			//normalArray->push_back(v3P5);

			v3Normal = (v3P7 - v3P6)^(v3P5 - v3P6);
			v3Normal.normalize();

			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);

			v3Normal = (v3P2 - v3P6)^(v3P5 - v3P6);
			v3Normal.normalize();

			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);

			v3Normal = (v3P2 - v3P6)^(v3P7 - v3P6);
			v3Normal.normalize();

			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			normalArray->push_back(v3Normal);
			//vertexArray->push_back(v3P0);vertexArray->push_back(v3P2);vertexArray->push_back(v3P1);/**/vertexArray->push_back(v3P0);vertexArray->push_back(v3P3);vertexArray->push_back(v3P2);
			//vertexArray->push_back(v3P0);vertexArray->push_back(v3P1);vertexArray->push_back(v3P4);/**/vertexArray->push_back(v3P1);vertexArray->push_back(v3P5);vertexArray->push_back(v3P4);
			//vertexArray->push_back(v3P1);vertexArray->push_back(v3P2);vertexArray->push_back(v3P5);/**/vertexArray->push_back(v3P2);vertexArray->push_back(v3P6);vertexArray->push_back(v3P5);
			//vertexArray->push_back(v3P2);vertexArray->push_back(v3P3);vertexArray->push_back(v3P6);/**/vertexArray->push_back(v3P3);vertexArray->push_back(v3P6);vertexArray->push_back(v3P7);
			//vertexArray->push_back(v3P0);vertexArray->push_back(v3P4);vertexArray->push_back(v3P7);/**/vertexArray->push_back(v3P0);vertexArray->push_back(v3P7);vertexArray->push_back(v3P3);
			//vertexArray->push_back(v3P4);vertexArray->push_back(v3P5);vertexArray->push_back(v3P6);/**/vertexArray->push_back(v3P4);vertexArray->push_back(v3P6);vertexArray->push_back(v3P7);
#endif
		}
		m_pGeometry = new osg::Geometry;

		m_pGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertexArray->size()));
		m_pGeometry->setVertexArray(vertexArray);

		m_pGeometry->setColorArray(colorArray);
		m_pGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

		//osgUtil::SmoothingVisitor::smooth(*m_pGeometry);
		m_pGeometry->setNormalArray(normalArray);
		m_pGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

		m_pGeo->addDrawable(m_pGeometry);

		//ValuePlane->addChild(pGeo);
		//m_earthNode->addChild(m_pGeo);
		//m_pClipNode->addChild(m_pGeo);
	}
}

void OsgEarthBridge::ValueToColor( int iDp, osg::Vec4& v4Color )
{
	if(m_pRadarMarchingCubes)
	{
		osg::Vec4 v4LocalColor;
		for(int iC = 0; iC < m_pRadarMarchingCubes->GetColorTable()->size(); iC++)
		{
			if(iDp >= m_pRadarMarchingCubes->GetColorTable()->at(iC)->fStart && iDp <= m_pRadarMarchingCubes->GetColorTable()->at(iC)->fEnd)
			{
				v4LocalColor.set(m_pRadarMarchingCubes->GetColorTable()->at(iC)->nRed / 255.0, m_pRadarMarchingCubes->GetColorTable()->at(iC)->nGreen / 255.0, m_pRadarMarchingCubes->GetColorTable()->at(iC)->nBlue / 255.0, 0.6);
			}
		}
		v4Color = v4LocalColor;
	}
}

void OsgEarthBridge::DelIsoSuerface( int iIso )
{
	std::map<int, osg::Geometry*>::iterator iFindResult = m_pIsoValueMapToGeometry.find(iIso);

	if(iFindResult != m_pIsoValueMapToGeometry.end())
	{
		m_pGeo->removeDrawable(iFindResult->second);
		m_pIsoValueMapToGeometry.erase(iFindResult);
	}
}

#if 1
osg::Node* createLine(osg::Vec3& p1, osg::Vec3& p2, osg::Vec4& colors = osg::Vec4(1, 1, 1, 1))
{
	osg::ref_ptr<osg::Vec3Array> lVertexs = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> lColors = new osg::Vec4Array;
	osg::Geometry* lpLine  = new osg::Geometry;

	lVertexs->push_back(p1);
	lVertexs->push_back(p2);
	/*lVertexs->push_back(osg::Vec3(-dsize, 1, -dsize));
	lVertexs->push_back(osg::Vec3(dsize, 1, -dsize));*/

	lColors->push_back(colors);

	lpLine->setVertexArray(lVertexs);
	lpLine->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, lVertexs->size()));

	lpLine->setColorArray(lColors);
	lpLine->setColorBinding(osg::Geometry::BIND_OVERALL);
	//lpLine->setDrawCallback(new DebugDraw);
	osg::Geode* pGeode = new osg::Geode;
	if(pGeode)
	{
		pGeode->addDrawable(lpLine);
	}
	return pGeode;
}
#endif

void OsgEarthBridge::ComputerVolume(osg::ImageList& imageList, float fVLen, float fVWidth, float fVHeight)
{
	VolumeDraw* pVD = VolumeAlgorithmFactory::GetVolumeDraw();

	pVD->InputImage(imageList, 8192);

	osg::Node* pVolumeNode = 0;


	osg::Vec3 radarCenter = m_RadarCenter;

	osg::MatrixTransform* pMT = new osg::MatrixTransform;
	//pMT->setReferenceFrame(osg::Transform::ABSOLUTE_RF_INHERIT_VIEWPOINT);

	radarCenter.set(radarCenter.y(), radarCenter.x(), 0/*radarCenter.z()*/);
	m_pCoordinateTransform->Transform(radarCenter, radarCenter);
	
	osg::Matrix transCenter, transCenter2, rotMatrix, tranMatrix;

	
	pVD->GetVolume(pMT);
	osg::Vec3 volumeCenter = pMT->getBound().center();

	volumeCenter.set(volumeCenter.x(), volumeCenter.y(), 0/*volumeCenter.z()*/);

	transCenter.makeTranslate(volumeCenter);
	//transCenter2 = transCenter;

	transCenter2 = osg::Matrix::inverse(transCenter);
	//////////////////////////////////////////////////////////////////////////
	osg::Vec3 normalRadarCenter = radarCenter;
	normalRadarCenter.normalize();
	osg::Vec3 zAix(0, 0, 1);

	//osg::Vec3 normalLine = zAix^normalRadarCenter;
	//normalLine.normalize();
	//float fDegree = zAix * normalRadarCenter;

	////fDegree = acos(fDegree);
	
	osg::Matrix scaleVolume;

	double dsx, dsy, dsz;
	osg::BoundingSphere bs = pMT->getBound();
	dsx = fVLen / bs.radius();
	dsy = fVWidth / bs.radius();
	dsz = fVHeight / bs.radius();

	scaleVolume.makeScale(dsx, dsy, dsz * 50);
	//////////////////////////////////////////////////////////////////////////
	rotMatrix.makeRotate(zAix,  normalRadarCenter);
	tranMatrix.makeTranslate(radarCenter);
	
	pMT->setMatrix( transCenter2 * scaleVolume * rotMatrix/* * transCenter */ * tranMatrix);

	m_pVolumeNode->addChild(pMT);
	m_earthNode->addChild(m_pVolumeNode);
	//clipnode->addChild(m_pVolumeNode);
	osg::Vec3 radarScale = radarCenter * 10000.0f;

	//m_earthNode->addChild(createLine(osg::Vec3(0, 0, 0), radarScale));
	//m_earthNode->addChild(createLine(osg::Vec3(0, 0, 0), osg::Vec3(19000000, 0, 0), osg::Vec4(1, 0, 0, 1)));
	//m_earthNode->addChild(createLine(osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 19000000), osg::Vec4(0, 1, 0, 1)));
	//m_earthNode->addChild(createLine(osg::Vec3(0, 0, 0), osg::Vec3(0, 19000000, 0), osg::Vec4(0, 0, 1, 1)));
}

void OsgEarthBridge::AddVolumeClipPlane(const osg::Vec4& rect )
{
	double dStationHeight = 200;
	double dHeight = 300000;
	
	//osg::Group* rootnode = pMapRootNode;
	osg::Vec3 p11, p12, p13, p14, p21, p22, p23, p24;

	p11.set(rect.x(), rect.y(), dStationHeight);
	p12.set(rect.x(), rect.w(), dStationHeight);
	p13.set(rect.x(), rect.w(), dHeight + dStationHeight);
	p14.set(rect.x(), rect.y(), dHeight + dStationHeight);
	
	p21.set(rect.x(), rect.w(), dStationHeight);
	p22.set(rect.z(), rect.w(), dStationHeight);
	p23.set(rect.z(), rect.w(), dHeight + dStationHeight);
	p24.set(rect.x(), rect.w(), dHeight + dStationHeight);
	
	ClipBoard* pGeode1 = new ClipBoard(m_osgEarthMap->getProfile()->getSRS());
	pGeode1->SetFourPointAndInitClipBoard(p11, p12, p13, p14);
	pGeode1->SetPlaneVisible(false);

	clipnode->addClipPlane(pGeode1->GetClipPlane());

	//osg::Group* pGroup = dynamic_cast<osg::Group*>(m_pVolumeNode);

	//clipnode->addChild(clipedNode);
	//pGroup->addChild(clipnode);

	m_earthNode->addChild(pGeode1);
}

#if 0
ClipBoard* OsgEarthBridge::GetClipBoard( int id )
{
	std::map< int, std::auto_ptr<ClipBoard> >::iterator findedClip = m_ClipBoardMap.find(id);

	if(findedClip != m_ClipBoardMap.end())
	{
		return findedClip->second.get();
	}

	return 0;
}

void OsgEarthBridge::DelClipPlane( int ID )
{
	ClipBoard* pCLipBoard = GetClipBoard(ID);

	if(pCLipBoard)
	{
		osg::Node* pParentNode = pCLipBoard->getParent(0);

		if(pParentNode)
		{
			osg::Group* pGroup = pParentNode->asGroup();

			if(pGroup)
			{
				pGroup->removeChild(pCLipBoard);
				m_ClipBoardMap.erase(ID);
			}
		}
	}
}
#endif
void OsgEarthBridge::DelClipPlane( ClipBoard* pCLipBoard )
{
	if(pCLipBoard->getNumParents() <= 0) return;

	osg::Node* pParentNode = pCLipBoard->getParent(0);

	if(pParentNode)
	{
		osg::Group* pGroup = pParentNode->asGroup();

		if(pGroup)
		{
			pGroup->removeChild(pCLipBoard);
		}
	}
}
