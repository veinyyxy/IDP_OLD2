
#include <algorithm>
#include <cmath>
#include <limits>
#include <QString>
#include <QTextStream>
#include <qnumeric.h>

#include "CipasPoint.h"
#include "CipasRectangle.h"

CCipasRectangle::CCipasRectangle( double newxmin, double newymin, double newxmax, double newymax )
    : xmin( newxmin ), ymin( newymin ), xmax( newxmax ), ymax( newymax )
{
  normalize();
}

CCipasRectangle::CCipasRectangle( CCipasPoint const & p1, CCipasPoint const & p2 )
{
  set( p1, p2 );
}

CCipasRectangle::CCipasRectangle( const CCipasRectangle &r )
{
  xmin = r.xMinimum();
  ymin = r.yMinimum();
  xmax = r.xMaximum();
  ymax = r.yMaximum();
}

void CCipasRectangle::set( const CCipasPoint& p1, const CCipasPoint& p2 )
{
  xmin = p1.x();
  xmax = p2.x();
  ymin = p1.y();
  ymax = p2.y();
  normalize();
}

void CCipasRectangle::set( double xmin_, double ymin_, double xmax_, double ymax_ )
{
  xmin = xmin_;
  ymin = ymin_;
  xmax = xmax_;
  ymax = ymax_;
  normalize();
}

void CCipasRectangle::normalize()
{
  if ( xmin > xmax )
  {
    std::swap( xmin, xmax );
  }
  if ( ymin > ymax )
  {
    std::swap( ymin, ymax );
  }
} // CCipasRectangle::normalize()


void CCipasRectangle::setMinimal()
{
  xmin = std::numeric_limits<double>::max();
  ymin = std::numeric_limits<double>::max();
  xmax = -std::numeric_limits<double>::max();
  ymax = -std::numeric_limits<double>::max();
}

void CCipasRectangle::scale( double scaleFactor, const CCipasPoint * cp )
{
  // scale from the center
  double centerX, centerY;
  if ( cp )
  {
    centerX = cp->x();
    centerY = cp->y();
  }
  else
  {
    centerX = xmin + width() / 2;
    centerY = ymin + height() / 2;
  }
  double newWidth = width() * scaleFactor;
  double newHeight = height() * scaleFactor;
  xmin = centerX - newWidth / 2.0;
  xmax = centerX + newWidth / 2.0;
  ymin = centerY - newHeight / 2.0;
  ymax = centerY + newHeight / 2.0;
}

void CCipasRectangle::expand( double scaleFactor, const CCipasPoint * cp )
{
  // scale from the center
  double centerX, centerY;
  if ( cp )
  {
    centerX = cp->x();
    centerY = cp->y();
  }
  else
  {
    centerX = xmin + width() / 2;
    centerY = ymin + height() / 2;
  }

  double newWidth = width() * scaleFactor;
  double newHeight = height() * scaleFactor;
  xmin = centerX - newWidth;
  xmax = centerX + newWidth;
  ymin = centerY - newHeight;
  ymax = centerY + newHeight;
}

CCipasRectangle CCipasRectangle::intersect( const CCipasRectangle * rect ) const
{
  CCipasRectangle intersection = CCipasRectangle();
  //If they don't actually intersect an empty CCipasRectangle should be returned
  if ( !rect || !intersects( *rect ) )
  {
    return intersection;
  }

  intersection.setXMinimum( xmin > rect->xMinimum() ? xmin : rect->xMinimum() );
  intersection.setXMaximum( xmax < rect->xMaximum() ? xmax : rect->xMaximum() );
  intersection.setYMinimum( ymin > rect->yMinimum() ? ymin : rect->yMinimum() );
  intersection.setYMaximum( ymax < rect->yMaximum() ? ymax : rect->yMaximum() );
  return intersection;
}

bool CCipasRectangle::intersects( const CCipasRectangle& rect ) const
{
  double x1 = ( xmin > rect.xmin ? xmin : rect.xmin );
  double x2 = ( xmax < rect.xmax ? xmax : rect.xmax );
  if ( x1 > x2 ) return false;
  double y1 = ( ymin > rect.ymin ? ymin : rect.ymin );
  double y2 = ( ymax < rect.ymax ? ymax : rect.ymax );
  if ( y1 > y2 ) return false;
  return true;
}

bool CCipasRectangle::contains( const CCipasRectangle& rect ) const
{
  return ( rect.xmin >= xmin && rect.xmax <= xmax && rect.ymin >= ymin && rect.ymax <= ymax );
}

bool CCipasRectangle::contains( const CCipasPoint &p ) const
{
  return xmin <= p.x() && p.x() <= xmax &&
         ymin <= p.y() && p.y() <= ymax;
}

void CCipasRectangle::combineExtentWith( CCipasRectangle * rect )
{

  xmin = (( xmin < rect->xMinimum() ) ? xmin : rect->xMinimum() );
  xmax = (( xmax > rect->xMaximum() ) ? xmax : rect->xMaximum() );

  ymin = (( ymin < rect->yMinimum() ) ? ymin : rect->yMinimum() );
  ymax = (( ymax > rect->yMaximum() ) ? ymax : rect->yMaximum() );

}

void CCipasRectangle::combineExtentWith( double x, double y )
{

  xmin = (( xmin < x ) ? xmin : x );
  xmax = (( xmax > x ) ? xmax : x );

  ymin = (( ymin < y ) ? ymin : y );
  ymax = (( ymax > y ) ? ymax : y );

}

bool CCipasRectangle::isEmpty() const
{
  return xmax <= xmin || ymax <= ymin;
}

QString CCipasRectangle::asWktCoordinates() const
{
  QString rep =
    QString::number( xmin, 'f', 16 ) + " " +
    QString::number( ymin, 'f', 16 ) + ", " +
    QString::number( xmax, 'f', 16 ) + " " +
    QString::number( ymax, 'f', 16 );

  return rep;
}

// Return a string representation of the rectangle with automatic or high precision
QString CCipasRectangle::toString( bool automaticPrecision ) const
{
  if ( automaticPrecision )
  {
    int precision = 0;
    if (( width() < 1 || height() < 1 ) && ( width() > 0 && height() > 0 ) )
    {
      precision = static_cast<int>( ceil( -1.0 * log10( qMin( width(), height() ) ) ) ) + 1;
      // sanity check
      if ( precision > 20 )
        precision = 20;
    }
    return toString( precision );
  }
  else
    return toString( 16 );
}

// overloaded version of above fn to allow precision to be set
// Return a string representation of the rectangle with high precision
QString CCipasRectangle::toString( int thePrecision ) const
{
  QString rep;
  if ( isEmpty() )
    rep = "Empty";
  else
    rep = QString( "%1,%2 : %3,%4" )
          .arg( xmin, 0, 'f', thePrecision )
          .arg( ymin, 0, 'f', thePrecision )
          .arg( xmax, 0, 'f', thePrecision )
          .arg( ymax, 0, 'f', thePrecision );

//  CCipasDebugMsgLevel( QString( "Extents : %1" ).arg( rep ), 4 );

  return rep;
}


// Return the rectangle as a set of polygon coordinates
QString CCipasRectangle::asPolygon() const
{
//   QString rep = tmp.sprintf("%16f %16f,%16f %16f,%16f %16f,%16f %16f,%16f %16f",
//     xmin, ymin, xmin, ymax, xmax, ymax, xmax, ymin, xmin, ymin);
  QString rep;

  QTextStream foo( &rep );

  foo.setRealNumberPrecision( 8 );
  foo.setRealNumberNotation( QTextStream::FixedNotation );
  // NOTE: a polygon isn't a polygon unless its closed. In the case of
  //       a rectangle, that means 5 points (last == first)
  foo <<  xmin << " " <<  ymin << ", "
  <<  xmin << " " <<  ymax << ", "
  <<  xmax << " " <<  ymax << ", "
  <<  xmax << " " <<  ymin << ", "
  <<  xmin << " " <<  ymin;

  return rep;

} // CCipasRectangle::asPolygon() const


bool CCipasRectangle::operator==( const CCipasRectangle & r1 ) const
{
  return ( r1.xMaximum() == xMaximum() &&
           r1.xMinimum() == xMinimum() &&
           r1.yMaximum() == yMaximum() &&
           r1.yMinimum() == yMinimum() );
}


bool CCipasRectangle::operator!=( const CCipasRectangle & r1 ) const
{
  return ( ! operator==( r1 ) );
}


CCipasRectangle & CCipasRectangle::operator=( const CCipasRectangle & r )
{
  if ( &r != this )
  {
    xmax = r.xMaximum();
    xmin = r.xMinimum();
    ymax = r.yMaximum();
    ymin = r.yMinimum();
  }

  return *this;
}


void CCipasRectangle::unionRect( const CCipasRectangle& r )
{
  if ( r.xMinimum() < xMinimum() ) setXMinimum( r.xMinimum() );
  if ( r.xMaximum() > xMaximum() ) setXMaximum( r.xMaximum() );
  if ( r.yMinimum() < yMinimum() ) setYMinimum( r.yMinimum() );
  if ( r.yMaximum() > yMaximum() ) setYMaximum( r.yMaximum() );
}

bool CCipasRectangle::isFinite() const
{
  if ( qIsInf( xmin ) || qIsInf( ymin ) || qIsInf( xmax ) || qIsInf( ymax ) )
  {
    return false;
  }
  if ( qIsNaN( xmin ) || qIsNaN( ymin ) || qIsNaN( xmax ) || qIsNaN( ymax ) )
  {
    return false;
  }
  return true;
}
