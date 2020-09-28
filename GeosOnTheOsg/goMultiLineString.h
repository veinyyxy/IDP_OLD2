#pragma once
/** @file		goMark.h
* @brief		
* @author		������
* @date			2013.4
* @version		1.0
  @note			
*/

#include "gogeometry.h"

GOTO_NAMESPACE_START

/** @class  goMultiLineString
    @brief  ���ߵ�ʵ�֣����������ļ���
    @author ������
    @note   ��Ա���ܲμ��ӿ��ࡣ
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