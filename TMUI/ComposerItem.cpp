#include <QCoreApplication>
#include <QWidget>
#include <QDomNode>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <limits>
#include <cmath>
#include "math.h"

#include "CipasComposition.h"
#include "ComposerItem.h"

#define M_PI       3.14159265358979323846

#define FONT_WORKAROUND_SCALE 10 //scale factor for upscaling fontsize and downscaling painter

CComposerItem::CComposerItem( CCipasComposition* composition, bool manageZValue)
	: QObject( 0 )
	, QGraphicsRectItem( 0 )
	, mComposition( composition )
	, mBoundingResizeRectangle( 0 )
	, mFrame( false )
	, mItemPositionLocked( false )
	, mLastValidViewScaleFactor( -1 )
	, mRotation( 0 )
{
	setFlag( QGraphicsItem::ItemIsSelectable, true );
	setAcceptsHoverEvents( true );

	//set default pen and brush
	setBrush( QBrush( QColor( 255, 255, 255, 255 ) ) );
	QPen defaultPen( QColor( 0, 0, 0 ) );
	defaultPen.setWidthF( 0.3 );
	setPen( defaultPen );

	//let z-Value be managed by composition
	if ( mComposition && manageZValue )
	{
		mComposition->addItemToZList( this );
	}
}

CComposerItem::CComposerItem( qreal x, qreal y, qreal width, qreal height, CCipasComposition* composition, bool manageZValue )
	: QObject( 0 )
	, QGraphicsRectItem( 0, 0, width, height, 0 )
	, mComposition( composition )
	, mBoundingResizeRectangle( 0 )
	, mFrame( true )
	, mItemPositionLocked( false )
	, mLastValidViewScaleFactor( -1 )
	, mRotation( 0 )
{
	setFlag( QGraphicsItem::ItemIsSelectable, true );
	setAcceptsHoverEvents( true );

	QTransform t;
	t.translate( x, y );
	setTransform( t );

	//set default pen and brush
	setBrush( QBrush( QColor( 255, 255, 255, 255 ) ) );
	QPen defaultPen( QColor( 0, 0, 0 ) );
	defaultPen.setWidthF( 0.3 );
	setPen( defaultPen );

	//let z-Value be managed by composition
	if ( mComposition && manageZValue )
	{
		mComposition->addItemToZList( this );
	}
}

CComposerItem::~CComposerItem()
{
	if ( mComposition )
	{
		mComposition->removeItemFromZList( this );
	}

	delete mBoundingResizeRectangle;
}

void CComposerItem::setSelected( bool s )
{
	//CDebugMsg( "entered." );
	QGraphicsRectItem::setSelected( s );
	update(); //to draw selection boxes
}
/*

bool CComposerItem::writeSettings( void )  { return true; }

bool CComposerItem::readSettings( void )  { return true; }

bool CComposerItem::removeSettings( void )  { return true; }

bool CComposerItem::_writeXML( QDomElement& itemElem, QDomDocument& doc ) const
{
	if ( itemElem.isNull() )
	{
		return false;
	}

	QDomElement composerItemElem = doc.createElement( "ComposerItem" );

	//frame
	if ( mFrame )
	{
		composerItemElem.setAttribute( "frame", "true" );
	}
	else
	{
		composerItemElem.setAttribute( "frame", "false" );
	}

	//scene rect
	composerItemElem.setAttribute( "x", transform().dx() );
	composerItemElem.setAttribute( "y", transform().dy() );
	composerItemElem.setAttribute( "width", rect().width() );
	composerItemElem.setAttribute( "height", rect().height() );
	composerItemElem.setAttribute( "zValue", QString::number( zValue() ) );
	composerItemElem.setAttribute( "outlineWidth", QString::number( pen().widthF() ) );
	composerItemElem.setAttribute( "rotation", mRotation );

	//position lock for mouse moves/resizes
	if ( mItemPositionLocked )
	{
		composerItemElem.setAttribute( "positionLock", "true" );
	}
	else
	{
		composerItemElem.setAttribute( "positionLock", "false" );
	}

	composerItemElem.setAttribute( "lastValidViewScaleFactor", mLastValidViewScaleFactor );

	//frame color
	QDomElement frameColorElem = doc.createElement( "FrameColor" );
	QColor frameColor = pen().color();
	frameColorElem.setAttribute( "red", QString::number( frameColor.red() ) );
	frameColorElem.setAttribute( "green", QString::number( frameColor.green() ) );
	frameColorElem.setAttribute( "blue", QString::number( frameColor.blue() ) );
	frameColorElem.setAttribute( "alpha", QString::number( frameColor.alpha() ) );
	composerItemElem.appendChild( frameColorElem );

	//background color
	QDomElement bgColorElem = doc.createElement( "BackgroundColor" );
	QColor bgColor = brush().color();
	bgColorElem.setAttribute( "red", QString::number( bgColor.red() ) );
	bgColorElem.setAttribute( "green", QString::number( bgColor.green() ) );
	bgColorElem.setAttribute( "blue", QString::number( bgColor.blue() ) );
	bgColorElem.setAttribute( "alpha", QString::number( bgColor.alpha() ) );
	composerItemElem.appendChild( bgColorElem );

	itemElem.appendChild( composerItemElem );

	return true;
}
*/

bool CComposerItem::_readXML( const QDomElement& itemElem, const QDomDocument& doc )
{
	if ( itemElem.isNull() )
	{
		return false;
	}

	//rotation
	mRotation = itemElem.attribute( "rotation", "0" ).toDouble();

	//frame
	QString frame = itemElem.attribute( "frame" );
	if ( frame.compare( "true", Qt::CaseInsensitive ) == 0 )
	{
		mFrame = true;
	}
	else
	{
		mFrame = false;
	}

	//position lock for mouse moves/resizes
	QString positionLock = itemElem.attribute( "positionLock" );
	if ( positionLock.compare( "true", Qt::CaseInsensitive ) == 0 )
	{
		mItemPositionLocked = true;
	}
	else
	{
		mItemPositionLocked = false;
	}

	//position
	double x, y, width, height;
	bool xOk, yOk, widthOk, heightOk;

	x = itemElem.attribute( "x" ).toDouble( &xOk );
	y = itemElem.attribute( "y" ).toDouble( &yOk );
	width = itemElem.attribute( "width" ).toDouble( &widthOk );
	height = itemElem.attribute( "height" ).toDouble( &heightOk );
	templateHeight=height;
	if ( !xOk || !yOk || !widthOk || !heightOk )
	{
		return false;
	}

	mLastValidViewScaleFactor = itemElem.attribute( "lastValidViewScaleFactor", "-1" ).toDouble();

	setSceneRect( QRectF( x, y, width, height ) );
	setZValue( itemElem.attribute( "zValue" ).toDouble() );

	//pen
	QDomNodeList frameColorList = itemElem.elementsByTagName( "FrameColor" );
	if ( frameColorList.size() > 0 )
	{
		QDomElement frameColorElem = frameColorList.at( 0 ).toElement();
		bool redOk, greenOk, blueOk, alphaOk, widthOk;
		int penRed, penGreen, penBlue, penAlpha;
		double penWidth;

		penWidth = itemElem.attribute( "outlineWidth" ).toDouble( &widthOk );
		penRed = frameColorElem.attribute( "red" ).toDouble( &redOk );
		penGreen = frameColorElem.attribute( "green" ).toDouble( &greenOk );
		penBlue = frameColorElem.attribute( "blue" ).toDouble( &blueOk );
		penAlpha = frameColorElem.attribute( "alpha" ).toDouble( &alphaOk );
		if ( redOk && greenOk && blueOk && alphaOk && widthOk )
		{
			QPen framePen( QColor( penRed, penGreen, penBlue, penAlpha ) );
			framePen.setWidthF( penWidth );
			setPen( framePen );
		}
	}

	//brush
	QDomNodeList bgColorList = itemElem.elementsByTagName( "BackgroundColor" );
	if ( bgColorList.size() > 0 )
	{
		QDomElement bgColorElem = bgColorList.at( 0 ).toElement();
		bool redOk, greenOk, blueOk, alphaOk;
		int bgRed, bgGreen, bgBlue, bgAlpha;
		bgRed = bgColorElem.attribute( "red" ).toDouble( &redOk );
		bgGreen = bgColorElem.attribute( "green" ).toDouble( &greenOk );
		bgBlue = bgColorElem.attribute( "blue" ).toDouble( &blueOk );
		bgAlpha = bgColorElem.attribute( "alpha" ).toDouble( &alphaOk );
		if ( redOk && greenOk && blueOk && alphaOk )
		{
			QColor brushColor( bgRed, bgGreen, bgBlue, bgAlpha );
			setBrush( QBrush( brushColor ) );
		}
	}
	return true;
}


void CComposerItem::beginCommand( const QString& commandText, CipasComposerMergeCommand::Context c )
{
	if ( mComposition )
	{
		mComposition->beginCommand( this, commandText, c );
	}
}

void CComposerItem::endCommand()
{
	if ( mComposition )
	{
		mComposition->endCommand();
	}
}

void CComposerItem::cancelCommand()
{
	if ( mComposition )
	{
		mComposition->cancelCommand();
	}
}




bool CComposerItem::_writeXML( QDomElement& itemElem, QDomDocument& doc ) const
{
	if ( itemElem.isNull() )
	{
		return false;
	}

	QDomElement composerItemElem = doc.createElement( "ComposerItem" );

	//frame
	if ( mFrame )
	{
		composerItemElem.setAttribute( "frame", "true" );
	}
	else
	{
		composerItemElem.setAttribute( "frame", "false" );
	}

	//scene rect
	composerItemElem.setAttribute( "x", transform().dx() );
	composerItemElem.setAttribute( "y", transform().dy() );
	composerItemElem.setAttribute( "width", rect().width() );
	composerItemElem.setAttribute( "height", rect().height() );
	composerItemElem.setAttribute( "zValue", QString::number( zValue() ) );
	composerItemElem.setAttribute( "outlineWidth", QString::number( pen().widthF() ) );
	composerItemElem.setAttribute( "rotation", mRotation );

	//position lock for mouse moves/resizes
	if ( mItemPositionLocked )
	{
		composerItemElem.setAttribute( "positionLock", "true" );
	}
	else
	{
		composerItemElem.setAttribute( "positionLock", "false" );
	}

	composerItemElem.setAttribute( "lastValidViewScaleFactor", mLastValidViewScaleFactor );

	//frame color
	QDomElement frameColorElem = doc.createElement( "FrameColor" );
	QColor frameColor = pen().color();
	frameColorElem.setAttribute( "red", QString::number( frameColor.red() ) );
	frameColorElem.setAttribute( "green", QString::number( frameColor.green() ) );
	frameColorElem.setAttribute( "blue", QString::number( frameColor.blue() ) );
	frameColorElem.setAttribute( "alpha", QString::number( frameColor.alpha() ) );
	composerItemElem.appendChild( frameColorElem );

	//background color
	QDomElement bgColorElem = doc.createElement( "BackgroundColor" );
	QColor bgColor = brush().color();
	bgColorElem.setAttribute( "red", QString::number( bgColor.red() ) );
	bgColorElem.setAttribute( "green", QString::number( bgColor.green() ) );
	bgColorElem.setAttribute( "blue", QString::number( bgColor.blue() ) );
	bgColorElem.setAttribute( "alpha", QString::number( bgColor.alpha() ) );
	composerItemElem.appendChild( bgColorElem );

	itemElem.appendChild( composerItemElem );

	return true;
}

void CComposerItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	if ( mItemPositionLocked )
	{
		return;
	}

	if ( mBoundingResizeRectangle )
	{
		double diffX = event->lastScenePos().x() - mLastMouseEventPos.x();
		double diffY = event->lastScenePos().y() - mLastMouseEventPos.y();

		changeItemRectangle( event->lastScenePos(), mMouseMoveStartPos, this, diffX, diffY, mBoundingResizeRectangle );
	}
	mLastMouseEventPos = event->lastScenePos();
}

void CComposerItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	if ( mItemPositionLocked )
	{
		return;
	}

	//set current position and type of mouse move action
	mMouseMoveStartPos = event->lastScenePos();
	mLastMouseEventPos = event->lastScenePos();
	mCurrentMouseMoveAction = mouseMoveActionForPosition( event->pos() );

	//remove the old rubber band item if it is still there
	if ( mBoundingResizeRectangle )
	{
		scene()->removeItem( mBoundingResizeRectangle );
		delete mBoundingResizeRectangle;
		mBoundingResizeRectangle = 0;
	}
	//create and show bounding rectangle
	mBoundingResizeRectangle = new QGraphicsRectItem( 0 );
	scene()->addItem( mBoundingResizeRectangle );
	mBoundingResizeRectangle->setRect( QRectF( 0, 0, rect().width(), rect().height() ) );
	QTransform resizeTransform;
	resizeTransform.translate( transform().dx(), transform().dy() );
	mBoundingResizeRectangle->setTransform( resizeTransform );

	mBoundingResizeRectangle->setBrush( Qt::NoBrush );
	mBoundingResizeRectangle->setPen( QPen( QColor( 0, 0, 0 ), 0 ) );
	mBoundingResizeRectangle->setZValue( 90 );
	mBoundingResizeRectangle->show();
}

void CComposerItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	if ( mItemPositionLocked )
	{
		return;
	}

	//delete frame rectangle
	if ( mBoundingResizeRectangle )
	{
		scene()->removeItem( mBoundingResizeRectangle );
		delete mBoundingResizeRectangle;
		mBoundingResizeRectangle = 0;
	}

	QPointF mouseMoveStopPoint = event->lastScenePos();
	double diffX = mouseMoveStopPoint.x() - mMouseMoveStartPos.x();
	double diffY = mouseMoveStopPoint.y() - mMouseMoveStartPos.y();

	//it was only a click
	if ( qAbs( diffX ) < std::numeric_limits<double>::min() && qAbs( diffY ) < std::numeric_limits<double>::min() )
	{
		return;
	}

	//beginCommand( tr( "Change item position" ) );
	changeItemRectangle( mouseMoveStopPoint, mMouseMoveStartPos, this, diffX, diffY, this );
	//endCommand();

	//reset default action
	mCurrentMouseMoveAction = CComposerItem::MoveItem;
	setCursor( Qt::ArrowCursor );
}
////鼠标样式
Qt::CursorShape CComposerItem::cursorForPosition( const QPointF& itemCoordPos )
{
	CComposerItem::MouseMoveAction mouseAction = mouseMoveActionForPosition( itemCoordPos );

	if ( mouseAction == CComposerItem::NoAction )
	{
		return Qt::ForbiddenCursor;
	}
	if ( mouseAction == CComposerItem::MoveItem )
	{
		return Qt::ClosedHandCursor;
	}
	else if (mouseAction == CComposerItem::MoveMapContent)
	{
		return Qt::OpenHandCursor;
	}
	else if ( mouseAction == CComposerItem::ResizeLeftUp || mouseAction == CComposerItem::ResizeRightDown )
	{
		return Qt::SizeFDiagCursor;
	}
	else if ( mouseAction == CComposerItem::ResizeLeftDown || mouseAction == CComposerItem::ResizeRightUp )
	{
		return Qt::SizeBDiagCursor;
	}
	else if ( mouseAction == CComposerItem::ResizeUp || mouseAction == CComposerItem::ResizeDown )
	{
		return Qt::SizeVerCursor;
	}
	else //if(mouseAction == CComposerItem::ResizeLeft || mouseAction == CComposerItem::ResizeRight)
	{
		return Qt::SizeHorCursor;
	}
}
////鼠标样式
CComposerItem::MouseMoveAction CComposerItem::mouseMoveActionForPosition( const QPointF& itemCoordPos )
{
	//no action at all if item position is locked for mouse
	if ( mItemPositionLocked )
	{
		return CComposerItem::NoAction;
	}

	bool nearLeftBorder = false;
	bool nearRightBorder = false;
	bool nearLowerBorder = false;
	bool nearUpperBorder = false;

	double borderTolerance = rectHandlerBorderTolerance();

	if ( itemCoordPos.x() < borderTolerance )
	{
		nearLeftBorder = true;
	}
	if ( itemCoordPos.y() < borderTolerance )
	{
		nearUpperBorder = true;
	}
	if ( itemCoordPos.x() > ( rect().width() - borderTolerance ) )
	{
		nearRightBorder = true;
	}
	if ( itemCoordPos.y() > ( rect().height() - borderTolerance ) )
	{
		nearLowerBorder = true;
	}

	if ( nearLeftBorder && nearUpperBorder )
	{
		return CComposerItem::ResizeLeftUp;
	}
	else if ( nearLeftBorder && nearLowerBorder )
	{
		return CComposerItem::ResizeLeftDown;
	}
	else if ( nearRightBorder && nearUpperBorder )
	{
		return CComposerItem::ResizeRightUp;
	}
	else if ( nearRightBorder && nearLowerBorder )
	{
		return CComposerItem::ResizeRightDown;
	}
	else if ( nearLeftBorder )
	{
		return CComposerItem::ResizeLeft;
	}
	else if ( nearRightBorder )
	{
		return CComposerItem::ResizeRight;
	}
	else if ( nearUpperBorder )
	{
		return CComposerItem::ResizeUp;
	}
	else if ( nearLowerBorder )
	{
		return CComposerItem::ResizeDown;
	}
	return CComposerItem::MoveItem; //default
	////wuenping 20111207 修改鼠标样式	
	//if (type()==CComposerItem::ComposerGLW && /*移动地图内容*/)
	//{
	//	return CComposerItem::MoveMapContent;
	//} 
	//else
	//{
	//	return CComposerItem::MoveItem; //default
	//}
}

void CComposerItem::changeItemRectangle( const QPointF& currentPosition, const QPointF& mouseMoveStartPos, const QGraphicsRectItem* originalItem, double dx, double dy, QGraphicsRectItem* changeItem )
{
	if ( !changeItem || !originalItem || !mComposition )
	{
		return;
	}

	//test if change item is a composer item. If so, prefer call to  setSceneRect() instead of setTransform() and setRect()
	CComposerItem* changeComposerItem = dynamic_cast<CComposerItem *>( changeItem );

	double mx = 0.0, my = 0.0, rx = 0.0, ry = 0.0;
	QPointF snappedPosition = mComposition->snapPointToGrid( currentPosition );
	//double diffX = snappedPosition.x() - mouseMoveStartPos.x();
	//double diffY = snappedPosition.y() - mouseMoveStartPos.y();
	double diffX = 0;
	double diffY = 0;

	switch ( mCurrentMouseMoveAction )
	{
		//vertical resize
	case CComposerItem::ResizeUp:
		diffY = snappedPosition.y() - originalItem->transform().dy();
		mx = 0; my = diffY; rx = 0; ry = -diffY;
		break;

	case CComposerItem::ResizeDown:
		diffY = snappedPosition.y() - ( originalItem->transform().dy() + originalItem->rect().height() );
		mx = 0; my = 0; rx = 0; ry = diffY;
		break;

		//horizontal resize
	case CComposerItem::ResizeLeft:
		diffX = snappedPosition.x() - originalItem->transform().dx();
		mx = diffX, my = 0; rx = -diffX; ry = 0;
		break;

	case CComposerItem::ResizeRight:
		diffX = snappedPosition.x() - ( originalItem->transform().dx() + originalItem->rect().width() );
		mx = 0; my = 0; rx = diffX, ry = 0;
		break;

		//diagonal resize
	case CComposerItem::ResizeLeftUp:
		diffX = snappedPosition.x() - originalItem->transform().dx();
		diffY = snappedPosition.y() - originalItem->transform().dy();
		mx = diffX, my = diffY; rx = -diffX; ry = -diffY;
		break;

	case CComposerItem::ResizeRightDown:
		diffX = snappedPosition.x() - ( originalItem->transform().dx() + originalItem->rect().width() );
		diffY = snappedPosition.y() - ( originalItem->transform().dy() + originalItem->rect().height() );
		mx = 0; my = 0; rx = diffX, ry = diffY;
		break;

	case CComposerItem::ResizeRightUp:
		diffX = snappedPosition.x() - ( originalItem->transform().dx() + originalItem->rect().width() );
		diffY = snappedPosition.y() - originalItem->transform().dy();
		mx = 0; my = diffY, rx = diffX, ry = -diffY;
		break;

	case CComposerItem::ResizeLeftDown:
		diffX = snappedPosition.x() - originalItem->transform().dx();
		diffY = snappedPosition.y() - ( originalItem->transform().dy() + originalItem->rect().height() );
		mx = diffX, my = 0; rx = -diffX; ry = diffY;
		break;

	case CComposerItem::MoveItem:
		{
			//calculate total move difference
			double moveX = currentPosition.x() - mouseMoveStartPos.x();
			double moveY = currentPosition.y() - mouseMoveStartPos.y();

			QPointF upperLeftPoint( originalItem->transform().dx() + moveX, originalItem->transform().dy() + moveY );
			QPointF snappedLeftPoint = mComposition->snapPointToGrid( upperLeftPoint );

			double moveRectX = snappedLeftPoint.x() - originalItem->transform().dx();
			double moveRectY = snappedLeftPoint.y() - originalItem->transform().dy();

			if ( !changeComposerItem )
			{
				QTransform moveTransform;
				moveTransform.translate( originalItem->transform().dx() + moveRectX, originalItem->transform().dy() + moveRectY );
				changeItem->setTransform( moveTransform );
			}
			else  //for composer items, we prefer setSceneRect as subclasses can implement custom behaviour (e.g. item group)
			{
				changeComposerItem->setSceneRect( QRectF( originalItem->transform().dx() + moveRectX,
					originalItem->transform().dy() + moveRectY,
					originalItem->rect().width(), originalItem->rect().height() ) );
			}
		}
		return;
	case CComposerItem::NoAction:
		break;
	}

	if ( !changeComposerItem )
	{
		QTransform itemTransform;
		itemTransform.translate( originalItem->transform().dx() + mx, originalItem->transform().dy() + my );
		changeItem->setTransform( itemTransform );
		QRectF itemRect( 0, 0, originalItem->rect().width() + rx,  originalItem->rect().height() + ry );
		changeItem->setRect( itemRect );
	}
	else //for composer items, we prefer setSceneRect as subclasses can implement custom behaviour (e.g. item group)
	{
		changeComposerItem->setSceneRect( QRectF( originalItem->transform().dx() + mx, originalItem->transform().dy() + my,
			originalItem->rect().width() + rx, originalItem->rect().height() + ry ) );
	}
}

void CComposerItem::drawSelectionBoxes( QPainter* p )
{
	if ( !mComposition )
	{
		return;
	}

	if ( mComposition->plotStyle() == CCipasComposition::Preview )
	{
		//size of symbol boxes depends on zoom level in composer view
		double rectHandlerSize = rectHandlerBorderTolerance();
		double sizeLockSymbol = lockSymbolSize();

		if ( mItemPositionLocked )
		{
				//draw lock symbol at upper left edge. Use QImage to be independent of the graphic system
				// QString lockIconPath = QgsApplication::activeThemePath() + "/mIconLock.png";
				QString lockIconPath= IDPApplication::Path()+"/themes/default/"+ "mIconLock.png";
				if ( !QFile::exists( lockIconPath ) )
				{
					//lockIconPath = QgsApplication::defaultThemePath() + "/mIconLock.png";
					int vvv=100;
				}
			QImage lockImage( lockIconPath );
			if ( !lockImage.isNull() )
			{
				p->drawImage( QRectF( 0, 0, sizeLockSymbol, sizeLockSymbol ), lockImage, QRectF( 0, 0, lockImage.width(), lockImage.height() ) );
			}
		}
		else //draw blue squares
		{
			//p->setPen( QPen(QColor( 176, 255, 255, 200 )));
			p->setPen(Qt::DashLine);

			//p->drawRect( QRectF(0,0,rect().width(),rect().height()));
			QPainterPath path;
			path.moveTo(0,0);
			path.lineTo(0,rect().height());
			path.moveTo(0,rect().height());
			path.lineTo(rect().width(),rect().height());
			path.moveTo(rect().width(),rect().height());
			path.lineTo(rect().width(),0);
			path.moveTo(rect().width(),0);
			path.lineTo(0,0);
			p->drawPath(path);
			if (true) //对于可以修改的项，绘制四个小矩形 如插入图形，图例不加
			{
				p->setBrush( QColor( 176, 255, 255, 255 ) );
				p->setPen(Qt::SolidLine);
				p->drawRect( QRectF( -0.5*rectHandlerSize, -0.5*rectHandlerSize, rectHandlerSize, rectHandlerSize ) );
				p->drawRect( QRectF( rect().width() - 0.5*rectHandlerSize, -0.5*rectHandlerSize, rectHandlerSize, rectHandlerSize ) );
				p->drawRect( QRectF( rect().width() - 0.5*rectHandlerSize, rect().height() - 0.5*rectHandlerSize, rectHandlerSize, rectHandlerSize ) );
				p->drawRect( QRectF( -0.5*rectHandlerSize, rect().height() - 0.5*rectHandlerSize, rectHandlerSize, rectHandlerSize ) );
			}
			
		}
	}
}

void CComposerItem::drawFrame( QPainter* p )
{
	if ( mFrame && p )
	{
		p->setPen( pen() );
		p->setBrush( Qt::NoBrush );
		p->setRenderHint( QPainter::Antialiasing, true );
		p->drawRect( QRectF( 0, 0, rect().width(), rect().height() ) );
	}
}

void CComposerItem::move( double dx, double dy )
{
	QTransform t = transform();
	QRectF newSceneRect( t.dx() + dx, t.dy() + dy, rect().width(), rect().height() );
	setSceneRect( newSceneRect );
}

void CComposerItem::setItemPosition( double x, double y, ItemPositionMode itemPoint )
{
	double width = rect().width();
	double height = rect().height();
	setItemPosition( x, y, width, height, itemPoint );
}

void CComposerItem::setItemPosition( double x, double y, double width, double height, ItemPositionMode itemPoint )
{
	double upperLeftX = x;
	double upperLeftY = y;

	//adjust x-coordinate if placement is not done to a left point
	if ( itemPoint == UpperMiddle || itemPoint == Middle || itemPoint == LowerMiddle )
	{
		upperLeftX -= width / 2.0;
	}
	else if ( itemPoint == UpperRight || itemPoint == MiddleRight || itemPoint == LowerRight )
	{
		upperLeftX -= width;
	}

	//adjust y-coordinate if placement is not done to an upper point
	if ( itemPoint == MiddleLeft || itemPoint == Middle || itemPoint == MiddleRight )
	{
		upperLeftY -= height / 2.0;
	}
	else if ( itemPoint == LowerLeft || itemPoint == LowerMiddle || itemPoint == LowerRight )
	{
		upperLeftY -= height;
	}

	setSceneRect( QRectF( upperLeftX, upperLeftY, width, height ) );
}

void CComposerItem::setSceneRect( const QRectF& rectangle )
{
	//setRect in item coordinates
	double newWidth = rectangle.width();
	double newHeight = rectangle.height();
	double xTranslation = rectangle.x();
	double yTranslation = rectangle.y();

	//correction if width and/or height are negative
	if ( rectangle.width() < 0 )
	{
		newWidth = - rectangle.width();
		xTranslation -= newWidth;
	}

	if ( rectangle.height() < 0 )
	{
		newHeight = - rectangle.height();
		yTranslation -= newHeight;
	}

	QRectF newRect( 0, 0, newWidth, newHeight );
	QGraphicsRectItem::setRect( newRect );

	//set up transformation matrix for item coordinates
	QTransform t;
	t.translate( xTranslation, yTranslation );
	setTransform( t );
}

void CComposerItem::drawBackground( QPainter* p )
{
	if ( p )
	{
		p->setBrush( brush() );
		p->setPen( Qt::NoPen );
		p->setRenderHint( QPainter::Antialiasing, true );
		if (this->type()==ItemType::ComposerPaper)
		{
			p->drawRect( QRectF( 0, 0, rect().width()+0.3, rect().height()) );
			//ComposerItem 背景宽度+0.3 掩盖出图右侧有边线的bug，暂时方案 20111219 吴恩平
		}
		else
		{
			p->drawRect( QRectF( 0, 0, rect().width(), rect().height()) );
		}
	}
}

void CComposerItem::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
	if ( isSelected() )
	{
		setCursor( cursorForPosition( event->pos() ) );
	}
}

void CComposerItem::drawText( QPainter* p, double x, double y, const QString& text, const QFont& font ) const
{
	QFont textFont = scaledFontPixelSize( font );

	p->save();
	p->setFont( textFont );
	//p->setPen( QColor( 0, 0, 0 ) ); //draw text always in black
	double scaleFactor = 1.0 / FONT_WORKAROUND_SCALE;
	p->scale( scaleFactor, scaleFactor );
	p->drawText( QPointF( x * FONT_WORKAROUND_SCALE, y * FONT_WORKAROUND_SCALE ), text );
	p->restore();
}

void CComposerItem::drawText( QPainter* p, const QRectF& rect, const QString& text, const QFont& font, Qt::AlignmentFlag halignement, Qt::AlignmentFlag valignment ) const
{
	QFont textFont = scaledFontPixelSize( font );

	QRectF scaledRect( rect.x() * FONT_WORKAROUND_SCALE, rect.y() * FONT_WORKAROUND_SCALE,
		rect.width() * FONT_WORKAROUND_SCALE, rect.height() * FONT_WORKAROUND_SCALE );

	p->save();
	p->setFont( textFont );
	double scaleFactor = 1.0 / FONT_WORKAROUND_SCALE;
	p->scale( scaleFactor, scaleFactor );
	p->drawText( scaledRect, halignement | valignment | Qt::TextWordWrap, text );
	p->restore();
}
void CComposerItem::drawArrowHead( QPainter* p, double x, double y, double angle, double arrowHeadWidth ) const
{
	if ( !p )
	{
		return;
	}
	double angleRad = angle / 180.0 * M_PI;
	QPointF middlePoint( x, y );
	//rotate both arrow points
	QPointF p1 = QPointF( -arrowHeadWidth / 2.0, arrowHeadWidth );
	QPointF p2 = QPointF( arrowHeadWidth / 2.0, arrowHeadWidth );

	QPointF p1Rotated, p2Rotated;
	p1Rotated.setX( p1.x() * cos( angleRad ) + p1.y() * -sin( angleRad ) );
	p1Rotated.setY( p1.x() * sin( angleRad ) + p1.y() * cos( angleRad ) );
	p2Rotated.setX( p2.x() * cos( angleRad ) + p2.y() * -sin( angleRad ) );
	p2Rotated.setY( p2.x() * sin( angleRad ) + p2.y() * cos( angleRad ) );

	QPolygonF arrowHeadPoly;
	arrowHeadPoly << middlePoint;
	arrowHeadPoly << QPointF( middlePoint.x() + p1Rotated.x(), middlePoint.y() + p1Rotated.y() );
	arrowHeadPoly << QPointF( middlePoint.x() + p2Rotated.x(), middlePoint.y() + p2Rotated.y() );

	p->save();

	QPen arrowPen = p->pen();
	arrowPen.setJoinStyle( Qt::RoundJoin );
	QBrush arrowBrush = p->brush();
	arrowBrush.setStyle( Qt::SolidPattern );
	p->setPen( arrowPen );
	p->setBrush( arrowBrush );
	arrowBrush.setStyle( Qt::SolidPattern );
	p->drawPolygon( arrowHeadPoly );

	p->restore();
}

double CComposerItem::textWidthMillimeters( const QFont& font, const QString& text ) const
{
	QFont metricsFont = scaledFontPixelSize( font );
	QFontMetrics fontMetrics( metricsFont );
	return ( fontMetrics.width( text ) / FONT_WORKAROUND_SCALE );
}

double CComposerItem::textHeightMillimeters( const QFont& font, const QString& text ) const
{
	QFont metricsFont = scaledFontPixelSize( font );
	QFontMetrics fontMetrics( metricsFont );
	return ( fontMetrics.height() / FONT_WORKAROUND_SCALE );
}

double CComposerItem::fontHeightCharacterMM( const QFont& font, const QChar& c ) const
{
	QFont metricsFont = scaledFontPixelSize( font );
	QFontMetricsF fontMetrics( metricsFont );
	return ( fontMetrics.boundingRect( c ).height() / FONT_WORKAROUND_SCALE );
}

double CComposerItem::fontAscentMillimeters( const QFont& font ) const
{
	QFont metricsFont = scaledFontPixelSize( font );
	QFontMetricsF fontMetrics( metricsFont );
	double df=fontMetrics.ascent() / FONT_WORKAROUND_SCALE;
	return ( fontMetrics.ascent() / FONT_WORKAROUND_SCALE );
}

double CComposerItem::pixelFontSize( double pointSize ) const
{
	return ( pointSize * 0.3527 );
}

QFont CComposerItem::scaledFontPixelSize( const QFont& font ) const
{
	QFont scaledFont = font;
	double fd=font.pointSizeF();
	double pixelSize = pixelFontSize( font.pointSizeF() ) * FONT_WORKAROUND_SCALE + 0.5;
	scaledFont.setPixelSize( pixelSize );
	return scaledFont;
}

double CComposerItem::angle( const QPointF& p1, const QPointF& p2 ) const
{
	double xDiff = p2.x() - p1.x();
	double yDiff = p2.y() - p1.y();
	double length = sqrt( xDiff * xDiff + yDiff * yDiff );
	if ( length <= 0 )
	{
		return 0;
	}

	double angle = acos(( -yDiff * length ) / ( length * length ) ) * 180 / M_PI;
	if ( xDiff < 0 )
	{
		return ( 360 - angle );
	}
	return angle;
}

double CComposerItem::horizontalViewScaleFactor() const
{
	double result = -1;
	if ( scene() )
	{
		QList<QGraphicsView*> viewList = scene()->views();
		if ( viewList.size() > 0 ) //if not, probably this function was called from non-gui code
		{
			QGraphicsView* currentView = viewList.at( 0 );
			if ( currentView->isVisible() )
			{
				result = currentView->transform().m11();
				mLastValidViewScaleFactor = result;
			}
		}
	}
	return result;
}

double CComposerItem::rectHandlerBorderTolerance() const
{
	//size of symbol boxes depends on zoom level in composer view
	double viewScaleFactor = horizontalViewScaleFactor();
	double rectHandlerSize = 10.0 / viewScaleFactor;

	//make sure the boxes don't get too large
	if ( rectHandlerSize > ( rect().width() / 4 ) )
	{
		rectHandlerSize = rect().width() / 4;
	}
	if ( rectHandlerSize > ( rect().height() / 4 ) )
	{
		rectHandlerSize = rect().height() / 4;
	}
	return rectHandlerSize/2;
}

double CComposerItem::lockSymbolSize() const
{
	double lockSymbolSize = 20.0 / horizontalViewScaleFactor();

	if ( lockSymbolSize > ( rect().width() / 3 ) )
	{
		lockSymbolSize = rect().width() / 3;
	}
	if ( lockSymbolSize > ( rect().height() / 3 ) )
	{
		lockSymbolSize = rect().height() / 3;
	}
	return lockSymbolSize;
}

void CComposerItem::updateCursor( const QPointF& itemPos )
{
	setCursor( cursorForPosition( itemPos ) );
}

void CComposerItem::setRotation( double r )
{
	if ( r > 360 )
	{
		mRotation = (( int )r ) % 360;
	}
	else
	{
		mRotation = r;
	}
	emit rotationChanged( r );
	update();
}

bool CComposerItem::imageSizeConsideringRotation( double& width, double& height ) const
{
	if ( qAbs( mRotation ) <= 0.0 ) //width and height stays the same if there is no rotation
	{
		return true;
	}

	double x1 = 0;
	double y1 = 0;
	double x2 = width;
	double y2 = 0;
	double x3 = width;
	double y3 = height;
	double x4 = 0;
	double y4 = height;
	double midX = width / 2.0;
	double midY = height / 2.0;

	if ( !cornerPointOnRotatedAndScaledRect( x1, y1, width, height ) )
	{
		return false;
	}
	if ( !cornerPointOnRotatedAndScaledRect( x2, y2, width, height ) )
	{
		return false;
	}
	if ( !cornerPointOnRotatedAndScaledRect( x3, y3, width, height ) )
	{
		return false;
	}
	if ( !cornerPointOnRotatedAndScaledRect( x4, y4, width, height ) )
	{
		return false;
	}

	//assume points 1 and 3 are on the rectangle boundaries. Calculate 2 and 4.
	double distM1 = sqrt(( x1 - midX ) * ( x1 - midX ) + ( y1 - midY ) * ( y1 - midY ) );
	QPointF p2 = pointOnLineWithDistance( QPointF( midX, midY ), QPointF( x2, y2 ), distM1 );

	if ( p2.x() < width && p2.x() > 0 && p2.y() < height && p2.y() > 0 )
	{
		width = sqrt(( p2.x() - x1 ) * ( p2.x() - x1 ) + ( p2.y() - y1 ) * ( p2.y() - y1 ) );
		height = sqrt(( x3 - p2.x() ) * ( x3 - p2.x() ) + ( y3 - p2.y() ) * ( y3 - p2.y() ) );
		return true;
	}

	//else assume that points 2 and 4 are on the rectangle boundaries. Calculate 1 and 3
	double distM2 = sqrt(( x2 - midX ) * ( x2 - midX ) + ( y2 - midY ) * ( y2 - midY ) );
	QPointF p1 = pointOnLineWithDistance( QPointF( midX, midY ), QPointF( x1, y1 ), distM2 );
	QPointF p3 = pointOnLineWithDistance( QPointF( midX, midY ), QPointF( x3, y3 ), distM2 );
	width = sqrt(( x2 - p1.x() ) * ( x2 - p1.x() ) + ( y2 - p1.y() ) * ( y2 - p1.y() ) );
	height = sqrt(( p3.x() - x2 ) * ( p3.x() - x2 ) + ( p3.y() - y2 ) * ( p3.y() - y2 ) );
	return true;

#if 0
	double x1 = 0;
	double y1 = 0;
	double x2 = width;
	double y2 = 0;
	double x3 = width;
	double y3 = height;

	if ( !cornerPointOnRotatedAndScaledRect( x1, y1, width, height ) )
	{
		return false;
	}
	if ( !cornerPointOnRotatedAndScaledRect( x2, y2, width, height ) )
	{
		return false;
	}
	if ( !cornerPointOnRotatedAndScaledRect( x3, y3, width, height ) )
	{
		return false;
	}

	width = sqrt(( x2 - x1 ) * ( x2 - x1 ) + ( y2 - y1 ) * ( y2 - y1 ) );
	height = sqrt(( x3 - x2 ) * ( x3 - x2 ) + ( y3 - y2 ) * ( y3 - y2 ) );
	return true;
#endif //0
}

bool CComposerItem::cornerPointOnRotatedAndScaledRect( double& x, double& y, double width, double height ) const
{
	//first rotate point clockwise
	double rotToRad = mRotation * M_PI / 180.0;
	QPointF midpoint( width / 2.0, height / 2.0 );
	double xVector = x - midpoint.x();
	double yVector = y - midpoint.y();
	//double xRotated = cos(rotToRad) * xVector + sin(rotToRad) * yVector;
	//double yRotated = -sin(rotToRad) * xVector + cos(rotToRad) * yVector;
	double xRotated = cos( rotToRad ) * xVector - sin( rotToRad ) * yVector;
	double yRotated = sin( rotToRad ) * xVector + cos( rotToRad ) * yVector;

	//create line from midpoint to rotated point
	QLineF line( midpoint.x(), midpoint.y(), midpoint.x() + xRotated, midpoint.y() + yRotated );

	//intersect with all four borders and return result
	QList<QLineF> borders;
	borders << QLineF( 0, 0, width, 0 );
	borders << QLineF( width, 0, width, height );
	borders << QLineF( width, height, 0, height );
	borders << QLineF( 0, height, 0, 0 );

	QList<QLineF>::const_iterator it = borders.constBegin();
	QPointF intersectionPoint;

	for ( ; it != borders.constEnd(); ++it )
	{
		if ( line.intersect( *it, &intersectionPoint ) == QLineF::BoundedIntersection )
		{
			x = intersectionPoint.x();
			y = intersectionPoint.y();
			return true;
		}
	}
	return false;
}

QPointF CComposerItem::pointOnLineWithDistance( const QPointF& startPoint, const QPointF& directionPoint, double distance ) const
{
	double dx = directionPoint.x() - startPoint.x();
	double dy = directionPoint.y() - startPoint.y();
	double length = sqrt( dx * dx + dy * dy );
	double scaleFactor = distance / length;
	return QPointF( startPoint.x() + dx * scaleFactor, startPoint.y() + dy * scaleFactor );
}

void CComposerItem::sizeChangedByRotation( double& width, double& height )
{
	if ( mRotation == 0.0 )
	{
		return;
	}

	//vector to p1
	double x1 = -width / 2.0;
	double y1 = -height / 2.0;
	rotate( mRotation, x1, y1 );
	//vector to p2
	double x2 = width / 2.0;
	double y2 = -height / 2.0;
	rotate( mRotation, x2, y2 );
	//vector to p3
	double x3 = width / 2.0;
	double y3 = height / 2.0;
	rotate( mRotation, x3, y3 );
	//vector to p4
	double x4 = -width / 2.0;
	double y4 = height / 2.0;
	rotate( mRotation, x4, y4 );

	//double midpoint
	QPointF midpoint( width / 2.0, height / 2.0 );

	QPolygonF rotatedRectPoly;
	rotatedRectPoly << QPointF( midpoint.x() + x1, midpoint.y() + y1 );
	rotatedRectPoly << QPointF( midpoint.x() + x2, midpoint.y() + y2 );
	rotatedRectPoly << QPointF( midpoint.x() + x3, midpoint.y() + y3 );
	rotatedRectPoly << QPointF( midpoint.x() + x4, midpoint.y() + y4 );
	QRectF boundingRect = rotatedRectPoly.boundingRect();
	width = boundingRect.width();
	height = boundingRect.height();
}

void CComposerItem::rotate( double angle, double& x, double& y ) const
{
	double rotToRad = angle * M_PI / 180.0;
	double xRot, yRot;
	xRot = x * cos( rotToRad ) - y * sin( rotToRad );
	yRot = x * sin( rotToRad ) + y * cos( rotToRad );
	x = xRot;
	y = yRot;
}

void CComposerItem::repaint()
{
	update();
}