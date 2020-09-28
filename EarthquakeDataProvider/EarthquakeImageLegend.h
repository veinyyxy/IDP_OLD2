#pragma once

#include <QtGui/QIMage>
#include "LegendInterface.h"
using namespace goto_gis;

class EarthquakeImageLegend :
	public LegendInterface
{
public:
	EarthquakeImageLegend(void);
	~EarthquakeImageLegend(void);

	virtual int GetLegendColor( double MinValue, double MaxValue, osg::Vec4* v4Color );

	virtual int GetValueScope( LegendInterface::P_VALUE_SCOPE valueList );

	virtual int GetValueScope( double MinValue, double MaxValue, QPaintDevice** pPaintDev );

	void InitLegendData();

	virtual int GetLegendSize( double MinValue, double MaxValue, int* size );

private:
	LegendInterface::VALUE_SCOPE m_ValueScope;
	QImage m_LegendImage;
};

