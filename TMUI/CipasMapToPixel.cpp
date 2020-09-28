
#include <QPoint>
#include <QTextStream>

#include "CipasMapToPixel.h"

CCipasMapToPixel::CCipasMapToPixel( double mapUnitsPerPixel,
	double ymax,
	double ymin,
	double xmin )
	: mMapUnitsPerPixel( mapUnitsPerPixel ),
	yMax( ymax ),
	yMin( ymin ),
	xMin( xmin ),
	xMax( 0 )                 // XXX wasn't originally specified?  Why?
{
}


CCipasMapToPixel::~CCipasMapToPixel(void)
{
}

CCipasPoint CCipasMapToPixel::toMapPoint( double x, double y ) const
{
	double mx = x * mMapUnitsPerPixel + xMin;
	double my = -1 * (( y - yMax ) * mMapUnitsPerPixel - yMin );
	return CCipasPoint( mx, my );
}

CCipasPoint CCipasMapToPixel::toMapCoordinates( QPoint p ) const
{
	CCipasPoint mapPt = toMapPoint( p.x(), p.y() );
	return CCipasPoint( mapPt );
}

CCipasPoint CCipasMapToPixel::toMapCoordinates( int x, int y ) const
{
	return toMapPoint( x, y );
}

CCipasPoint CCipasMapToPixel::toMapCoordinatesF( double x, double y ) const
{
	return toMapPoint( x, y );
}

void CCipasMapToPixel::setMapUnitsPerPixel( double mapUnitsPerPixel )
{
	mMapUnitsPerPixel = mapUnitsPerPixel;
}

double CCipasMapToPixel::mapUnitsPerPixel() const
{
	return mMapUnitsPerPixel;
}

void CCipasMapToPixel::setYMaximum( double ymax )
{
	yMax = ymax;
}

void CCipasMapToPixel::setYMinimum( double ymin )
{
	yMin = ymin;
}

void CCipasMapToPixel::setXMinimum( double xmin )
{
	xMin = xmin;
}

void CCipasMapToPixel::setParameters( double mapUnitsPerPixel, double xmin, double ymin, double ymax )
{
	mMapUnitsPerPixel = mapUnitsPerPixel;
	xMin = xmin;
	yMin = ymin;
	yMax = ymax;

}

QString CCipasMapToPixel::showParameters()
{
	QString rep;
	QTextStream( &rep ) << "Map units/pixel: " << mMapUnitsPerPixel
		<< " X minimum: " << xMin << " Y minimum: " << yMin << " Y maximum: " << yMax;
	return rep;

}


CCipasPoint CCipasMapToPixel::transform( double x, double y ) const
{
	transformInPlace( x, y );
	return CCipasPoint( x, y );
}

CCipasPoint CCipasMapToPixel::transform( const CCipasPoint& p ) const
{
	double dx = p.x();
	double dy = p.y();
	transformInPlace( dx, dy );

	// QgsDebugMsg(QString("Point to pixel...X : %1-->%2, Y: %3 -->%4").arg(p.x()).arg(dx).arg(p.y()).arg(dy));
	return CCipasPoint( dx, dy );
}

void CCipasMapToPixel::transform( CCipasPoint* p ) const
{
	double x = p->x();
	double y = p->y();
	transformInPlace( x, y );

#ifdef QGISDEBUG
	// QgsDebugMsg(QString("Point to pixel...X : %1-->%2, Y: %3 -->%4").arg(p->x()).arg(x).arg(p->y()).arg(y));
#endif
	p->set( x, y );
}

void CCipasMapToPixel::transformInPlace( double& x, double& y ) const
{
	x = ( x - xMin ) / mMapUnitsPerPixel;
	y = yMax - ( y - yMin ) / mMapUnitsPerPixel;
}

void CCipasMapToPixel::transformInPlace( std::vector<double>& x,
	std::vector<double>& y ) const
{
	assert( x.size() == y.size() );
	for ( unsigned int i = 0; i < x.size(); ++i )
		transformInPlace( x[i], y[i] );
}
