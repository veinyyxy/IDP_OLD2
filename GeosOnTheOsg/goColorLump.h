#pragma once
/** @file		glColorLump.h
* @brief		
* @author		杨晓宇
* @date			2013.7
* @version		1.0
  @note			
*/

GOTO_NAMESPACE_START

/** @class  goColorLump
    @brief  色斑图图元描述类
    @author 杨晓宇
    @note   暂时没什么作用
*/ 
class goColorLump :
	public goGeometry
{
public:
	goColorLump(void);
	virtual ~goColorLump(void);

	virtual void CreateOsgFromGeos( osg::Node* pNode  );

	virtual void SetCoordinateTransform( CoordinateTransform* pCT );

	virtual geom::GeometryTypeId GetGeometryType();

	virtual void TransformGeometry();

};

GOTO_NAMESPACE_END