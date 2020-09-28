#include "GeoTo3DTransform.h"
GOTO_NAMESPACE_START

#define PI 3.141592653590
#define ANGLE_TO_RADIAN(A1) A1*PI/180
GeoTo3DTransform::GeoTo3DTransform(void) : m_fEarthR(6400.1f)
{
}


GeoTo3DTransform::~GeoTo3DTransform(void)
{
}

osg::Vec3 GeoTo3DTransform::Transform( const osg::Vec3 vertex)
{
	float x, y, z;
	GeoTo3D(vertex.x(), vertex.y(), vertex.z(), &x, &y, &z);
	osg::Vec3 transResult(x, y, z);
	return transResult;
}

osg::Vec3 GeoTo3DTransform::Transform( GLdouble gLx, GLdouble gLy, GLdouble gLz)
{
	float x, y, z;
	GeoTo3D(gLx, gLy, gLz, &x, &y, &z);
	osg::Vec3 transResult(x, y, z);
	return transResult;
}

int GeoTo3DTransform::Transform( const osg::Vec3Array& src, osg::Vec3Array& dst)
{
	return -1;
}

int GeoTo3DTransform::Transform( const osg::Vec3& src, osg::Vec3& dst )
{
	float x, y, z;
	GeoTo3D(src.x(), src.y(), src.z(), &x, &y, &z);
	dst.set(x, y, z);
	return 0;
}

//QRectF GeoTo3DTransform::TransformBoundingBox( const QRectF rectangle, TRANSFORM_DIRECTION direction /*= FORWARD_DIRECTION*/ )
//{
//	QRectF planeRect(0, 0, m_fEarthR, m_fEarthR);
//	return planeRect;
//}

std::string GeoTo3DTransform::ImportFromESRI( const char * pszFname )
{
	std::string notThis("Do not need this.");
	return notThis;
}

void GeoTo3DTransform::setDestCRS( std::string ProjString )
{

}

std::string GeoTo3DTransform::getDesCRS()
{
	std::string notThis("Do not need this.");
	return notThis;
}

void GeoTo3DTransform::setSourceCRS( std::string ProjString )
{

}

std::string GeoTo3DTransform::getSourceCRS()
{
	std::string notThis("Do not need this.");
	return notThis;
}

std::string GeoTo3DTransform::getDesProjName()
{
	std::string notThis("Do not need this.");
	return notThis;
}

std::string GeoTo3DTransform::getSourceProjName()
{
	std::string notThis("Do not need this.");
	return notThis;
}

void GeoTo3DTransform::setSourceProjName( std::string ProjString )
{

}

void GeoTo3DTransform::setDesProjName( std::string ProjString )
{

}

void GeoTo3DTransform::initProMap()
{

}

void GeoTo3DTransform::initGeoMap()
{

}

void GeoTo3DTransform::GeoTo3D( float fIX, float fIY, float fIZ, float* fOX, float* fOY, float* fOZ )
{
	fIZ = 0.0f;
	float fNewR =  m_fEarthR + fIZ;
	/**fOX = m_fEarthR * sin(ANGLE_TO_RADIAN(fIX)) * cos(ANGLE_TO_RADIAN(fIY));
	*fOY = m_fEarthR * sin(ANGLE_TO_RADIAN(fIX)) * sin(ANGLE_TO_RADIAN(fIY));
	*fOZ = m_fEarthR* cos(ANGLE_TO_RADIAN(fIX));*/
	*fOX = - fNewR * sin(fIX / 180 * 3.14159264) * cos(fIY / 180 * 3.14159264);
	*fOY = fNewR * cos(fIX / 180 * 3.14159264) * cos(fIY / 180 * 3.14159264);
	*fOZ = fNewR * sin(fIY / 180 * 3.14159264);
}

GOTO_NAMESPACE_END