#pragma once
#include "LegendInterface.h"
#include "RadReadColor_XML.h"

class RadarSingleLegend :
	public goto_gis::LegendInterface
{
public:
	RadarSingleLegend(void);
	virtual ~RadarSingleLegend(void);

	virtual int GetValueScopeList( goto_gis::LegendInterface::P_VALUE_SCOPE valueList );

	virtual int GetLegendColor(double dValue, osg::Vec4* v4Color );
	virtual int GetLegendSize(double dValue, int* size );
	virtual int GetValueScope(double dValue, QPaintDevice* pPaintDev );
	virtual int GetValueScope(double dValue, osg::Drawable* pDrawable );
	virtual int GetValueScope(double dValue, osg::Node* pNode );
	virtual int GetVauleScope(double dValue, void* pAny );

	void SetColorTable(P_COLORTABLE_VECTOR pColorTable);
private:
	P_COLORTABLE_VECTOR m_ColorTable;
	goto_gis::LegendInterface::VALUE_SCOPE m_ValueScopeVector;
};

