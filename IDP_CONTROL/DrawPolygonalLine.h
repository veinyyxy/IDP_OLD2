#pragma once
#include "idp_control_global.h"
#include "IDPDrawGeometryData.h"
#include "CardinalSpline.h"
#include "AlgorithmInterface.h"

class IDP_CONTROL_EXPORT DrawPolygonalLine :
	public IDPDrawGeometryData
{
public:
	DrawPolygonalLine(void);
	virtual ~DrawPolygonalLine(void);
	virtual bool handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea );
	void SetColor(osg::Vec4& v4c);
	void SetDrawLayer( goto_gis::Layer* pLayer );
	void SetAlgorithm(CAlgorithmInterface* pAI){m_pAlgorithmInterface = pAI;}

protected:
	virtual void CreateLineGeometry();
	void CreateBoundBox(osg::BoundingBox&, osg::Geometry* pGeom);
	osg::ref_ptr<osg::Geometry> m_pLine;
	osg::ref_ptr<osg::Vec4Array> m_Colors;
	osg::ref_ptr<osg::Vec3Array> m_pVertexs;
	osg::ref_ptr<osg::Vec3Array> m_pTempVertexs;
	osg::ref_ptr<osg::DrawArrays> m_DrawArrays;
	bool m_bStart;
	
	CCardinalSpline m_CardinalSpline;
	CAlgorithmInterface* m_pAlgorithmInterface;
};

