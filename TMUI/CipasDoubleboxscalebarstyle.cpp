#include <QList>
#include <QPainter>

#include "CipasComposerscalebar.h"
#include "CipasDoubleboxscalebarstyle.h"

CipasDoubleBoxScaleBarStyle::CipasDoubleBoxScaleBarStyle( const CCipasComposerScaleBar* bar ): CCipasScaleBarStyle( bar )
{

}

CipasDoubleBoxScaleBarStyle::CipasDoubleBoxScaleBarStyle(): CCipasScaleBarStyle( 0 )
{

}

CipasDoubleBoxScaleBarStyle::~CipasDoubleBoxScaleBarStyle()
{

}

QString CipasDoubleBoxScaleBarStyle::name() const
{
  return "Double Box";
}

void CipasDoubleBoxScaleBarStyle::draw( QPainter* p, double xOffset ) const
{
  if ( !mScaleBar )
  {
    return;
  }
  double barTopPosition = mScaleBar->fontAscentMillimeters( mScaleBar->font() ) + mScaleBar->labelBarSpace() + mScaleBar->boxContentSpace();
  double segmentHeight = mScaleBar->height() / 2;

  p->save();
  p->setPen( p->pen() );

  QList<QPair<double, double> > segmentInfo;
  mScaleBar->segmentPositions( segmentInfo );

  bool useColor = true; //alternate brush color/white



  QList<QPair<double, double> >::const_iterator segmentIt = segmentInfo.constBegin();
  for ( ; segmentIt != segmentInfo.constEnd(); ++segmentIt )
  {
    //draw top half
    if ( useColor )
    {
      p->setBrush( mScaleBar->brush() );
    }
    else //white
    {
      p->setBrush( QColor( 255, 255, 255 ) );
    }

    QRectF segmentRectTop( segmentIt->first + xOffset, barTopPosition, segmentIt->second, segmentHeight );
    p->drawRect( segmentRectTop );

    //draw bottom half
    if ( useColor )
    {
      p->setBrush( QColor( 255, 255, 255 ) );
    }
    else //white
    {
      p->setBrush( mScaleBar->brush() );
    }

    QRectF segmentRectBottom( segmentIt->first + xOffset, barTopPosition + segmentHeight, segmentIt->second, segmentHeight );
    p->drawRect( segmentRectBottom );
    useColor = !useColor;
  }

  p->restore();

  //draw labels using the default method
  drawLabels( p );
}
