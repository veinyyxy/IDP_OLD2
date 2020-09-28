#include <QGLWidget>
#include "ComposerView.h"

ComposerView::ComposerView(QWidget *parent): QGraphicsView(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	QGraphicsScene* scene = new QGraphicsScene(this);
	this->setScene(scene);
	centerOn(0,0);
	setBackgroundBrush(QColor(0, 0, 0, 255));
	setContentsMargins(0,0,0,0);
	setDragMode(QGraphicsView::ScrollHandDrag);

	setRenderHint(QPainter::Antialiasing);
	setCacheMode(QGraphicsView::CacheBackground);

	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);
	
}

ComposerView::~ComposerView()
{

}

void ComposerView::mousePressEvent( QMouseEvent* event)
{
	switch ( m_currentTool )
	{
		case Select:
			{

			}break;
		case AddArrow:
			{

			}break;
		case AddMap:
			{

			}break;
		case AddLegend:
			{

			}break;
		case AddLabel:
			{

			}break;
		case AddScalebar:
			{

			}break;
		case AddPicture:
			{
				QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(QPixmap(":images/thematicMap/mActionAddImage.png"));
				pixmapItem->setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable );
				pixmapItem->setPos(event->pos());
				this->scene()->addItem(pixmapItem);
			}break;
		case AddShape:
			{

			}break;
		case AddTable:
			{

			}break;
		case MoveItemContent:
			{

			}break;
		case AddGLW:
			{

			}break;
		case MapZoomIn:
			{

			}break;
		case MapZoomOut:
			{

			}break;
	}
}
void ComposerView::mouseReleaseEvent( QMouseEvent* )
{
	switch ( m_currentTool )
	{
	case Select:
		{

		}break;
	case AddArrow:
		{

		}break;
	case AddMap:
		{

		}break;
	case AddLegend:
		{

		}break;
	case AddLabel:
		{

		}break;
	case AddScalebar:
		{

		}break;
	case AddPicture:
		{

		}break;
	case AddShape:
		{

		}break;
	case AddTable:
		{

		}break;
	case MoveItemContent:
		{

		}break;
	case AddGLW:
		{

		}break;
	case MapZoomIn:
		{

		}break;
	case MapZoomOut:
		{

		}break;
	}
	m_currentTool = Select;
}
void ComposerView::mouseMoveEvent( QMouseEvent* )
{
	switch ( m_currentTool )
	{
	case Select:
		{

		}break;
	case AddArrow:
		{

		}break;
	case AddMap:
		{

		}break;
	case AddLegend:
		{

		}break;
	case AddLabel:
		{

		}break;
	case AddScalebar:
		{

		}break;
	case AddPicture:
		{

		}break;
	case AddShape:
		{

		}break;
	case AddTable:
		{

		}break;
	case MoveItemContent:
		{

		}break;
	case AddGLW:
		{

		}break;
	case MapZoomIn:
		{

		}break;
	case MapZoomOut:
		{

		}break;
	}
}
void ComposerView::mouseDoubleClickEvent( QMouseEvent* e )
{

}
void ComposerView::keyPressEvent( QKeyEvent * e )
{

}
void ComposerView::keyReleaseEvent( QKeyEvent * e )
{

}
void ComposerView::wheelEvent( QWheelEvent* event )
{
	
}
//void ComposerView::paintEvent( QPaintEvent* event )
//{
//
//}
void	ComposerView::resizeEvent ( QResizeEvent * event )
{

}

void ComposerView::AddGraphicsPixmapItem( QPixmap pixmap )
{
	QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(pixmap);
	this->scene()->addItem(pixmapItem);
}

void ComposerView::CurrentTool( Tool currentTool )
{
	m_currentTool = currentTool;
}

