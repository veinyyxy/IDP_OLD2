#pragma once
#include <iosfwd>

#include "tmui_global.h"
#include "CipasPoint.h"


class TMUI_EXPORT CCipasRectangle
{
  public:
    //! Constructor
    CCipasRectangle( double xmin = 0, double ymin = 0, double xmax = 0, double ymax = 0 );
    //! Construct a rectangle from two points. The rectangle is normalized after construction.
    CCipasRectangle( CCipasPoint const & p1, CCipasPoint const & p2 );
    //! Copy constructor
    CCipasRectangle( const CCipasRectangle &other );
    //! Destructor
    ~CCipasRectangle();
    //! Set the rectangle from two CCipasPoints. The rectangle is
    //normalised after construction.
    void set( const CCipasPoint& p1, const CCipasPoint& p2 );
    //! Set the rectangle from four points. The rectangle is
    //  normalised after construction.
    void set( double xmin, double ymin, double xmax, double ymax );
    //! Set the minimum x value
    void setXMinimum( double x );
    //! Set the maximum x value
    void setXMaximum( double x );
    //! Set the minimum y value
    void setYMinimum( double y );
    //! Set the maximum y value
    void setYMaximum( double y );
    //! Set a rectangle so that min corner is at max
    // and max corner is at min. It is NOT normalized.
    void setMinimal();
    //! Get the x maximum value (right side of rectangle)
    double xMaximum() const;
    //! Get the x minimum value (left side of rectangle)
    double xMinimum() const;
    //! Get the y maximum value (top side of rectangle)
    double yMaximum() const;
    //! Get the y minimum value (bottom side of rectangle)
    double yMinimum() const;
    //! Normalize the rectangle so it has non-negative width/height
    void normalize();
    //! Width of the rectangle
    double width() const;
    //! Height of the rectangle
    double height() const;
    //! Center point of the rectangle
    CCipasPoint center() const;
    //! Scale the rectangle around its center point
    void scale( double, const CCipasPoint *c = 0 );
    //! Expand the rectangle to support zoom out scaling
    void expand( double, const CCipasPoint *c = 0 );
    //! return the intersection with the given rectangle
    CCipasRectangle intersect( const CCipasRectangle *rect ) const;
    //! returns true when rectangle intersects with other rectangle
    bool intersects( const CCipasRectangle& rect ) const;
    //! return true when rectangle contains other rectangle
    //! @note added in version 1.1
    bool contains( const CCipasRectangle& rect ) const;
    //! return true when rectangle contains a point
    //! @note added in version 1.3
    bool contains( const CCipasPoint &p ) const;
    //! expand the rectangle so that covers both the original rectangle and the given rectangle
    void combineExtentWith( CCipasRectangle *rect );
    //! expand the rectangle so that covers both the original rectangle and the given point
    void combineExtentWith( double x, double y );
    //! test if rectangle is empty
    bool isEmpty() const;
    //! returns string representation in Wkt form
    QString asWktCoordinates() const;
    //! returns string representation of form xmin,ymin xmax,ymax
    QString toString( bool automaticPrecision = false ) const;
    //! overloaded toString that allows precision of numbers to be set
    QString toString( int thePrecision ) const;
    //! returns rectangle s a polygon
    QString asPolygon() const;
    /*! Comparison operator
      @return True if rectangles are equal
    */
    bool operator==( const CCipasRectangle &r1 ) const;
    /*! Comparison operator
    @return False if rectangles are equal
     */
    bool operator!=( const CCipasRectangle &r1 ) const;
    /*! Assignment operator
     * @param r1 CCipasRectangle to assign from
     */
    CCipasRectangle & operator=( const CCipasRectangle &r1 );

    /** updates rectangle to include passed argument */
    void unionRect( const CCipasRectangle& rect );

    /** Returns true if the rectangle has finite boundaries. Will
        return false if any of the rectangle boundaries are NaN or Inf. */
    bool isFinite() const;

  protected:

    // These are protected instead of private so that things like
    // the CCipasPostGisBox3d can get at them.

    double xmin;
    double ymin;
    double xmax;
    double ymax;

};


inline CCipasRectangle::~CCipasRectangle()
{
}

inline void CCipasRectangle::setXMinimum( double x )
{
  xmin = x;
}

inline void CCipasRectangle::setXMaximum( double x )
{
  xmax = x;
}

inline void CCipasRectangle::setYMinimum( double y )
{
  ymin = y;
}

inline void CCipasRectangle::setYMaximum( double y )
{
  ymax = y;
}

inline double CCipasRectangle::xMaximum() const
{
  return xmax;
}

inline double CCipasRectangle::xMinimum() const
{
  return xmin;
}

inline double CCipasRectangle::yMaximum() const
{
  return ymax;
}

inline double CCipasRectangle::yMinimum() const
{
  return ymin;
}

inline double CCipasRectangle::width() const
{
  return xmax - xmin;
}

inline double CCipasRectangle::height() const
{
  return ymax - ymin;
}

inline CCipasPoint CCipasRectangle::center() const
{
  return CCipasPoint( xmin + width() / 2,  ymin + height() / 2 );
}
inline std::ostream& operator << ( std::ostream& os, const CCipasRectangle &r )
{
  return os << r.toString().toLocal8Bit().data();
}


