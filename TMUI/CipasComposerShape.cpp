#include <QPainter>

#include "CipasComposerShape.h"

CCipasComposerShape::CCipasComposerShape(CCipasComposition *composition)
	: CComposerItem( composition ), mShape( Ellipse )
{
	 initGraphicsSettings();
}
CCipasComposerShape::CCipasComposerShape(qreal x, qreal y, qreal width, qreal height,CCipasComposition *composition)
	: CComposerItem( x, y, width, height,composition ), mShape( Ellipse )
{
	setSceneRect( QRectF( x, y, width, height ) );
	mShapeWidth = width;
	mShapeHeight = height;
	initGraphicsSettings();
}
CCipasComposerShape::~CCipasComposerShape(void)
{
}

void CCipasComposerShape::paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget )
{
	if ( !painter )
	{
		return;
	}
	drawBackground( painter );

	painter->save();
	painter->setRenderHint( QPainter::Antialiasing );
	painter->setPen( mPen );
	painter->setBrush( mBrush );

	painter->translate( rect().width() / 2.0, rect().height() / 2.0 );
	painter->rotate( mRotation );
	painter->translate( -mShapeWidth / 2.0, -mShapeHeight / 2.0 );

	double halfPenWidth = mPen.widthF() / 2.0;

	switch ( mShape )
	{
	case Ellipse:
		painter->drawEllipse( QRectF( halfPenWidth, halfPenWidth , mShapeWidth - mPen.widthF(), mShapeHeight - mPen.widthF() ) );
		break;
	case Rectangle:
		painter->drawRect( QRectF( halfPenWidth, halfPenWidth , mShapeWidth - mPen.widthF(), mShapeHeight - mPen.widthF() ) );
		break;
	case Triangle:
		QPolygonF triangle;
		triangle << QPointF( halfPenWidth, mShapeHeight - halfPenWidth );
		triangle << QPointF( mShapeWidth - halfPenWidth, mShapeHeight - halfPenWidth );
		triangle << QPointF( mShapeWidth / 2.0, halfPenWidth );
		painter->drawPolygon( triangle );
		break;
	}

	painter->restore();

	drawFrame( painter );
	if ( isSelected() )
	{
		drawSelectionBoxes( painter );
	}
}


void CCipasComposerShape::setLineWidth( double width )
{
	mPen.setWidthF( width );
}

double CCipasComposerShape::lineWidth() const
{
	return mPen.widthF();
}

void CCipasComposerShape::setOutlineColor( const QColor& color )
{
	mPen.setColor( color );
}

QColor CCipasComposerShape::outlineColor() const
{
	return mPen.color();
}

void CCipasComposerShape::setFillColor( const QColor& color )
{
	mBrush.setColor( color );
}

QColor CCipasComposerShape::fillColor() const
{
	return mBrush.color();
}

bool CCipasComposerShape::transparentFill() const
{
	return mBrush.style() == Qt::NoBrush;
}

void CCipasComposerShape::setTransparentFill( bool transparent )
{
	if ( transparent )
	{
		mBrush.setStyle( Qt::NoBrush );
	}
	else
	{
		mBrush.setStyle( Qt::SolidPattern );
	}
}

void CCipasComposerShape::initGraphicsSettings()
{
	mPen.setColor( QColor( 0, 0, 0 ) );
	mPen.setWidthF( 1.0);
	mPen.setJoinStyle( Qt::RoundJoin );
	mBrush.setColor( QColor( 0, 0, 0 ) );
	mBrush.setStyle( Qt::NoBrush );

	//set composer item brush and pen to transparent white by default
	setPen( QPen( QColor( 255, 255, 255, 0 ) ) );
	setBrush( QBrush( QColor( 255, 255, 255, 0 ) ) );
}

void CCipasComposerShape::setRotation( double r )
{
	//adapt rectangle size
	double width = mShapeWidth;
	double height = mShapeHeight;
	sizeChangedByRotation( width, height );

	//adapt scene rect to have the same center and the new width / height
	double x = transform().dx() + rect().width() / 2.0 - width / 2.0;
	double y = transform().dy() + rect().height() / 2.0 - height / 2.0;
	CComposerItem::setSceneRect( QRectF( x, y, width, height ) );

	CComposerItem::setRotation( r );
}

void CCipasComposerShape::setSceneRect( const QRectF& rectangle )
{


	//consider to change size of the shape if the rectangle changes width and/or height
	if ( rectangle.width() != rect().width() || rectangle.height() != rect().height() )
	{
		double newShapeWidth = rectangle.width();
		double newShapeHeight = rectangle.height();
		imageSizeConsideringRotation( newShapeWidth, newShapeHeight );
		mShapeWidth = newShapeWidth;
		mShapeHeight = newShapeHeight;
	}

	CComposerItem::setSceneRect( rectangle );
}

bool CCipasComposerShape::writeXML( QDomElement& elem, QDomDocument & doc ) const
{
	QDomElement composerShapeElem = doc.createElement( "ComposerShape" );
	composerShapeElem.setAttribute( "shapeType", mShape );
	composerShapeElem.setAttribute( "outlineWidth", mPen.widthF() );
	composerShapeElem.setAttribute( "transparentFill", mBrush.style() == Qt::NoBrush );
	composerShapeElem.setAttribute( "shapeWidth", mShapeWidth );
	composerShapeElem.setAttribute( "shapeHeight", mShapeHeight );
	QDomElement outlineColorElem = doc.createElement( "OutlineColor" );
	outlineColorElem.setAttribute( "red", mPen.color().red() );
	outlineColorElem.setAttribute( "green", mPen.color().green() );
	outlineColorElem.setAttribute( "blue", mPen.color().blue() );
	outlineColorElem.setAttribute( "alpha", mPen.color().alpha() );
	composerShapeElem.appendChild( outlineColorElem );
	QDomElement fillColorElem = doc.createElement( "FillColor" );
	fillColorElem.setAttribute( "red", mBrush.color().red() );
	fillColorElem.setAttribute( "green", mBrush.color().green() );
	fillColorElem.setAttribute( "blue", mBrush.color().blue() );
	fillColorElem.setAttribute( "alpha", mBrush.color().alpha() );
	composerShapeElem.appendChild( fillColorElem );
	elem.appendChild( composerShapeElem );
	return _writeXML( composerShapeElem, doc );
}

bool CCipasComposerShape::readXML( const QDomElement& itemElem, const QDomDocument& doc )
{
	mShape = CCipasComposerShape::Shape( itemElem.attribute( "shapeType", "0" ).toInt() );
	mShapeWidth = itemElem.attribute( "shapeWidth", "10" ).toDouble();
	mShapeHeight = itemElem.attribute( "shapeHeight", "10" ).toDouble();
	mPen.setWidthF( itemElem.attribute( "outlineWidth", "0.4" ).toDouble() );

	//transparent fill
	bool transparent = itemElem.attribute( "transparentFill", "1" ).toInt() == 1;
	if ( transparent )
	{
		mBrush.setStyle( Qt::NoBrush );
	}
	else
	{
		mBrush.setStyle( Qt::SolidPattern );
	}

	//outline color
	QDomNodeList outlineColorList = itemElem.elementsByTagName( "OutlineColor" );
	if ( outlineColorList.size() > 0 )
	{
		QDomElement outlineColorElem = outlineColorList.at( 0 ).toElement();
		int penRed = outlineColorElem.attribute( "red", "0" ).toInt();
		int penGreen = outlineColorElem.attribute( "green", "0" ).toInt();
		int penBlue = outlineColorElem.attribute( "blue", "0" ).toInt();
		int penAlpha = outlineColorElem.attribute( "alpha", "255" ).toInt();
		mPen.setColor( QColor( penRed, penGreen, penBlue, penAlpha ) );
	}

	//fill color
	QDomNodeList fillNodeList = itemElem.elementsByTagName( "FillColor" );
	if ( fillNodeList.size() > 0 )
	{
		QDomElement fillColorElem = fillNodeList.at( 0 ).toElement();
		int brushRed = fillColorElem.attribute( "red", "0" ).toInt();
		int brushGreen = fillColorElem.attribute( "green", "0" ).toInt();
		int brushBlue = fillColorElem.attribute( "blue", "0" ).toInt();
		int brushAlpha = fillColorElem.attribute( "alpha", "255" ).toInt();
		mBrush.setColor( QColor( brushRed, brushGreen, brushBlue, brushAlpha ) );
	}


	//restore general composer item properties
	QDomNodeList composerItemList = itemElem.elementsByTagName( "ComposerItem" );
	if ( composerItemList.size() > 0 )
	{
		QDomElement composerItemElem = composerItemList.at( 0 ).toElement();
		_readXML( composerItemElem, doc );
	}
	emit itemChanged();
	return true;
}

