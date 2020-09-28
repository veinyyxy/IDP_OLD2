#pragma once
#include "CoordinateTransform.h"
GOTO_NAMESPACE_START

class GEOSONTHEOSG_EXPORT OsgEarthTransform :
	public CoordinateTransform
{
public:
	OsgEarthTransform(osgEarth::SpatialReference* pSRF);
	virtual ~OsgEarthTransform(void);
	virtual osg::Vec3 Transform(const osg::Vec3 vertex) ;
	virtual osg::Vec3 Transform(double gLx, double gLy, double gLz) ;
	virtual int Transform(const osg::Vec3Array& src, osg::Vec3Array& dst) ;
	virtual int Transform(const osg::Vec3& src, osg::Vec3& dst) ;
	//virtual QRectF TransformBoundingBox(const QRectF rectangle, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION) ;
	virtual std::string ImportFromESRI(const char *  pszFname ) ;
	virtual void setDestCRS (std::string ProjString) ;
	virtual std::string getDesCRS() ;
	virtual void setSourceCRS (std::string ProjString) ;
	virtual std::string getSourceCRS() ;

	virtual std::string getDesProjName() ;
	virtual std::string getSourceProjName() ;
	virtual void setSourceProjName(std::string ProjString) ;
	virtual void setDesProjName(std::string ProjString) ;

	inline void SetOsgEarthSRS(osgEarth::SpatialReference* pSRF){m_pSpatialReference = pSRF;}
	virtual inline bool setCenter(double lon,double lat) {return true;};
	virtual inline void getCenter(double &lon,double &lat){};

private:
	osgEarth::SpatialReference* m_pSpatialReference;
};

GOTO_NAMESPACE_END