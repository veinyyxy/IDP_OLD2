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

/** @class  goLinearRing
    @brief  GIS中对应的线环类
    @author 杨晓宇
    @note   成员功能参见接口类。不明白哪个是接口类，请回家卖红薯
*/
class GEOSONTHEOSG_EXPORT goLinearRing :public goGeometry
{
public:
	goLinearRing(void);
	goLinearRing(geom::Geometry* geometry, CoordinateTransform* pCT = 0, Layer* pL = 0);
	virtual ~goLinearRing(void);
	virtual void CreateOsgFromGeos(osg::Node* pNode);
	virtual void SetCoordinateTransform(CoordinateTransform* pCT);
	virtual void TransformGeometry();
	inline geom::GeometryTypeId GetGeometryType(){return geom::GEOS_LINEARRING;}

	virtual void operator=(const goLinearRing& other);
};

GOTO_NAMESPACE_END