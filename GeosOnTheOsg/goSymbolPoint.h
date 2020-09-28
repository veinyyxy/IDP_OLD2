#pragma once
#include "GeosOnTheOsg_Global.h"

GOTO_NAMESPACE_START

class GEOSONTHEOSG_EXPORT goSymbolPoint :
	public goto_gis::goGeometry, osg::Geometry
{
public:
	goSymbolPoint(void);
	goSymbolPoint(int iSize, const osg::Vec3& pos, osg::Image* pImage);
	virtual ~goSymbolPoint(void);
	virtual void CreateOsgFromGeos( osg::Node* pNode );
	virtual void SetCoordinateTransform( goto_gis::CoordinateTransform* pCT );
	virtual geom::GeometryTypeId GetGeometryType();
	virtual void TransformGeometry();

public:
	virtual void drawImplementation( osg::RenderInfo& renderInfo ) const;
	inline void SetPosition(osg::Vec3 pos){m_v3Position = pos;}
	inline void SetSize(int iS){m_iSize = iS;}
	inline void SetTexture(osg::Image* pI);
	inline void SetColor(const osg::Vec4& color){m_v4Color = color;}
	void SetAngle(float fA){m_fAnagle = fA;}

private:
	void ScreenToWorld(osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out, float fZ = 0.0f) const;
	void WorldToScreen(osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out) const;
	void ComputeWorld(osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out) const;
	void ComputeNotVPWorld(osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out) const;
	void goSymbolPoint::SetAnglePrivate( float fA );
	float m_fAnagle;
	osg::Vec3 m_v3Position, m_v3WorldPosition;
	osg::ref_ptr<osg::PrimitiveSet> m_pPrimitiveSet;
	osg::ref_ptr<osg::PrimitiveSet> m_pPrimitiveSet2;
	int m_iSize;
	osg::Vec4 m_v4Color;
	osg::Matrix m_m4Rotate;
	osg::ref_ptr<osg::Vec3Array> m_pVertexes;
	osg::ref_ptr<osg::Vec4Array> m_pColors;
	osg::ref_ptr<osg::Vec2Array> m_pTexCoord;
	osg::ref_ptr<osg::StateSet> m_pStateSet;
};

GOTO_NAMESPACE_END
