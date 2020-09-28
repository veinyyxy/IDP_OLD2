#include <osg/Referenced>
#include <osgEarth/SpatialReference>
#include "PlaneInfo.h"


PlaneInfo::PlaneInfo(void)
{
	m_dStationHeight = 0;
	m_dHeight = 0;
	m_pSpatialRef = 0;
	m_pClipPline = 0;
}


PlaneInfo::~PlaneInfo(void)
{
}
