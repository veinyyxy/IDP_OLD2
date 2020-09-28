#pragma once

#include <iostream>
#include <QString>
#include <QPoint>

#include "tmui_global.h"

class TMUI_EXPORT CCipasVector
{
    double m_x, m_y;

  public:
    CCipasVector();
    CCipasVector( double x, double y );

    CCipasVector operator-( void ) const;
    CCipasVector operator*( double scalar ) const;
    CCipasVector operator/( double scalar ) const;
    double operator*( CCipasVector v ) const;
    double length() const;

    double x() const;
    double y() const;

    // perpendicular vector (rotated 90?counter-clockwise)
    CCipasVector perpVector() const;

    double angle( void ) const;
    double angle( CCipasVector v ) const;
    CCipasVector rotateBy( double rot ) const;
    CCipasVector normal() const;

};

/** \ingroup core
 * A class to represent a point geometry.
 * Currently no Z axis / 2.5D support is implemented.
 */

class TMUI_EXPORT CCipasPoint
{
public:
	CCipasPoint(void);
	    /*! Create a point from another point */
    CCipasPoint( const CCipasPoint& p );

    /*! Create a point from x,y coordinates
     * @param x x coordinate
     * @param y y coordinate
     */
    CCipasPoint( double x, double y )
        : m_x( x ), m_y( y )
    {}
	~CCipasPoint(void);
    void setX( double x )
    {
      m_x = x;
    }

    /*! Sets the y value of the point
     * @param y y coordinate
     */
    void setY( double y )
    {
      m_y = y;
    }

    /*! Sets the x and y value of the point */
    void set( double x, double y )
    {
      m_x = x;
      m_y = y;
    }

    /*! Get the x value of the point
     * @return x coordinate
     */
    double x() const
    {
      return m_x;
    }

    /*! Get the y value of the point
     * @return y coordinate
     */
    double y() const
    {
      return m_y;
    }

    //! String representation of the point (x,y)
    QString toString() const;

    //! As above but with precision for string representation of a point
    QString toString( int thePrecision ) const;

    /** Return a string representation as degrees minutes seconds.
     *  Its up to the calling function to ensure that this point can
     *  be meaningfully represented in this form.
     *  @note added in QGIS 1.4
     */
    QString toDegreesMinutesSeconds( int thePrecision ) const;


    /*! Return the well known text representation for the point.
     * The wkt is created without an SRID.
     * @return Well known text in the form POINT(x y)
     */
    QString wellKnownText() const;

    /**Returns the squared distance between this point and x,y*/
    double sqrDist( double x, double y ) const;

    /**Returns the squared distance between this and other point*/
    double sqrDist( const CCipasPoint& other ) const;

    /**Returns the minimum distance between this point and a segment
    @note added in QGIS 1.5*/
    double sqrDistToSegment( double x1, double y1, double x2, double y2, CCipasPoint& minDistPoint ) const;

    /**Calculates azimut between this point and other one (clockwise in degree, starting from north)
      @note: this function has been added in version 1.7*/
    double azimuth( const CCipasPoint& other );

    //! equality operator
    bool operator==( const CCipasPoint &other );

    //! Inequality operator
    bool operator!=( const CCipasPoint &other ) const;

    //! Assignment
    CCipasPoint & operator=( const CCipasPoint &other );

    //! Multiply x and y by the given value
    void multiply( const double& scalar );

    //! Test if this point is on the segment defined by points a, b
    //! @return 0 if this point is not on the open ray through a and b,
    //! 1 if point is on open ray a, 2 if point is within line segment,
    //! 3 if point is on open ray b.
    int onSegment( const CCipasPoint& a, const CCipasPoint& b ) const;

    CCipasVector operator-( CCipasPoint p ) const { return CCipasVector( m_x - p.m_x, m_y - p.m_y ); }
    CCipasPoint &operator+=( const CCipasVector &v ) { *this = *this + v; return *this; }
    CCipasPoint &operator-=( const CCipasVector &v ) { *this = *this - v; return *this; }
    CCipasPoint operator+( const CCipasVector &v ) const { return CCipasPoint( m_x + v.x(), m_y + v.y() ); }
    CCipasPoint operator-( const CCipasVector &v ) const { return CCipasPoint( m_x - v.x(), m_y - v.y() ); }

  private:

    //! x coordinate
    double m_x;

    //! y coordinate
    double m_y;


}; // class CCipasPoint


inline bool operator==( const CCipasPoint &p1, const CCipasPoint &p2 )
{
  if (( p1.x() == p2.x() ) && ( p1.y() == p2.y() ) )
    { return true; }
  else
    { return false; }
}

inline std::ostream& operator << ( std::ostream& os, const CCipasPoint &p )
{
  // Use Local8Bit for printouts
  os << p.toString().toLocal8Bit().data();
  return os;
}

//inline bool doubleNear( double a, double b, double epsilon = 4 * DBL_EPSILON )
//{
//	const double diff = a - b;
//	return diff > -epsilon && diff <= epsilon;
//}
