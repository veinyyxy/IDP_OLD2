#pragma once
#include "idpdrawgeometrydata.h"
#include "Wind.h"

#include <osg/MatrixTransform>
#include <osg/AutoTransform>
#include <osgText/Text>

class IDP_CONTROL_EXPORT DrawWindSymbol :
	public IDPDrawGeometryData
{
public:
	DrawWindSymbol(int iWindSpeed, int iWindWidth = 10);
	virtual ~DrawWindSymbol(void);
	virtual bool handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea );
	void SetColor(osg::Vec4& v4c);
	void SetDrawLayer( goto_gis::Layer* pLayer );
	inline void SetWindSpeed(int iWindSpeed){m_iWindSpeed = iWindSpeed;}

protected:	
	virtual void CreateLineGeometry();
	void ToDrawCircle( osg::Vec3& pos, float fr, float fz, osg::Vec3Array& out );
	void CreateCircle(float fr);
	void SetTextOnLine(double dAngle, std::string& strText);
	double CalculateAngle(const osg::Vec3& startPoint, const osg::Vec3& endPoint);
	osgText::Text* CreateText(const std::string name, float fSize, const osg::Vec3 pos, const osg::Vec4 Color);
	void SetText(osgText::Text* pT, const std::string& strText);
	void CreateWind(float fSpeed, osg::Geometry* pWind);
	void CreateBoundBox(osg::BoundingBox& bb, osg::Geometry* pGeom);
private:
	CWind m_Wind;
	int m_iWindSpeed;
	osg::ref_ptr<osg::Geometry> m_pCircle;
	osg::ref_ptr<osg::Geometry> m_pLine;
	osg::ref_ptr<osg::Geometry> m_pWindLine;
	osg::ref_ptr<osg::Vec4Array> m_Colors;
	osg::ref_ptr<osg::Vec3Array> m_pVertexs;
	osg::ref_ptr<osg::Vec3Array> m_pWindVertexs;
	osg::ref_ptr<osg::Vec4Array> m_WindColors;
	osg::ref_ptr<osg::DrawArrays> m_DrawArrays;
	osg::ref_ptr<osg::AutoTransform> m_pCircleNode1;
	//osg::ref_ptr<osg::AutoTransform> m_pCircle2;
	osg::ref_ptr<osg::AutoTransform> m_pWindNode;
	osg::ref_ptr<osg::MatrixTransform> m_pWindTransformNode;
	bool m_bStart;

	osg::ref_ptr<osgText::Text> m_WindDirect;
	osg::ref_ptr<osgText::Text> m_SpeedWindText;

	std::vector<int> m_WindSpeedVector;

	int m_iWindID;
};

