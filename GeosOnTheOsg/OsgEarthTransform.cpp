#include <osgEarth/SpatialReference>

#include "OsgEarthTransform.h"

GOTO_NAMESPACE_START

OsgEarthTransform::OsgEarthTransform(osgEarth::SpatialReference* pSRF) : m_pSpatialReference(pSRF)
{
	m_TransType = TRANSFORM_3D;
}

OsgEarthTransform::~OsgEarthTransform(void)
{
}

osg::Vec3 OsgEarthTransform::Transform( const osg::Vec3 vertex)
{
	osg::Vec3 res;
	return res;
}

osg::Vec3 OsgEarthTransform::Transform( double gLx, double gLy, double gLz)
{
	osg::Vec3 res;
	return res;
}

int OsgEarthTransform::Transform( const osg::Vec3Array& src, osg::Vec3Array& dst)
{
	return 0;
}

int OsgEarthTransform::Transform( const osg::Vec3& src, osg::Vec3& dst )
{
	osg::Vec3d locDst;
	m_pSpatialReference->transformToWorld(src, locDst);

	dst.set(locDst.x(), locDst.y(), locDst.z());
	return 0;
}

std::string OsgEarthTransform::ImportFromESRI( const char * pszFname )
{
	std::string str;
	return str;
}

void OsgEarthTransform::setDestCRS( std::string ProjString )
{

}

std::string OsgEarthTransform::getDesCRS()
{
	std::string str;
	return str;
}

void OsgEarthTransform::setSourceCRS( std::string ProjString )
{

}

std::string OsgEarthTransform::getSourceCRS()
{
	std::string str;
	return str;
}

std::string OsgEarthTransform::getDesProjName()
{
	std::string str;
	return str;
}

std::string OsgEarthTransform::getSourceProjName()
{
	std::string str;
	return str;
}

void OsgEarthTransform::setSourceProjName( std::string ProjString )
{

}

void OsgEarthTransform::setDesProjName( std::string ProjString )
{

}

GOTO_NAMESPACE_END