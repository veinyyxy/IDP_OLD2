#pragma once
/** @file		glColorLump.h
* @brief		
* @author		������
* @date			2013.7
* @version		1.0
  @note			
*/

GOTO_NAMESPACE_START

/** @class  goColorLump
    @brief  ɫ��ͼͼԪ������
    @author ������
    @note   ��ʱûʲô����
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