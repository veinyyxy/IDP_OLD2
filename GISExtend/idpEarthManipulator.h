#pragma once

#include "GISExtendConfig.h"

class idpEarthManipulator :
	public osgEarth::Util::EarthManipulator
{
public:
	idpEarthManipulator(osg::NodePath& nodePath, unsigned um = 0xFFFFFFFF);
	virtual ~idpEarthManipulator(void);
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	osg::MatrixTransform* pickTransformNode(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);
	osg::Geometry* pickGeometry(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);
	osg::Drawable* pickDrawable(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);
	//void MoveGeometry(const osg::Vec3& Trans, osg::Geometry* pGeo);
	bool getWorldCoordsUnderMouse(osg::View* view, float x, float y, osg::Vec3d& out_coords);
	void GetCurrentPosition(osg::Vec3& pos);
private:
	unsigned m_uMark;
	osg::NodePath m_NodePath;
	osg::Vec3 m_currentPosition;//当前位置经纬度
};

