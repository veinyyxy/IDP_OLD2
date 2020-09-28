#pragma once
#include "GISExtendConfig.h"
#include <memory>
#include <osgDB/ReadFile>
#include <osg/Node>

#include <osg/Notify>
#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <OsgEarth/ModelLayer>
#include <osg/ImageUtils>

#include "RadarMarchingCubes.h"
#include "CoordinateTransform.h"
#include "MapFactory.h"
#include "IsoSurface.h"
#include "ClipPlaneInterface.h"

class osg::Node;
class osgEarth::Map;
class QRectF;
class InterpolationRadarDataInterface;
class RadarMarchingCubes;
class ClipBoard;

using namespace goto_gis;

struct CubeEdge
{
	v3Point start;
	v3Point end;
};

class OsgEarthBridge : public IsoSurface, public ClipPlaneInterface
{
public:
	OsgEarthBridge(void);
	virtual ~OsgEarthBridge(void);
	inline void SetViewer(osg::View* pV){m_pView = pV;}
	void LoadEarthModel(const char* earthFileName);
	void OpenSky(bool bO);
	inline osg::Node* GetOsgEarthMapNode(){return m_earthNode;}
	inline osgEarth::Map* GetOsgEarthMap() {return m_osgEarthMap;}
	void LoadGeoTiff(const char* layerName, const char* tifFileName);
	void LoadAicShp(const char* layerName, const char* shpFileName);
	
	virtual ClipBoard* AddClipPlane(osg::Node* pClipedNode, osg::Group* pMapRootNode,  const osg::Vec4& rect);
	//void DelClipPlane(int ID);
	virtual void DelClipPlane(ClipBoard* clipBoard);
	void AddCompass(const QString& strCompassFile);
	void UpdateCompass(float fw, float fh, float fangle);

	inline unsigned GetEarthNodeMark(){return m_EarthNodeMake;}
	inline osg::NodePath& GetEarthNodePath(){return m_EarthNodePath;}
	//inline void SetRadarInterface(InterpolationRadarDataInterface* pRDI){m_pRadarInterface = pRDI;}
	virtual void SetRadarMarchingCubes(RadarMarchingCubes* pRMC){m_pRadarMarchingCubes = pRMC;}
	//inline void SetRadarCenter(const osg::Vec3& cp){m_RadarCenter = cp;}

	virtual void SetAddtionalData(void* pData){osg::Vec3* v3Data = (osg::Vec3*)pData;m_RadarCenter = *v3Data;}
	virtual void ComputerIso(int iDp, bool bDel = false);
	virtual void DelIsoSuerface(int iIso);
	void Computer3DClouds();
	virtual void ComputerVolume(osg::ImageList& imageList, float fVLen, float fVWidth, float fVHeight);
	virtual void AddVolumeClipPlane(const osg::Vec4& rect);

	///debug
	inline void setCount1(int iC){iPrePointCount = iC;}
	inline void setCount2(int iC){iVertexCount = iC;}

	void LoadSky();
	inline goto_gis::CoordinateTransform* GetCoord(){return m_pCoordinateTransform;}
	inline osg::Geode* GetClipAndSurfaceGeode(){return m_pGeo;}
	//inline osg::Group* GetVolumeGroup(){return m_pVolumeNode;}
	//inline goto_gis::MapFactory* GetMapFactory();
	//ClipBoard* GetClipBoard(int id);
	//////////////////////////////////////////////////////////////////////////

private:
	goto_gis::CoordinateTransform* m_pCoordinateTransform;
	//void AddClipPlane(osg::Node* pClipedNode, osg::Group* pMapRootNode, osg::Vec3 v3Center, double dR);
	osg::Geometry* createCompass(const QString& compasspng);
	void ValueToColor(int iValue, osg::Vec4& v4Color);
	osg::ref_ptr<osgEarth::Map> m_osgEarthMap;
	osg::ref_ptr<osg::Group> m_earthNode;
	osg::View* m_pView;
	osg::ref_ptr<osgEarth::Util::SkyNode> s_sky;
	osg::ref_ptr<osgEarth::MapNode> m_EarthMapNode;
	osg::ref_ptr<osg::Projection> HUDNode;
	osg::ref_ptr<osg::Camera> m_pHUDCamera;
	osg::ref_ptr<osg::MatrixTransform> pCompassTransform;
	osg::NodePath m_EarthNodePath;
	osg::Node::NodeMask m_EarthNodeMake;

	RadarMarchingCubes* m_pRadarMarchingCubes;
	osg::Vec3 m_RadarCenter;
	//////////////////////////////////////////////////////////////////////////
	int m_dIsoValue;
	P_CUBE_DATA_VECTOR m_CubeDataBuffer;
	osg::ref_ptr<osg::Geode> m_pGeo;
	osg::ref_ptr<osg::Geometry> m_pGeometry;
	bool bStransformed;
	//debug 
	int iPrePointCount;
	int iVertexCount;
	osg::ref_ptr<osg::Vec3Array> m_NormalLineVertexArray;
	osg::ref_ptr<osg::Vec4Array> m_NormalLineColorArray;
	osg::ref_ptr<osg::Group> m_pClipNode;
	osg::ref_ptr<osg::Group> m_pVolumeNode;
	std::map<int, osg::Geometry*> m_pIsoValueMapToGeometry;

	osg::ClipNode* clipnode;
	//ClipBoard* m_pClipBoard;
};

