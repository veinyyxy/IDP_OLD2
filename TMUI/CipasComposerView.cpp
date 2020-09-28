#include <QMainWindow>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QTableWidgetItem>
#include <QTableWidget>


#include "ComposerItem.h"
#include "CipasComposerMap.h"
#include "CipasComposerLabel.h"
#include "CipasComposerShape.h"
#include "CipasComposition.h"
#include "CipasComposerPicture.h"
#include "CipasComposerScaleBar.h"
#include "CipasComposerArrow.h"
#include "CipasComposerLegend.h"
#include "CipasComposerItemgroup.h"
#include "CipasComposerGLW.h"
#include "CipasComposerView.h"

CCipasComposerView::CCipasComposerView(QWidget* parent, const char* name, Qt::WFlags f ):QGraphicsView( parent )
	, mShiftKeyPressed( false )
	, mRubberBandItem( 0 )
	, mRubberBandLineItem( 0 )
	, mMoveContentItem( 0 )
	, mPaintingEnabled( true )
{
	setResizeAnchor( QGraphicsView::AnchorViewCenter );
	setMouseTracking( true );
	viewport()->setMouseTracking( true );
}

CCipasComposerView::~CCipasComposerView(void)
{
	
}

void CCipasComposerView::mousePressEvent( QMouseEvent* e )
{
	m_beginPoint = e->pos();
	if ( !composition() )
	{
		return;
	}
	m_beginPoint = composition()->snapPointToGrid(e->pos()).toPoint();
	QPointF scenePoint = mapToScene( e->pos() );
	QPointF snappedScenePoint = composition()->snapPointToGrid( scenePoint );

	//lock/unlock position of item with right click
	if ( e->button() == Qt::RightButton )
	{
		CComposerItem* selectedItem = composition()->composerItemAt( scenePoint );
		if ( selectedItem )
		{
			bool lock = selectedItem->positionLock() ? false : true;
			selectedItem->setPositionLock( lock );
			selectedItem->update();
			//make sure the new cursor is correct
			QPointF itemPoint = selectedItem->mapFromScene( scenePoint );
			selectedItem->updateCursor( itemPoint );
		}
		return;
	}

	switch ( mCurrentTool )
	{
		//select/deselect items and pass mouse event further
	case Select:
		{
			if ( !mShiftKeyPressed ) //keep selection if shift key pressed
			{
				composition()->clearSelection();
			}

			//select topmost item at position of event
			CComposerItem* selectedItem = composition()->composerItemAt( scenePoint );
			if ( !selectedItem )
			{
				break;
			}
			composition()->setCurrentItem(selectedItem);
			selectedItem->setSelected( true );
			QGraphicsView::mousePressEvent( e );
			emit selectedItemChanged( selectedItem );
		}break;

	case MoveItemContent:
		{
			CComposerItem* item = dynamic_cast<CComposerItem *>( itemAt( e->pos() ) );
			if ( item )
			{
				item->setSelected(true);
				composition()->setCurrentItem(item);
				mMoveContentStartPos = scenePoint;
			}
			mMoveContentItem = item;
		}break;
	case MapZoomIn:
		{
			double curScale;
			//select topmost item at position of event
			CComposerItem* theItem = composition()->composerItemAt( scenePoint );
			CCipasComposerGLW* composerGLW = dynamic_cast<CCipasComposerGLW *>( theItem );
			if ( composerGLW )
			{
				//if ( composerGLW->isSelected() )
				{
					QPointF itemPoint = composerGLW->mapFromScene( scenePoint );
					emit emitMapZoonIn();
					composerGLW->update();
					scene()->update();
				}
			}
		}break;
	case MapZoomOut:
		{
			double curScale;
			//select topmost item at position of event
			CComposerItem* theItem = composition()->composerItemAt( scenePoint );
			CCipasComposerGLW* composerGLW = dynamic_cast<CCipasComposerGLW *>( theItem );
			if ( composerGLW )
			{
				//if ( composerGLW->isSelected() )
				{
					QPointF itemPoint = composerGLW->mapFromScene( scenePoint );
					curScale = 1.2f;
					emit emitMapZoonOut();
					composerGLW->update();
					scene()->update();
				}
			}
		}break;
	case AddArrow:
		{
			mRubberBandStartPos = QPointF( snappedScenePoint.x(), snappedScenePoint.y() );
			mRubberBandLineItem = new QGraphicsLineItem( snappedScenePoint.x(), snappedScenePoint.y(), snappedScenePoint.x(), snappedScenePoint.y() );
			mRubberBandLineItem->setZValue( 100 );
			scene()->addItem( mRubberBandLineItem );
			scene()->update();
		}break;
	case AddShape:
		{
			QTransform t;
			mRubberBandItem = new QGraphicsRectItem( 0, 0, 0, 0 );
			mRubberBandStartPos = QPointF( snappedScenePoint.x(), snappedScenePoint.y() );
			t.translate( snappedScenePoint.x(), snappedScenePoint.y() );
			mRubberBandItem->setTransform( t );
			mRubberBandItem->setZValue( 100 );
			scene()->addItem( mRubberBandItem );
			scene()->update();
		}break;
	case AddLabel:
		{
			CCipasComposerLabel* newLabelItem = new CCipasComposerLabel( composition() );
			newLabelItem->setText( tr( "Label" ) );
			newLabelItem->adjustSizeToText();
			newLabelItem->setSceneRect( QRectF( snappedScenePoint.x(), snappedScenePoint.y(), newLabelItem->rect().width(), newLabelItem->rect().height() ) );
			addComposerLabel( newLabelItem );
			emit actionFinished();
		}break;
	case AddScalebar:
		{
			CCipasComposerScaleBar* newScaleBar = new CCipasComposerScaleBar( composition() );
			newScaleBar->setSceneRect( QRectF( snappedScenePoint.x(), snappedScenePoint.y(), 20, 20 ) );
			addComposerScaleBar( newScaleBar );
			emit actionFinished();
		}break;
	case AddLegend:
		{
			CCipasComposerLegend* newLegend = new CCipasComposerLegend( composition() );
			newLegend->beginDrawSymbol(true);
			newLegend->setSceneRect( QRectF( snappedScenePoint.x(), snappedScenePoint.y(), newLegend->rect().width(), newLegend->rect().height() ) );
			double dx=newLegend->rect().width();
			double dy=newLegend->rect().height();
			addComposerLegend( newLegend );
			emit actionFinished();
		}break;
		
	case AddPicture:
		{
			CCipasComposerPicture* newPicture = new CCipasComposerPicture( composition() );
			newPicture->setSceneRect( QRectF( snappedScenePoint.x(), snappedScenePoint.y(), 30, 30 ) );
			addComposerPicture( newPicture );
			emit actionFinished();
		}break;
	case AddGLW:
		{
			QTransform t;
			mRubberBandItem = new QGraphicsRectItem( 0, 0, 0, 0 );
			mRubberBandStartPos = QPointF( snappedScenePoint.x(), snappedScenePoint.y() );
			t.translate( snappedScenePoint.x(), snappedScenePoint.y() );
			mRubberBandItem->setTransform( t );
			mRubberBandItem->setZValue( 100 );
			scene()->addItem( mRubberBandItem );
		}break;
	default:
		break;
	}
}

void CCipasComposerView::mouseReleaseEvent( QMouseEvent* e )
{
	m_endPoint = e->pos();


	if ( !composition() )
	{
		return;
	}
	m_endPoint =composition()->snapPointToGrid( e->pos()).toPoint();
	QPointF scenePoint = mapToScene( e->pos() );

	switch ( mCurrentTool )
	{
	case Select:
		{
			QGraphicsView::mouseReleaseEvent( e );
			CComposerItem* selectedItem = composition()->composerItemAt( scenePoint );
			if (selectedItem )
			{
				
			}
		}break;
	case MoveItemContent:
		{
			if ( mMoveContentItem )
			{
				double moveX = scenePoint.x() - mMoveContentStartPos.x();
				double moveY = scenePoint.y() - mMoveContentStartPos.y();
				mMoveContentItem->moveContent( -moveX, -moveY );
				mMoveContentItem->update();
				 
				
				emit emitMapMove(m_beginPoint,m_endPoint);

				QGraphicsScene* pGs = scene();
				if(pGs)
					pGs->update();
			}
		}break;

	case AddArrow:
		{
			QPointF scenePoint = mapToScene( e->pos() );
			QPointF snappedScenePoint = composition()->snapPointToGrid( scenePoint );
			CCipasComposerArrow* composerArrow = new CCipasComposerArrow( mRubberBandStartPos, QPointF( snappedScenePoint.x(), snappedScenePoint.y() ), composition() );
			addComposerArrow( composerArrow );
			scene()->removeItem( mRubberBandLineItem );
			delete mRubberBandLineItem;
			mRubberBandLineItem = 0;
			emit actionFinished();
			
		}break;
	case AddShape:
		{
			if ( !mRubberBandItem || mRubberBandItem->rect().width() < 0.1 || mRubberBandItem->rect().width() < 0.1 )
			{
				scene()->removeItem( mRubberBandItem );
				delete mRubberBandItem;
				mRubberBandItem = 0;
				return;
			}

			CCipasComposerShape* composerShape = new CCipasComposerShape( mRubberBandItem->transform().dx(), mRubberBandItem->transform().dy(), mRubberBandItem->rect().width(), mRubberBandItem->rect().height(), composition() );
			double dx=mRubberBandItem->transform().dx();
			double dy=mRubberBandItem->transform().dy();
			double wx=mRubberBandItem->rect().width();
			double wy=mRubberBandItem->rect().height();

			addComposerShape( composerShape );
			scene()->removeItem( mRubberBandItem );
			delete mRubberBandItem;
			mRubberBandItem = 0;
			emit actionFinished();
		}break;
	case AddGLW:
		{
			if ( !mRubberBandItem || mRubberBandItem->rect().width() < 0.1 || mRubberBandItem->rect().width() < 0.1 )
			{
				scene()->removeItem( mRubberBandItem );
				delete mRubberBandItem;
				mRubberBandItem = 0;
				return;
			}
			CCipasComposerGLW* composerGLW = new CCipasComposerGLW( mRubberBandItem->transform().dx(), mRubberBandItem->transform().dy(), mRubberBandItem->rect().width(), mRubberBandItem->rect().height(), composition());
			composerGLW->SetOsgViewer(m_osgViewer);
			connect(composerGLW, SIGNAL(myImages(float, float)), m_osgViewer, SLOT(GiveYouFuckImage(float, float)));
			connect(m_osgViewer, SIGNAL(toUpdate()), composerGLW->GetViewerQT(), SLOT(updateFuckImage()));
			double px=composition()->paperWidth();
			double wx=mRubberBandItem->rect().width();
			double wy=mRubberBandItem->rect().height();
			mXp=composition()->paperWidth()/25.4;
			mYp=composition()->paperHeight()/25.4;
			addComposerGLW( composerGLW );
			scene()->removeItem( mRubberBandItem );
			composerGLW->update();
			scene()->update();
			delete mRubberBandItem;
			mRubberBandItem = 0;
			emit actionFinished();
		}break;
		
		
	default:
		break;
	}
	scene()->update();
}

void CCipasComposerView::mouseMoveEvent( QMouseEvent* e )
{
	m_endPoint = e->pos();
	static QPoint lastPos;
	if ( !composition() )
	{
		return;
	}

	if ( e->buttons() == Qt::NoButton )
	{
		if ( mCurrentTool == Select )
		{
			QGraphicsView::mouseMoveEvent( e );
		}
	}
	else
	{
		QPointF scenePoint = mapToScene( e->pos() );

		switch ( mCurrentTool )
		{
		case Select:
			{
				QGraphicsView::mouseMoveEvent( e );
			}break;
		case AddArrow:
			{
				if ( mRubberBandLineItem )
				{
					mRubberBandLineItem->setLine( mRubberBandStartPos.x(), mRubberBandStartPos.y(),  scenePoint.x(),  scenePoint.y() );
				}
			}break;
		case AddMap:
		case AddGLW:
		case AddShape:
			{
				double x = 0;
				double y = 0;
				double width = 0;
				double height = 0;

				double dx = scenePoint.x() - mRubberBandStartPos.x();
				double dy = scenePoint.y() - mRubberBandStartPos.y();

				if ( dx < 0 )
				{
					x = scenePoint.x();
					width = -dx;
				}
				else
				{
					x = mRubberBandStartPos.x();
					width = dx;
				}

				if ( dy < 0 )
				{
					y = scenePoint.y();
					height = -dy;
				}
				else
				{
					y = mRubberBandStartPos.y();
					height = dy;
				}

				if ( mRubberBandItem )
				{
					mRubberBandItem->setRect( 0, 0, width, height );
					QTransform t;
					t.translate( x, y );
					mRubberBandItem->setTransform( t );
				}
				
			}break;
			
		case MoveItemContent:
			{
				QVector3D curTranslate;
				CCipasComposerGLW* composerGLW = dynamic_cast<CCipasComposerGLW *>( mMoveContentItem );
				QImage locImage;
				if ( composerGLW )
				{
					composerGLW->setSelected(true);
					composition()->setCurrentItem(composerGLW);
					if (composerGLW->gridEnabled() && composerGLW->gridIntervalX()>1&& composerGLW->gridIntervalY()>1)
					{
						composerGLW->setReCalcGrid(true);
					}
					composerGLW->setOffset( scenePoint.x() - mMoveContentStartPos.x(), scenePoint.y() - mMoveContentStartPos.y() );
					
					composerGLW->update();
					scene()->update();
				}
			}break;
			
		default:
			break;
		}
	}
	lastPos = e->pos();
}

void CCipasComposerView::mouseDoubleClickEvent( QMouseEvent* e )
{
	e->ignore();
}

void CCipasComposerView::keyPressEvent( QKeyEvent * e )
{
	if ( e->key() == Qt::Key_Shift )
	{
		mShiftKeyPressed = true;
	}

	if ( !composition() )
	{
		return;
	}

	QList<CComposerItem*> composerItemList = composition()->selectedComposerItems();
	QList<CComposerItem*>::iterator itemIt = composerItemList.begin();
	
	//delete selected items
	if ( e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace )
	{
		
		for ( ; itemIt != composerItemList.end(); ++itemIt )
		{
			composition()->removeItem( *itemIt );
			emit itemRemoved( *itemIt );
			pushAddRemoveCommand( *itemIt, tr( "Item deleted" ), CCipasAddRemoveItemCommand::Removed );
		}
		
	}
	else if ( e->key() == Qt::Key_Left )
	{
		for ( ; itemIt != composerItemList.end(); ++itemIt )
		{
			( *itemIt )->move( -1.0, 0.0 );
		}
	}
	else if ( e->key() == Qt::Key_Right )
	{
		for ( ; itemIt != composerItemList.end(); ++itemIt )
		{
			( *itemIt )->move( 1.0, 0.0 );
		}
	}
	else if ( e->key() == Qt::Key_Down )
	{
		for ( ; itemIt != composerItemList.end(); ++itemIt )
		{
			( *itemIt )->move( 0.0, 1.0 );
		}
	}
	else if ( e->key() == Qt::Key_Up )
	{
		for ( ; itemIt != composerItemList.end(); ++itemIt )
		{
			( *itemIt )->move( 0.0, -1.0 );
		}
	}
	
}

void CCipasComposerView::keyReleaseEvent( QKeyEvent * e )
{
	if ( e->key() == Qt::Key_Shift )
	{
		mShiftKeyPressed = false;
	}
}

void CCipasComposerView::wheelEvent( QWheelEvent* event )
{
	QPointF scenePoint = mapToScene( event->pos() );
	double curScale;
	//select topmost item at position of event
	CComposerItem* theItem = composition()->composerItemAt( scenePoint );
	
}

void CCipasComposerView::paintEvent( QPaintEvent* event )
{
	if ( mPaintingEnabled )
	{
		

		QGraphicsView::paintEvent( event );

		

		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void CCipasComposerView::setComposition( CCipasComposition* c )
{
	setScene( c );
}

CCipasComposition* CCipasComposerView::composition()
{
	if ( scene() )
	{
		CCipasComposition* c = dynamic_cast<CCipasComposition *>( scene() );
		if ( c )
		{
			return c;
		}
	}
	return 0;
}


void CCipasComposerView::addComposerLabel( CCipasComposerLabel* label )
{
	composition()->addItem( label );
	emit composerLabelAdded( label );
	scene()->clearSelection();
	label->setSelected( true );
	emit selectedItemChanged( label );
	pushAddRemoveCommand( label, tr( "Label added" ) );
}


void CCipasComposerView::addComposerPicture( CCipasComposerPicture* picture )
{
	scene()->addItem( picture );
	emit composerPictureAdded( picture );
	scene()->clearSelection();
	picture->setSelected( true );
	emit selectedItemChanged( picture );
	pushAddRemoveCommand( picture, tr( "Picture added" ) );
}

void CCipasComposerView::sendItemAddedSignal( CComposerItem* item )
{
	item->setSelected( true );

	CCipasComposerArrow* arrow = dynamic_cast<CCipasComposerArrow*>( item );
	if ( arrow )
	{
		emit composerArrowAdded( arrow );
		emit selectedItemChanged( arrow );
		return;
	}

	CCipasComposerLabel* label = dynamic_cast<CCipasComposerLabel*>( item );
	if ( label )
	{
		emit composerLabelAdded( label );
		emit selectedItemChanged( label );
		return;
	}

	CCipasComposerMap* map = dynamic_cast<CCipasComposerMap*>( item );
	if ( map )
	{
		emit composerMapAdded( map );
		emit selectedItemChanged( map );
		return;
	}

	CCipasComposerScaleBar* scalebar = dynamic_cast<CCipasComposerScaleBar*>( item );
	if ( scalebar )
	{
		emit composerScaleBarAdded( scalebar );
		emit selectedItemChanged( scalebar );
		return;
	}

	CCipasComposerLegend* legend = dynamic_cast<CCipasComposerLegend*>( item );
	if ( legend )
	{
		emit composerLegendAdded( legend );
		emit selectedItemChanged( legend );
		return;
	}

	CCipasComposerPicture* picture = dynamic_cast<CCipasComposerPicture*>( item );
	if ( picture )
	{
		emit composerPictureAdded( picture );
		emit selectedItemChanged( picture );
		return;
	}
	
	CCipasComposerShape* shape = dynamic_cast<CCipasComposerShape*>( item );
	if ( shape )
	{
		emit composerShapeAdded( shape );
		emit selectedItemChanged( shape );
		return;
	}
}

void CCipasComposerView::addComposerScaleBar( CCipasComposerScaleBar* scaleBar )
{
	QList<const CCipasComposerGLW*> mapItemList = composition()->composerGLWItems();
	if ( mapItemList.size() > 0 )
	{
		scaleBar->setComposerGLW( mapItemList.at( 0 ) );
	}
	scaleBar->applyDefaultSize(); //4 segments, 1/5 of composer map width
	scene()->addItem( scaleBar );
	emit composerScaleBarAdded( scaleBar );
	scene()->clearSelection();
	scaleBar->setSelected( true );
	emit selectedItemChanged( scaleBar );
	pushAddRemoveCommand( scaleBar, tr( "Scale bar added" ) );
}

void CCipasComposerView::addComposerShape( CCipasComposerShape* shape )
{
	scene()->addItem( shape );
	emit composerShapeAdded( shape );
	scene()->clearSelection();
	shape->setSelected( true );
	emit selectedItemChanged( shape );
	pushAddRemoveCommand( shape, tr( "Shape added" ) );
}

void CCipasComposerView::addComposerArrow( CCipasComposerArrow* arrow )
{
	composition()->addItem( arrow );
	emit composerArrowAdded( arrow );
	scene()->clearSelection();
	arrow->setSelected( true );
	emit selectedItemChanged( arrow );
	pushAddRemoveCommand( arrow, tr( "Arrow added" ) );
}

void CCipasComposerView::pushAddRemoveCommand( CComposerItem* item, const QString& text, CCipasAddRemoveItemCommand::State state /*= CCipasAddRemoveItemCommand::State::Added */ )
{
	if ( !composition() )
	{
		return;
	}

	CCipasAddRemoveItemCommand* c = new CCipasAddRemoveItemCommand( state, item, composition(), text );
	connectAddRemoveCommandSignals( c );
	composition()->undoStack()->push( c );
}

void CCipasComposerView::connectAddRemoveCommandSignals( CCipasAddRemoveItemCommand* c )
{
	if ( !c )
	{
		return;
	}
	QObject::connect( c, SIGNAL( itemRemoved( CComposerItem* ) ), this, SIGNAL( itemRemoved( CComposerItem* ) ) );
	QObject::connect( c, SIGNAL( itemAdded( CComposerItem* ) ), this, SLOT( sendItemAddedSignal( CComposerItem* ) ) );
}

void CCipasComposerView::addComposerLegend( CCipasComposerLegend* legend )
{
	scene()->addItem( legend );
	emit composerLegendAdded( legend );
	scene()->clearSelection();
	legend->setSelected( true );
	emit selectedItemChanged( legend );
}

void CCipasComposerView::groupItems()
{
	if ( !composition() )
	{
		return;
	}

	QList<CComposerItem*> selectionList = composition()->selectedComposerItems();
	if ( selectionList.size() < 2 )
	{
		return; //not enough items for a group
	}
	CipasComposerItemGroup* itemGroup = new CipasComposerItemGroup( composition() );

	QList<CComposerItem*>::iterator itemIter = selectionList.begin();
	for ( ; itemIter != selectionList.end(); ++itemIter )
	{
		itemGroup->addItem( *itemIter );
	}

	composition()->addItem( itemGroup );
	itemGroup->setSelected( true );
	emit selectedItemChanged( itemGroup );
}

void CCipasComposerView::ungroupItems()
{
	if ( !composition() )
	{
		return;
	}

	QList<CComposerItem*> selectionList = composition()->selectedComposerItems();
	QList<CComposerItem*>::iterator itemIter = selectionList.begin();
	for ( ; itemIter != selectionList.end(); ++itemIter )
	{
		CipasComposerItemGroup* itemGroup = dynamic_cast<CipasComposerItemGroup *>( *itemIter );
		if ( itemGroup )
		{
			itemGroup->removeItems();
			composition()->removeItem( *itemIter );
			delete( *itemIter );
			emit itemRemoved( *itemIter );
		}
	}
}

void CCipasComposerView::addComposerGLW(CCipasComposerGLW* glw)
{
	scene()->addItem( glw );
	emit composerGLWAdded( glw );
	scene()->clearSelection();
	glw->setSelected( true );
	emit selectedItemChanged( glw );
	pushAddRemoveCommand( glw, tr( "glw added" ) );
}