#pragma once
#include <osgGA/KeySwitchMatrixManipulator>
#include "Map.h"
#include "OsgEarthBridge.h"
#include "GISMapManager.h"

using namespace goto_gis;

class ExtendMap :
	public Map
{
public:
	typedef struct ___Perspective
	{
		double fovy;
		double aspectRatio;
		double zNear;
		double zFar;

	} PERSPECTIVE_PARAM, *P_PERSPECTIVE_PARAM;

	ExtendMap(osgViewer::View* pAttchViewer = 0);
	virtual ~ExtendMap(void);
	virtual void Switch2DMap();
	virtual void Switch3DMap();

	//OsgEarthBridge* GetOsgEarthBridge(){return &m_3DMapManager;}

	void Load2DMapFromConfig(std::string str2dMap);
	void Load3DMapFromConfig(std::string str3dMap);

	void MapLocation(float fLon, float fLat);
	void MapScale(float fLeft, float fBottom, float fRight, float fTop);
	virtual void UpdateMap();
	virtual void ScaleToLayer(std::string strName = "");

	virtual void Site( osg::Vec3 pos );
	virtual osg::Vec3& GetCurrentLocation();
	virtual void SetAttachView(osgViewer::View* pV);
	//virtual void SetCurrentLocation(const osg::Vec3& v3P);
	void InitDisplayLatLong();
private:
	
	goto_gis::CoordinateTransform* m_pCoordinateRefer;
	OsgEarthBridge m_3DMapManager;
	GISMapManager m_2DMapManager;
	//osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> m_pKeySwitch;
	osg::ref_ptr<osgGA::CameraManipulator> m_p2DManipulater;
	osg::ref_ptr<osgGA::CameraManipulator> m_pEarthManipulator;
	bool m_b2DMap;

	PERSPECTIVE_PARAM m_2dCamera;
	PERSPECTIVE_PARAM m_3dCamera;
};

