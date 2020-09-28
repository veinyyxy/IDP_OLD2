#include <QList>
#include <QPainter>

#include "CipasScaleBarStyle.h"
#include "CipasComposerScaleBar.h"
#include "CipasSingleBoxScaleBarStyle.h"

CCipasSingleBoxScaleBarStyle::CCipasSingleBoxScaleBarStyle(const CCipasComposerScaleBar* bar): CCipasScaleBarStyle( bar )
{

}

CCipasSingleBoxScaleBarStyle::CCipasSingleBoxScaleBarStyle(): CCipasScaleBarStyle( 0 )
{

}

CCipasSingleBoxScaleBarStyle::~CCipasSingleBoxScaleBarStyle(void)
{
}

void CCipasSingleBoxScaleBarStyle::draw( QPainter* p, double xOffset ) const
{
	if ( !mScaleBar )
	{
		return;
	}
	double barTopPosition = mScaleBar->fontAscentMillimeters( mScaleBar->font() ) + mScaleBar->labelBarSpace() + mScaleBar->boxContentSpace();

	p->save();
	p->setPen( p->pen() );

	QList<QPair<double, double> > segmentInfo;
	mScaleBar->segmentPositions( segmentInfo );

	bool useColor = true; //alternate brush color/white

	QList<QPair<double, double> >::const_iterator segmentIt = segmentInfo.constBegin();
	for ( ; segmentIt != segmentInfo.constEnd(); ++segmentIt )
	{
		if ( useColor ) //alternating colors
		{
			p->setBrush( mScaleBar->brush() );
		}
		else //white
		{
			p->setBrush( QColor( 255, 255, 255 ) );
		}

		QRectF segmentRect( segmentIt->first + xOffset, barTopPosition, segmentIt->second, mScaleBar->height() );
		p->drawRect( segmentRect );
		useColor = !useColor;
	}

	p->restore();

	//draw labels using the default method
	drawLabels( p );
}

QString CCipasSingleBoxScaleBarStyle::name() const
{
	return "Single Box";
}