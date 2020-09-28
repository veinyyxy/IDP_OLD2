#pragma once
/** @file		goLinearRing.h
* @brief		
* @author		杨晓宇
* @date			2013.4
* @version		1.0
  @note			
*/

#include "goGeometry.h"
GOTO_NAMESPACE_START

/** @class  goLineString
    @brief  GIS中对应的线段类
    @author 杨晓宇
    @note   成员功能参见接口类。不明白哪个是接口类，请回家卖红薯
*/
class GEOSONTHEOSG_EXPORT goLineString : public goGeometry
{
public:
	goLineString(void);
	goLineString(geom::Geometry* geometry, CoordinateTransform* pCT = 0, Layer* pL = 0);
	//goLineString(geom::CoordinateSequence* pCoordinateSequence, CoordinateTransform* pCT = 0, Layer* pL = 0);
	virtual ~goLineString(void);
	virtual void CreateOsgFromGeos(osg::Node* pNode);
	virtual void SetCoordinateTransform(CoordinateTransform* pCT);
	virtual void TransformGeometry();
	inline geom::GeometryTypeId GetGeometryType(){return geom::GEOS_LINESTRING;}

	virtual void operator=(const goLineString& other);
};

GOTO_NAMESPACE_END