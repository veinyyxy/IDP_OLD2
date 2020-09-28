#pragma once
#include "GraphicsControler_global.h"

class GRAPHICSCONTROLER_EXPORT ManualDrawLine :
	public osgGA::GUIEventHandler
{
public:
	ManualDrawLine(osg::Geode* pG = 0, double dZ = 1.0);
	virtual ~ManualDrawLine(void);
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	void ScreenToGL(osg::Camera* pC, const osg::Vec3& screenPoint, osg::Vec3& glPoint );
	inline osg::Geometry* GetLinePtr(){return m_pLine;}
	inline osg::Geode* GetDrawGeode(){return m_pLayerGeode;}
	inline void setZValue(double dZ){m_dZValue = dZ;}
	inline osg::Vec3Array* GetWorldPoint(){return m_pWorldPoint;}
	inline osg::Vec3Array* GetScreenPoint(){return m_pScreenPoint;}

private:
	
	osg::Geode* m_pLayerGeode;
	double m_dZValue;
	bool m_bStart;
	double m_dStartX, m_dStartY;
	double m_dEndX, m_dEndY;
	osg::ref_ptr<osg::Vec3Array> m_pWorldPoint;
	osg::ref_ptr<osg::Vec3Array> m_pScreenPoint;
	osg::ref_ptr<osg::Geometry> m_pLine;
};

