#pragma once
/** @file		goContour.h
* @brief		
* @author		杨晓宇
* @date			2013.7
* @version		1.0
  @note			
*/

GOTO_NAMESPACE_START

/** @class  DataProvider
    @brief  等值线描述类
    @author 杨晓宇
    @note   暂时没什么作用
*/ 
class goContour :
	public goGeometry
{
public:
	goContour(void);
	virtual ~goContour(void);

	virtual void CreateOsgFromGeos( osg::Node* pNode );

	virtual void SetCoordinateTransform( CoordinateTransform* pCT );

	virtual geom::GeometryTypeId GetGeometryType();

	virtual void TransformGeometry();

public:
	void PushBackPoint(float x, float y, float z);
	void SetColor(float r, float g, float b, float a);
private:
	osg::ref_ptr<osg::Vec3Array> m_coodArray;
	osg::ref_ptr<osg::Vec3Array> m_VertexArray;
	osg::ref_ptr<osg::Vec4Array> m_ColorArray;
};

GOTO_NAMESPACE_END