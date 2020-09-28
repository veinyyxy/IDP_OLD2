#include "goGeometry.h"
#include "Layer.h"
GOTO_NAMESPACE_START

	goGeometry::goGeometry(void) 
	: m_iGeometryID(-1), 
	m_pParentLayer(0),
	m_bTransform(true),
	m_pCorrdinateTransform(0), 
	m_bUserLayerColor(true)
{
	m_pOsgGeometry = new osg::Geometry;
	m_pColors = new osg::Vec4Array;
}

goGeometry::goGeometry(geom::Geometry* geometry, CoordinateTransform* pCT, Layer* pL)
	: m_pGeosGeomtry(geometry), 
	m_pCorrdinateTransform(pCT), 
	m_pParentLayer(pL),
	m_bTransform(true), 
	m_bUserLayerColor(true),
	m_bUserLayerZ(true),
	m_strName("")
{
	m_pOsgGeometry = new osg::Geometry;
	m_pColors = new osg::Vec4Array;
}

goGeometry::~goGeometry(void)
{
}

void goGeometry::UpdateGeometry()
{
	UpdateColor();
	m_pOsgGeometry->dirtyDisplayList();
	m_pOsgGeometry->dirtyBound();
}

void goGeometry::UpdateColor()
{
	if(m_bUserLayerColor)
	{
		m_pColors->clear();
		m_pColors->push_back(m_pParentLayer->GetLayerColor());
		m_pOsgGeometry->setColorArray(m_pColors);
	}
	else
	{
		m_pOsgGeometry->setColorArray(m_pColors);
	}
}

void goGeometry::TransormPoint( osg::Vec3& v3In, osg::Vec3& v3Out )
{
	osg::Vec3 srcVec3, destVec3;
	CoordinateTransform::TRANSFORM_TYPE TransType = m_pCorrdinateTransform->GetTransformType();
	if(TransType == CoordinateTransform::TRANSFORM_2D)
	{
		m_pCorrdinateTransform->Transform(v3In, destVec3);

		double x = destVec3.x();
		double y = destVec3.y();
		double z = destVec3.z();
		if(m_bUserLayerZ)
			y = m_pParentLayer->LayerZ();

		destVec3.set(x,y,z);
	}
	else if(TransType == CoordinateTransform::TRANSFORM_3D)
	{
		double z = v3In.z();
		if(m_bUserLayerZ)
			z = m_pParentLayer->LayerZ();
		
		srcVec3.set(v3In.x(), v3In.y(), z);

		m_pCorrdinateTransform->Transform(srcVec3, destVec3);
	}
	v3Out = destVec3;
}

GOTO_NAMESPACE_END