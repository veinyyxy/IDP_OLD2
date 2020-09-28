#pragma once
#include "gogeometry.h"

GOTO_NAMESPACE_START

class GEOSONTHEOSG_EXPORT goPolygon :	public goGeometry
{
public:
	goPolygon(void);
	goPolygon(geom::Geometry* geometry, CoordinateTransform* pCT = 0, Layer* pL = 0);
	virtual ~goPolygon(void);
	virtual void CreateOsgFromGeos(osg::Node* pNode);
	virtual void SetCoordinateTransform(CoordinateTransform* pCT);
	virtual void TransformGeometry();
	inline geom::GeometryTypeId GetGeometryType(){return geom::GEOS_POLYGON;}

	virtual void operator=(const goPolygon& other);
	osg::Vec3Array* triangulizeGeometry(osg::Geometry* src);
private:
	geom::CoordinateSequence* pCoordinateSequence;
};

GOTO_NAMESPACE_END