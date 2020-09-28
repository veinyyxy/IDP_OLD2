
#include <QPen>
#include <QPainter>

#include "CipasComposition.h"
#include "CipasComposeritemgroup.h"

CipasComposerItemGroup::CipasComposerItemGroup( CCipasComposition* c ): CComposerItem( c )
{
  setZValue( 90 );
  show();
}

CipasComposerItemGroup::~CipasComposerItemGroup()
{
  QSet<CComposerItem*>::iterator itemIt = mItems.begin();
   for ( ; itemIt != mItems.end(); ++itemIt )
   {
     if ( *itemIt )
     {
       mComposition->removeItem( *itemIt );
       ( *itemIt )->setFlag( QGraphicsItem::ItemIsSelectable, true );
     }
   }
   mItems.clear();
}

void CipasComposerItemGroup::addItem( CComposerItem* item )
{
  if ( !item )
  {
    return;
  }

  if ( mItems.contains( item ) )
  {
    return;
  }
  mItems.insert( item );
  item->setSelected( false );
  item->setFlag( QGraphicsItem::ItemIsSelectable, false ); //item in groups cannot be selected

  //update extent (which is in scene coordinates)
  double minXItem = item->transform().dx();
  double minYItem = item->transform().dy();
  double maxXItem = minXItem + item->rect().width();
  double maxYItem = minYItem + item->rect().height();

  if ( mSceneBoundingRectangle.isEmpty() ) //we add the first item
  {
    mSceneBoundingRectangle.setLeft( minXItem );
    mSceneBoundingRectangle.setTop( minYItem );
    mSceneBoundingRectangle.setRight( maxXItem );
    mSceneBoundingRectangle.setBottom( maxYItem );
  }

  else
  {
    if ( minXItem < mSceneBoundingRectangle.left() )
    {
      mSceneBoundingRectangle.setLeft( minXItem );
    }
    if ( minYItem < mSceneBoundingRectangle.top() )
    {
      mSceneBoundingRectangle.setTop( minYItem );
    }
    if ( maxXItem > mSceneBoundingRectangle.right() )
    {
      mSceneBoundingRectangle.setRight( maxXItem );
    }
    if ( maxYItem > mSceneBoundingRectangle.bottom() )
    {
      mSceneBoundingRectangle.setBottom( maxYItem );
    }
  }

  CComposerItem::setSceneRect( mSceneBoundingRectangle ); //call method of superclass to avoid repositioning of items

}

void CipasComposerItemGroup::removeItems()
{
  QSet<CComposerItem*>::iterator item_it = mItems.begin();
  for ( ; item_it != mItems.end(); ++item_it )
  {
    ( *item_it )->setFlag( QGraphicsItem::ItemIsSelectable, true ); //enable item selection again
    ( *item_it )->setSelected( true );
  }
  mItems.clear();
}

void CipasComposerItemGroup::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  drawFrame( painter );
  if ( isSelected() )
  {
    drawSelectionBoxes( painter );
  }
}

void CipasComposerItemGroup::setSceneRect( const QRectF& rectangle )
{
  //calculate values between 0 and 1 for boundaries of all contained items, depending on their positions in the item group rectangle.
  //then position the item boundaries in the new item group rect such that these values are the same
  double xLeftCurrent = transform().dx();
  double xRightCurrent = xLeftCurrent + rect().width();
  double yTopCurrent = transform().dy();
  double yBottomCurrent = yTopCurrent + rect().height();

  double xItemLeft, xItemRight, yItemTop, yItemBottom;
  double xItemLeftNew, xItemRightNew, yItemTopNew, yItemBottomNew;
  double xParamLeft, xParamRight, yParamTop, yParamBottom;


  QSet<CComposerItem*>::iterator item_it = mItems.begin();
  for ( ; item_it != mItems.end(); ++item_it )
  {
    xItemLeft = ( *item_it )->transform().dx();
    xItemRight = xItemLeft + ( *item_it )->rect().width();
    yItemTop = ( *item_it )->transform().dy();
    yItemBottom = yItemTop + ( *item_it )->rect().height();

    xParamLeft = ( xItemLeft - xLeftCurrent ) / ( xRightCurrent - xLeftCurrent );
    xParamRight = ( xItemRight - xLeftCurrent ) / ( xRightCurrent - xLeftCurrent );
    yParamTop = ( yItemTop - yTopCurrent ) / ( yBottomCurrent - yTopCurrent );
    yParamBottom = ( yItemBottom - yTopCurrent ) / ( yBottomCurrent - yTopCurrent );

    xItemLeftNew = xParamLeft * rectangle.right()  + ( 1 - xParamLeft ) * rectangle.left();
    xItemRightNew = xParamRight * rectangle.right() + ( 1 - xParamRight ) * rectangle.left();
    yItemTopNew = yParamTop * rectangle.bottom() + ( 1 - yParamTop ) * rectangle.top();
    yItemBottomNew = yParamBottom * rectangle.bottom() + ( 1 - yParamBottom ) * rectangle.top();

    ( *item_it )->setSceneRect( QRectF( xItemLeftNew, yItemTopNew, xItemRightNew - xItemLeftNew, yItemBottomNew - yItemTopNew ) );
  }
  CComposerItem::setSceneRect( rectangle );
}

void CipasComposerItemGroup::drawFrame( QPainter* p )
{
  if ( !mComposition )
  {
    return;
  }

  if ( mFrame && mComposition->plotStyle() == CCipasComposition::Preview )
  {
    QPen newPen( pen() );
    newPen.setStyle( Qt::DashLine );
    newPen.setColor( QColor( 128, 128, 128, 128 ) );
    p->setPen( newPen );
    p->setRenderHint( QPainter::Antialiasing, true );
    p->drawRect( QRectF( 0, 0, rect().width(), rect().height() ) );
  }
}
