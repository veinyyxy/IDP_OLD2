#pragma once
#include "gogeometry.h"
GOTO_NAMESPACE_START
	
class GEOSONTHEOSG_EXPORT goPoint :	public goGeometry
{
public:
	goPoint(void);
	goPoint(geom::Geometry* geometry, CoordinateTransform* pCT = 0, Layer* pL = 0);
	virtual ~goPoint(void);
	virtual void CreateOsgFromGeos(osg::Node* pNode);
	virtual void SetCoordinateTransform(CoordinateTransform* pCT);
	virtual void TransformGeometry();
	inline geom::GeometryTypeId GetGeometryType(){return geom::GEOS_POINT;}

	virtual void operator=(const goPoint& other);
};

GOTO_NAMESPACE_END