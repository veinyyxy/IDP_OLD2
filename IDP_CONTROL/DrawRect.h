#pragma once
#include "idp_control_global.h"
#include "IDPDrawGeometryData.h"

class IDP_CONTROL_EXPORT DrawRect :
	public IDPDrawGeometryData
{
public:
	DrawRect(void);
	virtual ~DrawRect(void);
	virtual bool handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea );
	void SetColor(osg::Vec4& v4c);
	void SetDrawLayer( goto_gis::Layer* pLayer );

protected:
	virtual void CreateLineGeometry();
	void ToDrawRect( osg::Vec3& pos, osg::Vec3 pos2, float fz, osg::Vec3Array& out );
	
	osg::ref_ptr<osg::Geometry> m_pLine;
	osg::ref_ptr<osg::Vec4Array> m_Colors;
	osg::ref_ptr<osg::Vec3Array> m_pVertexs;
	osg::ref_ptr<osg::Vec3Array> m_pCircleVertexs;
	osg::ref_ptr<osg::DrawArrays> m_DrawArrays;
	bool m_bStart;
	float m_fr, m_fa, m_fb;
	osg::Vec3 m_P1;
	osg::Vec3 m_P2;
};

