#include <QPainter>

#include "CipasComposition.h"
#include "CipasPaperItem.h"


CCipasPaperItem::CCipasPaperItem( CCipasComposition* c ): CComposerItem( c, false )
{
	initialize();
}

CCipasPaperItem::CCipasPaperItem( qreal x, qreal y, qreal width, qreal height, CCipasComposition* composition ): 
CComposerItem( x, y, width, height, composition, false )
{
	initialize();
}

CCipasPaperItem::CCipasPaperItem(): CComposerItem( 0, false )
{
	initialize();
}

CCipasPaperItem::~CCipasPaperItem()
{

}

void CCipasPaperItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget )
{
	if ( !painter )
	{
		return;
	}

	drawBackground( painter );

	//draw rectagle
	if(mComposition)
	{
		if(mComposition->IsDrawBounding())
		{
			qreal x = 0, y =0, width = rect().width(), height = rect().height();
			QPen* pP = mComposition->BoundingPen();
			int iLineNum = mComposition->LineNum();
			double dLineInv = mComposition->LineInv();
			double xMargin=mComposition->getMarginX();
			double yMargin=mComposition->getMarginY();
			x += xMargin;y +=yMargin;
			width -= xMargin * 2;
			height -= yMargin * 2;
			painter->save();
			painter->setPen(*pP);
			painter->setBrush(Qt::NoBrush);
			for(int i = 0; i < iLineNum; i++)
			{
				painter->drawRect(QRectF(x, y, width, height));
				x += dLineInv;
				//x += xMargin;
				y += dLineInv;
				//y +=yMargin;
				width -= (dLineInv) * 2;
				height -= (dLineInv) * 2;
			}
			painter->restore();
		}
	}
	
	//draw grid

	if ( mComposition )
	{
		if ( mComposition->snapToGridEnabled() && mComposition->plotStyle() ==  CCipasComposition::Preview
			&& mComposition->snapGridResolution() > 0 )
		{
			int gridMultiplyX = ( int )( mComposition->snapGridOffsetX() / mComposition->snapGridResolution() );
			int gridMultiplyY = ( int )( mComposition->snapGridOffsetY() / mComposition->snapGridResolution() );
			double currentXCoord = mComposition->snapGridOffsetX() - gridMultiplyX * mComposition->snapGridResolution();
			double currentYCoord;
			double minYCoord = mComposition->snapGridOffsetY() - gridMultiplyY * mComposition->snapGridResolution();

			if ( mComposition->gridStyle() == CCipasComposition::Solid )
			{
				painter->setPen( mComposition->gridPen() );

				//draw vertical lines


				for ( ; currentXCoord <= rect().width(); currentXCoord += mComposition->snapGridResolution() )
				{
					painter->drawLine( QPointF( currentXCoord, 0 ), QPointF( currentXCoord, rect().height() ) );
				}

				//draw horizontal lines
				currentYCoord = minYCoord;
				for ( ; currentYCoord <= rect().height(); currentYCoord += mComposition->snapGridResolution() )
				{
					painter->drawLine( QPointF( 0, currentYCoord ), QPointF( rect().width(), currentYCoord ) );
				}
			}
			else //'Dots' or 'Crosses'
			{
				QPen gridPen = mComposition->gridPen();
				painter->setPen( gridPen );
				painter->setBrush( QBrush( gridPen.color() ) );
				double halfCrossLength = mComposition->snapGridResolution() / 6;

				for ( ; currentXCoord <= rect().width(); currentXCoord += mComposition->snapGridResolution() )
				{
					currentYCoord = minYCoord;
					for ( ; currentYCoord <= rect().height(); currentYCoord += mComposition->snapGridResolution() )
					{
						/*if ( mComposition->gridStyle() == CCipasComposition::Dots )
						{
							QRectF pieRect( currentXCoord - gridPen.widthF() / 2, currentYCoord - gridPen.widthF() / 2, gridPen.widthF(), gridPen.widthF() );
							painter->drawChord( pieRect, 0, 5760 );
						}
						else*/ if ( mComposition->gridStyle() == CCipasComposition::Crosses )
						{
							painter->drawLine( QPointF( currentXCoord - halfCrossLength, currentYCoord ), QPointF( currentXCoord + halfCrossLength, currentYCoord ) );
							painter->drawLine( QPointF( currentXCoord, currentYCoord - halfCrossLength ), QPointF( currentXCoord, currentYCoord + halfCrossLength ) );
						}
					}
				}
			}
		}
	}
}

void CCipasPaperItem::initialize()
{
	setFlag( QGraphicsItem::ItemIsSelectable, false );
	setFlag( QGraphicsItem::ItemIsMovable, false );
	setZValue( 0 );
}

bool CCipasPaperItem::writeXML( QDomElement& elem, QDomDocument & doc ) const
{
	return true;
}

bool CCipasPaperItem::readXML( const QDomElement& itemElem, const QDomDocument& doc )
{
	return true;
}