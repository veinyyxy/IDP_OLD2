#include <QtCore/QCoreApplication>
#include "EarthquakeImageLegend.h"


EarthquakeImageLegend::EarthquakeImageLegend(void)
{
}


EarthquakeImageLegend::~EarthquakeImageLegend(void)
{
}

int EarthquakeImageLegend::GetLegendColor( double MinValue, double MaxValue, osg::Vec4* v4Color )
{
	return 0;
}

int EarthquakeImageLegend::GetValueScope( LegendInterface::P_VALUE_SCOPE valueList )
{
	return 0;
}

int EarthquakeImageLegend::GetValueScope( double MinValue, double MaxValue, QPaintDevice** pPaintDev )
{
	*pPaintDev = &m_LegendImage;
	return 1;
}

void EarthquakeImageLegend::InitLegendData()
{
	QString fileLegend = QCoreApplication::applicationDirPath()+QString::fromLocal8Bit("/Config/Legend1.png");

	m_LegendImage.load(fileLegend);
}

int EarthquakeImageLegend::GetLegendSize( double MinValue, double MaxValue, int* size )
{
	return 0;
}
