#pragma once
#include "gogeometry.h"

GOTO_NAMESPACE_START

class GEOSONTHEOSG_EXPORT goQuadrilateral :  public goGeometry
{
public:
	goQuadrilateral(void);
	~goQuadrilateral(void);
	virtual void CreateOsgFromGeos(osg::Node* pNode);
	virtual void SetCoordinateTransform(CoordinateTransform* pCT);
	void SetOsgArray(osg::Vec3Array* coodinateArray);
	void SetOsgColorArray(osg::Vec4Array &colorArray );
	virtual void TransformGeometry();
	inline geom::GeometryTypeId GetGeometryType(){return geom::GEOS_POLYGON;}
	inline void setGlenum(GLenum mode){m_mode = mode;};
	inline void setTess(bool tess){b_tess = tess;}

	virtual void operator=(const goQuadrilateral& other);
private:
	bool b_tess;
	osg::ref_ptr<osg::Vec3Array> m_coodArray;
	osg::ref_ptr<osg::Vec4Array> m_colorArray;
	GLenum m_mode;
};

GOTO_NAMESPACE_END