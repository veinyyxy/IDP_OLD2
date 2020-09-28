#pragma once
#include "goGeometry.h"

GOTO_NAMESPACE_START
	
class GEOSONTHEOSG_EXPORT goMultiPolygon :public goGeometry
{
public:
	goMultiPolygon(void);
	goMultiPolygon(geom::Geometry* geometry, CoordinateTransform* pCT = 0, Layer* pL = 0);
	virtual ~goMultiPolygon(void);
	virtual void CreateOsgFromGeos(osg::Node* pNode);
	virtual void SetCoordinateTransform(CoordinateTransform* pCT);
	virtual void TransformGeometry();
	inline geom::GeometryTypeId GetGeometryType(){return geom::GEOS_MULTIPOLYGON;}

	virtual void operator=(const goMultiPolygon& other);
};

GOTO_NAMESPACE_END