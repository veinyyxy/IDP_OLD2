#pragma once
#include "idp_control_global.h"
#include "IDPDrawGeometryData.h"

class IDP_CONTROL_EXPORT DrawLine :
	public IDPDrawGeometryData
{
public:
	DrawLine(void);
	virtual ~DrawLine(void);
	virtual bool handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea );
	void SetColor(osg::Vec4& v4c);
	void SetDrawLayer( goto_gis::Layer* pLayer );
protected:
	virtual void CreateLineGeometry();
	osg::ref_ptr<osg::Geometry> m_pLine;
	osg::ref_ptr<osg::Vec4Array> m_Colors;
	osg::ref_ptr<osg::Vec3Array> m_pVertexs;
	osg::ref_ptr<osg::DrawArrays> m_DrawArrays;
	bool m_bStart;
};

