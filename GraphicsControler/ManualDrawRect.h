#pragma once
#include <osgGA/GUIEventHandler>

#include "GraphicsControler_global.h"
struct frameRect 
{
	double StartX;
	double StartY;
	double EndX;
	double EndY;
};
class GRAPHICSCONTROLER_EXPORT ManualDrawRect :
	public osgGA::GUIEventHandler
{
public:
public:
	ManualDrawRect(osg::Geode* pG = 0, double dZ = 1.0);
	virtual ~ManualDrawRect(void);
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	inline osg::Geometry* GetLinePtr(){return m_pRect;}
	inline osg::Geode* GetDrawGeode(){return m_pLayerGeode;}
	inline void setZValue(double dZ){m_dZValue = dZ;}
	inline frameRect GetFrameRect(){return m_rect;}
	void ScreenToGL(osg::Camera* pC, const osg::Vec3& screenPoint, osg::Vec3& glPoint );
	inline osg::Vec3Array* GetWorldPoint(){return m_pWorldPoint;}
	inline osg::Vec3Array* GetScreenPoint(){return m_pScreenPoint;}
	void RemoveRect();
	void AddBlankRect();
private:

	osg::Geode* m_pLayerGeode;
	double m_dZValue;
	bool m_bStart;
	double m_dStartX, m_dStartY;
	double m_dEndX, m_dEndY;
	frameRect m_rect; /** @brief ËÄ±ßÐÎ±ß¿ò×ø±ê */ 
	osg::ref_ptr<osg::Vec3Array> m_pWorldPoint;
	osg::ref_ptr<osg::Vec4Array> m_pPointColor;
	osg::ref_ptr<osg::Vec3Array> m_pScreenPoint;
	osg::ref_ptr<osg::Geometry> m_pRect;
};
