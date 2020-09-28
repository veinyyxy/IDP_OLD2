#pragma once
#include <osg/ClipPlane>

class PlaneInfo : public osg::Referenced 
{
public:
	PlaneInfo(void);
	virtual ~PlaneInfo(void);
	osg::Vec3 m_LeftPoint;
	osg::Vec3 m_RightPoint;
	double m_dHeight;
	double m_dStationHeight;
	const osgEarth::SpatialReference* m_pSpatialRef;
	osg::ClipPlane* m_pClipPline;
};

