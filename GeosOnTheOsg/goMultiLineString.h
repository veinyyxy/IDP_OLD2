#pragma once
/** @file		goMark.h
* @brief		
* @author		杨晓宇
* @date			2013.4
* @version		1.0
  @note			
*/

#include "gogeometry.h"

GOTO_NAMESPACE_START

/** @class  goMultiLineString
    @brief  多线的实现，就是线条的集合
    @author 杨晓宇
    @note   成员功能参见接口类。
*/
class GEOSONTHEOSG_EXPORT goMultiLineString : public goGeometry
{
public:
	goMultiLineString(void);
	goMultiLineString(geom::Geometry* geometry, CoordinateTransform* pCT = 0, Layer* pL = 0);
	virtual ~goMultiLineString(void);
	virtual void CreateOsgFromGeos(osg::Node* pNode);
	virtual void SetCoordinateTransform(CoordinateTransform* pCT);
	virtual void TransformGeometry();
	inline geom::GeometryTypeId GetGeometryType(){return geom::GEOS_MULTILINESTRING;}

	virtual void operator=(const goMultiLineString& other);
};

GOTO_NAMESPACE_END