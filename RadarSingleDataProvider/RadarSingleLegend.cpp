#include "RadarSingleLegend.h"


RadarSingleLegend::RadarSingleLegend(void)
{
}


RadarSingleLegend::~RadarSingleLegend(void)
{
}

int RadarSingleLegend::GetValueScopeList( goto_gis::LegendInterface::P_VALUE_SCOPE valueList )
{
	*valueList = m_ValueScopeVector;
	if(valueList->size() > 0)
		return 1;
	else
		return 0;
}

int RadarSingleLegend::GetLegendColor(double dValue, osg::Vec4* v4Color )
{
	osg::Vec4 v4CurrentColor(0, 0, 0, 0);

	for (int i = 0; i<m_ColorTable->count();i ++)
	{
		P_COLORNODE_PRODUCT color = m_ColorTable->at(i);
		
		if(dValue > color->fStart && dValue < color->fEnd)
		{
			v4CurrentColor.set(color->nRed/255.0, color->nGreen/255.0, color->nBlue/255.0, 1.0);
		}
	}

	*v4Color = v4CurrentColor;
	return 1;
}

int RadarSingleLegend::GetLegendSize(double dValue, int* size )
{
	return -1;
}

int RadarSingleLegend::GetValueScope( double dValue, QPaintDevice* pPaintDev )
{
	return -1;
}

int RadarSingleLegend::GetValueScope( double dValue, osg::Drawable* pDrawable )
{
	return -1;
}

int RadarSingleLegend::GetValueScope( double dValue, osg::Node* pNode )
{
	return -1;
}

int RadarSingleLegend::GetVauleScope( double dValue, void* pAny )
{
	return -1;
}

void RadarSingleLegend::SetColorTable( P_COLORTABLE_VECTOR pColorTable )
{
	m_ColorTable = pColorTable;

	for (int i = 0; i<m_ColorTable->count();i ++)
	{
		P_COLORNODE_PRODUCT color = m_ColorTable->at(i);
		m_ValueScopeVector.push_back(std::pair<double, double>(color->fStart, color->fEnd));
	}
}
