#pragma once
#include "goGeometry.h"
GOTO_NAMESPACE_START
	
class GEOSONTHEOSG_EXPORT goMultiPoint :public goGeometry
{
public:
	goMultiPoint(void);
	goMultiPoint(geom::Geometry* geometry, CoordinateTransform* pCT = 0, Layer* pL = 0);
	virtual ~goMultiPoint(void);
	virtual void CreateOsgFromGeos(osg::Node* pNode);
	virtual void SetCoordinateTransform(CoordinateTransform* pCT);
	virtual void TransformGeometry();
	inline geom::GeometryTypeId GetGeometryType(){return geom::GEOS_MULTIPOINT;}

	virtual void operator=(const goMultiPoint& other);
};

GOTO_NAMESPACE_END