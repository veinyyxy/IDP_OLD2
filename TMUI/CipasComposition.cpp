#include "CipasPaperItem.h"
#include "Cipasitemcommand.h"
#include "CipasComposerLabel.h"
#include "CipasComposerGLW.h"
#include "CipasComposerMap.h"

#include "CipasComposition.h"

CCipasComposition::CCipasComposition(void):QGraphicsScene( 0 )
	, mPlotStyle( CCipasComposition::Preview )
	, mPaperItem( 0 )
	, mPrintAsRaster( false )
	, mSnapToGrid( false )
	,mSnapGridResolution( 10.0)
	, mSnapGridOffsetX( 0.0 )
	, mSnapGridOffsetY( 0.0 )
	, mLineNum(1)
	, mLineInv(1)
	, mdMarginX(2.0)
	, mdMarginY(2.0)
	, mbIsDrawBounding(false)//²»»æÖÆframe
	, mActiveCommand( 0 )
{
	
	setBackgroundBrush( Qt::gray );

	//set paper item
	mPaperItem = new CCipasPaperItem( 0, 0, 297, 210, this ); //default size A4
	mPaperItem->setBrush( Qt::white );
	addItem( mPaperItem );
	mPaperItem->setZValue( 0 );
	mPrintResolution =96; 
	loadGridAppearanceSettings();
	/*m_bgkImage.load(IDPApplication::TempPath()+"Thematic.png");*/
	
	//QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(m_bgkImage.scaled(width(),height(),Qt::IgnoreAspectRatio)));
	//addItem(pixmapItem);
}


CCipasComposition::~CCipasComposition(void)
{
	delete mPaperItem;
	clear();
}

void CCipasComposition::setPaperSize( double width, double height )
{
	if ( mPaperItem )
	{
		mPaperItem->setRect( QRectF( 0, 0, width, height ) );
		emit paperSizeChanged();
	}
}

double CCipasComposition::paperHeight() const
{
	return mPaperItem->rect().height();
}

double CCipasComposition::paperWidth() const
{
	return mPaperItem->rect().width();
}

CComposerItem* CCipasComposition::composerItemAt( const QPointF & position )
{
	QList<QGraphicsItem *> itemList = items( position );
	QList<QGraphicsItem *>::iterator itemIt = itemList.begin();

	for ( ; itemIt != itemList.end(); ++itemIt )
	{
		CComposerItem* composerItem = dynamic_cast<CComposerItem *>( *itemIt );
		if ( composerItem && composerItem != mPaperItem )
		{
			return composerItem;
		}
	}
	return 0;
}


QList<CComposerItem*> CCipasComposition::inputComposerGLWs()
{
	QList<CComposerItem*> composerItemList;

	QList<QGraphicsItem *> graphicsItemList = selectedItems();
	QList<QGraphicsItem *>::iterator itemIter = graphicsItemList.begin();

	for ( ; itemIter != graphicsItemList.end();itemIter++ )
	{
		CComposerItem* composerItem = dynamic_cast<CComposerItem *>( *itemIter );
		if ( composerItem->type()==CComposerItem::ItemType::ComposerGLW )
		{
			composerItemList.push_back( composerItem );
		}
	}
	return composerItemList;
}

QList<CComposerItem*> CCipasComposition::selectedComposerItems()
{
	QList<CComposerItem*> composerItemList;

	QList<QGraphicsItem *> graphicsItemList = selectedItems();
	QList<QGraphicsItem *>::iterator itemIter = graphicsItemList.begin();

	for ( ; itemIter != graphicsItemList.end(); ++itemIter )
	{
		CComposerItem* composerItem = dynamic_cast<CComposerItem *>( *itemIter );
		if ( composerItem->type()!=CComposerItem::ItemType::ComposerLegend )
		{
			composerItemList.push_back( composerItem );
		}
	}
	itemIter = graphicsItemList.begin();
	for ( ; itemIter != graphicsItemList.end(); ++itemIter )
	{
		CComposerItem* composerItem = dynamic_cast<CComposerItem *>( *itemIter );
		if ( composerItem->type()==CComposerItem::ItemType::ComposerLegend )
		{
			composerItemList.push_back( composerItem );
		}
	}
	return composerItemList;
}


QList<const CCipasComposerMap*> CCipasComposition::composerMapItems() const
{
	QList<const CCipasComposerMap*> resultList;

	QList<QGraphicsItem *> itemList = items();
	QList<QGraphicsItem *>::iterator itemIt = itemList.begin();
	for ( ; itemIt != itemList.end(); ++itemIt )
	{
		const CCipasComposerMap* composerMap = dynamic_cast<const CCipasComposerMap *>( *itemIt );
		if ( composerMap )
		{
			resultList.push_back( composerMap );
		}
	}

	return resultList;
}

const CCipasComposerMap* CCipasComposition::getComposerMapById( int id ) const
{
	QList<const CCipasComposerMap*> resultList;

	QList<QGraphicsItem *> itemList = items();
	QList<QGraphicsItem *>::iterator itemIt = itemList.begin();
	for ( ; itemIt != itemList.end(); ++itemIt )
	{
		const CCipasComposerMap* composerMap = dynamic_cast<const CCipasComposerMap *>( *itemIt );
		if ( composerMap )
		{
			if ( composerMap->id() == id )
			{
				return composerMap;
			}
		}
	}

	return 0;
}


QList<const CCipasComposerGLW*> CCipasComposition::composerGLWItems() const
{
	QList<const CCipasComposerGLW*> resultList;

	QList<QGraphicsItem *> itemList = items();
	QList<QGraphicsItem *>::iterator itemIt = itemList.begin();
	for ( ; itemIt != itemList.end(); ++itemIt )
	{
		const CCipasComposerGLW* composerGLW = dynamic_cast<const CCipasComposerGLW *>( *itemIt );
		if ( composerGLW )
		{
			resultList.push_back( composerGLW );
		}
	}

	return resultList;
}


const CCipasComposerGLW* CCipasComposition::getComposerGLWById( int id ) const
{
	QList<const CCipasComposerGLW*> resultList;

	QList<QGraphicsItem *> itemList = items();
	QList<QGraphicsItem *>::iterator itemIt = itemList.begin();
	for ( ; itemIt != itemList.end(); ++itemIt )
	{
		const CCipasComposerGLW* composerGLW = dynamic_cast<const CCipasComposerGLW *>( *itemIt );
		if ( composerGLW )
		{
			if ( composerGLW->id() == id )
			{
				return composerGLW;
			}
		}
	}
	return 0;
}


QList<const CCipasComposerLabel*> CCipasComposition::composerLabelItems() const
{
	QList<const CCipasComposerLabel*> resultList;

	QList<QGraphicsItem *> itemList = items();
	QList<QGraphicsItem *>::iterator itemIt = itemList.begin();
	for ( ; itemIt != itemList.end(); ++itemIt )
	{
		const CCipasComposerLabel* composerLabel = dynamic_cast<const CCipasComposerLabel *>( *itemIt );
		if ( composerLabel )
		{
			resultList.push_back(composerLabel);
		}
	}

	return resultList;
}

const CCipasComposerLabel* CCipasComposition::getComposerLabelById( QString id ) const
{
	QList<const CCipasComposerLabel*> resultList;

	QList<QGraphicsItem *> itemList = items();
	QList<QGraphicsItem *>::iterator itemIt = itemList.begin();
	for ( ; itemIt != itemList.end(); ++itemIt )
	{
		const CCipasComposerLabel* composerLabel = dynamic_cast<const CCipasComposerLabel *>( *itemIt );
		if ( composerLabel )
		{
			if ( composerLabel->id() == id )
			{
				return composerLabel;
			}
		}
	}
	return 0;
}


int CCipasComposition::pixelFontSize( double pointSize ) const
{
	//in CCipasComposition, one unit = one mm
	double sizeMillimeters = pointSize * 0.3527;
	return ( sizeMillimeters + 0.5 ); //round to nearest mm
}

double CCipasComposition::pointFontSize( int pixelSize ) const
{
	double sizePoint = pixelSize / 0.3527;
	return sizePoint;
}

bool CCipasComposition::writeXML( QDomElement& composerElem, QDomDocument& doc )
{
	if ( composerElem.isNull() )
	{
		return false;
	}

	QDomElement compositionElem = doc.createElement( "Composition" );
	if ( mPaperItem )
	{
		compositionElem.setAttribute( "paperWidth", mPaperItem->rect().width() );
		compositionElem.setAttribute( "paperHeight", mPaperItem->rect().height() );
	}

	//snapping
	if ( mSnapToGrid )
	{
		compositionElem.setAttribute( "snapping", "1" );
	}
	else
	{
		compositionElem.setAttribute( "snapping", "0" );
	}
	compositionElem.setAttribute( "snapGridResolution", mSnapGridResolution );
	compositionElem.setAttribute( "snapGridOffsetX", mSnapGridOffsetX );
	compositionElem.setAttribute( "snapGridOffsetY", mSnapGridOffsetY );

	compositionElem.setAttribute( "printResolution", mPrintResolution );
	compositionElem.setAttribute( "printAsRaster", mPrintAsRaster );

	//data frame line 
	compositionElem.setAttribute( "pageMargin", mbIsDrawBounding );
	compositionElem.setAttribute( "MarginX", mdMarginX );
	compositionElem.setAttribute( "MarginY", mdMarginY );


	composerElem.appendChild( compositionElem );

	return true;
}


bool CCipasComposition::readXML( const QDomElement& compositionElem, const QDomDocument& doc )
{
	if ( compositionElem.isNull() )
	{
		return false;
	}

	//create paper item
	bool widthConversionOk, heightConversionOk;
	double paperWidth = compositionElem.attribute( "paperWidth" ).toDouble( &widthConversionOk );
	double paperHeight = compositionElem.attribute( "paperHeight" ).toDouble( &heightConversionOk );

	if ( widthConversionOk && heightConversionOk )
	{
		delete mPaperItem;
		mPaperItem = new CCipasPaperItem( 0, 0, paperWidth, paperHeight, this );
		mPaperItem->setBrush( Qt::white );
		addItem( mPaperItem );
		mPaperItem->setZValue( 0 );
	}

	//snapping
	if ( compositionElem.attribute( "snapping" ) == "0" )
	{
		mSnapToGrid = false;
	}
	else
	{
		mSnapToGrid = true;
	}
	mSnapGridResolution = compositionElem.attribute( "snapGridResolution" ,"10").toDouble();
	mSnapGridOffsetX = compositionElem.attribute( "snapGridOffsetX" ).toDouble();
	mSnapGridOffsetY = compositionElem.attribute( "snapGridOffsetY" ).toDouble();
	mPrintAsRaster = compositionElem.attribute( "printAsRaster" ).toInt();

	mPrintResolution = compositionElem.attribute( "printResolution", "96" ).toInt();
	// data frame line

	if (compositionElem.attribute("pageMargin")=="0")
	{
		mbIsDrawBounding=false;
	} 
	else
	{
		mbIsDrawBounding=true;
	}

	mdMarginX=compositionElem.attribute( "MarginX" ).toDouble();
	mdMarginY=compositionElem.attribute( "MarginY" ).toDouble();

	if ( mPaperItem )
	{
		mPaperItem->update();
	}

	return true;
}

void CCipasComposition::addItemToZList( CComposerItem* item )
{
	if ( !item )
	{
		return;
	}
	mItemZList.push_back( item );
	//QgsDebugMsg( QString::number( mItemZList.size() ) );
	item->setZValue( mItemZList.size() );
}

void CCipasComposition::removeItemFromZList( CComposerItem* item )
{
	if ( !item )
	{
		return;
	}
	mItemZList.removeAll( item );
}

void CCipasComposition::raiseSelectedItems()
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	QList<CComposerItem*>::iterator it = selectedItems.begin();
	for ( ; it != selectedItems.end(); ++it )
	{
		raiseItem( *it );
	}

	//update all positions
	updateZValues();
	update();
}

void CCipasComposition::raiseItem( CComposerItem* item )
{
	//search item
	QMutableLinkedListIterator<CComposerItem*> it( mItemZList );
	if ( it.findNext( item ) )
	{
		if ( it.hasNext() )
		{
			it.remove();
			it.next();
			it.insert( item );
		}
	}
}

void CCipasComposition::lowerSelectedItems()
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	QList<CComposerItem*>::iterator it = selectedItems.begin();
	for ( ; it != selectedItems.end(); ++it )
	{
		lowerItem( *it );
	}

	//update all positions
	updateZValues();
	update();
}

void CCipasComposition::lowerItem( CComposerItem* item )
{
	//search item
	QMutableLinkedListIterator<CComposerItem*> it( mItemZList );
	if ( it.findNext( item ) )
	{
		it.previous();
		if ( it.hasPrevious() )
		{
			it.remove();
			it.previous();
			it.insert( item );
		}
	}
}

void CCipasComposition::moveSelectedItemsToTop()
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	QList<CComposerItem*>::iterator it = selectedItems.begin();

	for ( ; it != selectedItems.end(); ++it )
	{
		moveItemToTop( *it );
	}

	//update all positions
	updateZValues();
	update();
}

void CCipasComposition::moveItemToTop( CComposerItem* item )
{
	//search item
	QMutableLinkedListIterator<CComposerItem*> it( mItemZList );
	if ( it.findNext( item ) )
	{
		it.remove();
	}
	mItemZList.push_back( item );
}

void CCipasComposition::moveSelectedItemsToBottom()
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	QList<CComposerItem*>::iterator it = selectedItems.begin();
	for ( ; it != selectedItems.end(); ++it )
	{
		moveItemToBottom( *it );
	}

	//update all positions
	updateZValues();
	update();
}

void CCipasComposition::moveItemToBottom( CComposerItem* item )
{
	//search item
	QMutableLinkedListIterator<CComposerItem*> it( mItemZList );
	if ( it.findNext( item ) )
	{
		it.remove();
	}
	mItemZList.push_front( item );
}

void CCipasComposition::alignSelectedItemsLeft()
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	if ( selectedItems.size() < 2 )
	{
		return;
	}

	QRectF selectedItemBBox;
	if ( boundingRectOfSelectedItems( selectedItemBBox ) != 0 )
	{
		return;
	}

	double minXCoordinate = selectedItemBBox.left();

	//align items left to minimum x coordinate
	QUndoCommand* parentCommand = new QUndoCommand( tr( "Aligned items left" ) );
	QList<CComposerItem*>::iterator align_it = selectedItems.begin();
	for ( ; align_it != selectedItems.end(); ++align_it )
	{
		CipasItemCommand* subcommand = new CipasItemCommand( *align_it, "", parentCommand );
		subcommand->savePreviousState();
		QTransform itemTransform = ( *align_it )->transform();
		itemTransform.translate( minXCoordinate - itemTransform.dx(), 0 );
		( *align_it )->setTransform( itemTransform );
		subcommand->saveAfterState();
	}
	mUndoStack.push( parentCommand );
}

void CCipasComposition::alignSelectedItemsHCenter()
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	if ( selectedItems.size() < 2 )
	{
		return;
	}

	QRectF selectedItemBBox;
	if ( boundingRectOfSelectedItems( selectedItemBBox ) != 0 )
	{
		return;
	}

	double averageXCoord = ( selectedItemBBox.left() + selectedItemBBox.right() ) / 2.0;

	//place items
	QUndoCommand* parentCommand = new QUndoCommand( tr( "Aligned items hcenter" ) );
	QList<CComposerItem*>::iterator align_it = selectedItems.begin();
	for ( ; align_it != selectedItems.end(); ++align_it )
	{
		CipasItemCommand* subcommand = new CipasItemCommand( *align_it, "", parentCommand );
		subcommand->savePreviousState();
		QTransform itemTransform = ( *align_it )->transform();
		itemTransform.translate( averageXCoord - itemTransform.dx() - ( *align_it )->rect().width() / 2.0, 0 );
		( *align_it )->setTransform( itemTransform );
		subcommand->saveAfterState();
	}
	mUndoStack.push( parentCommand );
}

void CCipasComposition::alignSelectedItemsRight()
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	if ( selectedItems.size() < 2 )
	{
		return;
	}

	QRectF selectedItemBBox;
	if ( boundingRectOfSelectedItems( selectedItemBBox ) != 0 )
	{
		return;
	}

	double maxXCoordinate = selectedItemBBox.right();

	//align items right to maximum x coordinate
	QUndoCommand* parentCommand = new QUndoCommand( tr( "Aligned items right" ) );
	QList<CComposerItem*>::iterator align_it = selectedItems.begin();
	for ( ; align_it != selectedItems.end(); ++align_it )
	{
		CipasItemCommand* subcommand = new CipasItemCommand( *align_it, "", parentCommand );
		subcommand->savePreviousState();
		QTransform itemTransform = ( *align_it )->transform();
		itemTransform.translate( maxXCoordinate - itemTransform.dx() - ( *align_it )->rect().width(), 0 );
		( *align_it )->setTransform( itemTransform );
		subcommand->saveAfterState();
	}
	mUndoStack.push( parentCommand );
}

void CCipasComposition::alignSelectedItemsTop()
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	if ( selectedItems.size() < 2 )
	{
		return;
	}

	QRectF selectedItemBBox;
	if ( boundingRectOfSelectedItems( selectedItemBBox ) != 0 )
	{
		return;
	}

	double minYCoordinate = selectedItemBBox.top();

	QUndoCommand* parentCommand = new QUndoCommand( tr( "Aligned items top" ) );
	QList<CComposerItem*>::iterator align_it = selectedItems.begin();
	for ( ; align_it != selectedItems.end(); ++align_it )
	{
		CipasItemCommand* subcommand = new CipasItemCommand( *align_it, "", parentCommand );
		subcommand->savePreviousState();
		QTransform itemTransform = ( *align_it )->transform();
		itemTransform.translate( 0, minYCoordinate - itemTransform.dy() );
		( *align_it )->setTransform( itemTransform );
		subcommand->saveAfterState();
	}
	mUndoStack.push( parentCommand );
}

void CCipasComposition::alignSelectedItemsVCenter()
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	if ( selectedItems.size() < 2 )
	{
		return;
	}

	QRectF selectedItemBBox;
	if ( boundingRectOfSelectedItems( selectedItemBBox ) != 0 )
	{
		return;
	}

	double averageYCoord = ( selectedItemBBox.top() + selectedItemBBox.bottom() ) / 2.0;
	QUndoCommand* parentCommand = new QUndoCommand( tr( "Aligned items vcenter" ) );
	QList<CComposerItem*>::iterator align_it = selectedItems.begin();
	for ( ; align_it != selectedItems.end(); ++align_it )
	{
		CipasItemCommand* subcommand = new CipasItemCommand( *align_it, "", parentCommand );
		subcommand->savePreviousState();
		QTransform itemTransform = ( *align_it )->transform();
		itemTransform.translate( 0, averageYCoord - itemTransform.dy() - ( *align_it )->rect().height() / 2 );
		( *align_it )->setTransform( itemTransform );
		subcommand->saveAfterState();
	}
	mUndoStack.push( parentCommand );
}

void CCipasComposition::alignSelectedItemsBottom()
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	if ( selectedItems.size() < 2 )
	{
		return;
	}

	QRectF selectedItemBBox;
	if ( boundingRectOfSelectedItems( selectedItemBBox ) != 0 )
	{
		return;
	}

	double maxYCoord = selectedItemBBox.bottom();
	QUndoCommand* parentCommand = new QUndoCommand( tr( "Aligned items bottom" ) );
	QList<CComposerItem*>::iterator align_it = selectedItems.begin();
	for ( ; align_it != selectedItems.end(); ++align_it )
	{
		CipasItemCommand* subcommand = new CipasItemCommand( *align_it, "", parentCommand );
		subcommand->savePreviousState();
		QTransform itemTransform = ( *align_it )->transform();
		itemTransform.translate( 0, maxYCoord - itemTransform.dy() - ( *align_it )->rect().height() );
		( *align_it )->setTransform( itemTransform );
		subcommand->saveAfterState();
	}
	mUndoStack.push( parentCommand );
}

void CCipasComposition::updateZValues()
{
	int counter = 1;
	QLinkedList<CComposerItem*>::iterator it = mItemZList.begin();
	CComposerItem* currentItem = 0;

	QUndoCommand* parentCommand = new QUndoCommand( tr( "Item z-order changed" ) );
	for ( ; it != mItemZList.end(); ++it )
	{
		currentItem = *it;
		if ( currentItem )
		{
			CipasItemCommand* subcommand = new CipasItemCommand( *it, "", parentCommand );
			subcommand->savePreviousState();
			currentItem->setZValue( counter );
			subcommand->saveAfterState();
		}
		++counter;
	}
	mUndoStack.push( parentCommand );
}


void CCipasComposition::sortZList()
{
	if ( mItemZList.size() < 2 )
	{
		return;
	}

	QLinkedList<CComposerItem*>::const_iterator lIt = mItemZList.constBegin();
	QLinkedList<CComposerItem*> sortedList;

	for ( ; lIt != mItemZList.constEnd(); ++lIt )
	{
		QLinkedList<CComposerItem*>::iterator insertIt = sortedList.begin();
		for ( ; insertIt != sortedList.end(); ++insertIt )
		{
			if (( *lIt )->zValue() < ( *insertIt )->zValue() )
			{
				break;
			}
		}
		sortedList.insert( insertIt, ( *lIt ) );
	}

	mItemZList = sortedList;
}

QPointF CCipasComposition::snapPointToGrid( const QPointF& scenePoint ) const
{
	if ( !mSnapToGrid || mSnapGridResolution <= 0 )
	{
		return scenePoint;
	}

	//snap x coordinate //todo: add support for x- and y- offset
	int xRatio = ( int )(( scenePoint.x() - mSnapGridOffsetX ) / mSnapGridResolution + 0.5 );
	int yRatio = ( int )(( scenePoint.y() - mSnapGridOffsetY ) / mSnapGridResolution + 0.5 );

	return QPointF( xRatio * mSnapGridResolution + mSnapGridOffsetX, yRatio * mSnapGridResolution + mSnapGridOffsetY );
}

int CCipasComposition::boundingRectOfSelectedItems( QRectF& bRect )
{
	QList<CComposerItem*> selectedItems = selectedComposerItems();
	if ( selectedItems.size() < 1 )
	{
		return 1;
	}

	//set the box to the first item
	CComposerItem* currentItem = selectedItems.at( 0 );
	double minX = currentItem->transform().dx();
	double minY = currentItem->transform().dy();
	double maxX = minX + currentItem->rect().width();
	double maxY = minY + currentItem->rect().height();

	double currentMinX, currentMinY, currentMaxX, currentMaxY;

	for ( int i = 1; i < selectedItems.size(); ++i )
	{
		currentItem = selectedItems.at( i );
		currentMinX = currentItem->transform().dx();
		currentMinY = currentItem->transform().dy();
		currentMaxX = currentMinX + currentItem->rect().width();
		currentMaxY = currentMinY + currentItem->rect().height();

		if ( currentMinX < minX )
			minX = currentMinX;
		if ( currentMaxX > maxX )
			maxX = currentMaxX;
		if ( currentMinY < minY )
			minY = currentMinY;
		if ( currentMaxY > maxY )
			maxY = currentMaxY;
	}

	bRect.setTopLeft( QPointF( minX, minY ) );
	bRect.setBottomRight( QPointF( maxX, maxY ) );
	return 0;
}

void CCipasComposition::setSnapToGridEnabled( bool b )
{
	mSnapToGrid = b;
	if ( mPaperItem )
	{
		mPaperItem->update();
	}
}

void CCipasComposition::setSnapGridResolution( double r )
{
	mSnapGridResolution = r;
	if ( mPaperItem )
	{
		mPaperItem->update();
	}
}

void CCipasComposition::setSnapGridOffsetX( double offset )
{
	mSnapGridOffsetX = offset;
	if ( mPaperItem )
	{
		mPaperItem->update();
	}
}

void CCipasComposition::setSnapGridOffsetY( double offset )
{
	mSnapGridOffsetY = offset;
	if ( mPaperItem )
	{
		mPaperItem->update();
	}
}

void CCipasComposition::setGridPen( const QPen& p )
{
	mGridPen = p;
	if ( mPaperItem )
	{
		mPaperItem->update();
	}
	saveGridAppearanceSettings();
}

void CCipasComposition::setGridStyle( GridStyle s )
{
	mGridStyle = s;
	if ( mPaperItem )
	{
		mPaperItem->update();
	}
	saveGridAppearanceSettings();
}

void CCipasComposition::loadGridAppearanceSettings()
{
	//read grid style, grid color and pen width from settings
	QSettings s;

	QString gridStyleString;
	int red, green, blue;
	double penWidth;

	gridStyleString = s.value( "/cipas/composerGridStyle", "Solid" ).toString();
	penWidth = s.value( "/cipas/composerGridWidth", 0.5 ).toDouble();
	red = s.value( "/cipas/composerGridRed", 0 ).toInt();
	green = s.value( "/cipas/composerGridGreen", 0 ).toInt();
	blue = s.value( "/cipas/composerGridBlue", 0 ).toInt();

	mGridPen.setColor( QColor( red, green, blue ) );
	mGridPen.setWidthF( penWidth );

	/*if ( gridStyleString == "Dots" )
	{
		mGridStyle = Dots;
	}
	else*/ if ( gridStyleString == "Crosses" )
	{
		mGridStyle = Crosses;
	}
	else
	{
		mGridStyle = Solid;
	}
}

void CCipasComposition::saveGridAppearanceSettings()
{
	//store grid appearance settings
	QSettings s;
	s.setValue( "/cipas/composerGridWidth", mGridPen.widthF() );
	s.setValue( "/cipas/composerGridRed", mGridPen.color().red() );
	s.setValue( "/cipas/composerGridGreen", mGridPen.color().green() );
	s.setValue( "/cipas/composerGridBlue", mGridPen.color().blue() );

	if ( mGridStyle == Solid )
	{
		s.setValue( "/cipas/composerGridStyle", "Solid" );
	}
	//else if ( mGridStyle == Dots )
	//{
	//	s.setValue( "/qgis/composerGridStyle", "Dots" );
	//}
	else if ( mGridStyle == Crosses )
	{
		s.setValue( "/cipas/composerGridStyle", "Crosses" );
	}
}


void CCipasComposition::beginCommand( CComposerItem* item, const QString& commandText, CipasComposerMergeCommand::Context c )
{
	delete mActiveCommand;
	if ( !item )
	{
		mActiveCommand = 0;
		return;
	}

	if ( c == CipasComposerMergeCommand::Unknown )
	{
		mActiveCommand = new CipasComposerItemCommand( item, commandText );
	}
	else
	{
		mActiveCommand = new CipasComposerMergeCommand( c, item, commandText );
	}
	mActiveCommand->savePreviousState();
}

void CCipasComposition::endCommand()
{
	if ( mActiveCommand )
	{
		mActiveCommand->saveAfterState();
		if ( mActiveCommand->containsChange() ) //protect against empty commands
		{
			mUndoStack.push( mActiveCommand );
		}
		else
		{
			delete mActiveCommand;
		}
		mActiveCommand = 0;
	}
}

void CCipasComposition::cancelCommand()
{
	delete mActiveCommand;
	mActiveCommand = 0;
}

