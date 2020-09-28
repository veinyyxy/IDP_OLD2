#include <QPainter>

#include "CipasComposerscalebar.h"
#include "CipasTicksscalebarstyle.h"

CipasTicksScaleBarStyle::CipasTicksScaleBarStyle( const CCipasComposerScaleBar* bar ): CCipasScaleBarStyle( bar )
{
  mTickPosition = TicksMiddle;
}

CipasTicksScaleBarStyle::CipasTicksScaleBarStyle(): CCipasScaleBarStyle( 0 )
{
  mTickPosition = TicksMiddle;
}

CipasTicksScaleBarStyle::~CipasTicksScaleBarStyle()
{

}

QString CipasTicksScaleBarStyle::name() const
{
  switch ( mTickPosition )
  {
    case TicksUp:
      return "Line Ticks Up";
    case TicksDown:
      return "Line Ticks Down";
    case TicksMiddle:
      return "Line Ticks Middle";
  }
  return "";  // to make gcc happy
}

void CipasTicksScaleBarStyle::draw( QPainter* p, double xOffset ) const
{
  if ( !mScaleBar )
  {
    return;
  }
  double barTopPosition = mScaleBar->fontAscentMillimeters( mScaleBar->font() ) + mScaleBar->labelBarSpace() + mScaleBar->boxContentSpace();
  double middlePosition = barTopPosition + mScaleBar->height() / 2.0;
  double bottomPosition = barTopPosition + mScaleBar->height();

  p->save();
  p->setPen( mScaleBar->pen() );

  QList<QPair<double, double> > segmentInfo;
  mScaleBar->segmentPositions( segmentInfo );

  QList<QPair<double, double> >::const_iterator segmentIt = segmentInfo.constBegin();
  for ( ; segmentIt != segmentInfo.constEnd(); ++segmentIt )
  {
    p->drawLine( segmentIt->first + xOffset, barTopPosition, segmentIt->first + xOffset, barTopPosition + mScaleBar->height() );
    switch ( mTickPosition )
    {
      case TicksDown:
        p->drawLine( xOffset + segmentIt->first, barTopPosition, xOffset + segmentIt->first + mScaleBar->segmentMillimeters(), barTopPosition );
        break;
      case TicksMiddle:
        p->drawLine( xOffset + segmentIt->first, middlePosition, xOffset + segmentIt->first + mScaleBar->segmentMillimeters(), middlePosition );
        break;
      case TicksUp:
        p->drawLine( xOffset + segmentIt->first, bottomPosition, xOffset + segmentIt->first + mScaleBar->segmentMillimeters(), bottomPosition );
        break;
    }
  }

  //draw last tick
  if ( !segmentInfo.isEmpty() )
  {
    double lastTickPositionX = segmentInfo.last().first + mScaleBar->segmentMillimeters();
    p->drawLine( lastTickPositionX + xOffset, barTopPosition, lastTickPositionX + xOffset, barTopPosition + mScaleBar->height() );
  }

  p->restore();

  //draw labels using the default method
  drawLabels( p );
}


