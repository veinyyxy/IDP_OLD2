#ifndef COMPOSERVIEW_H
#define COMPOSERVIEW_H

#include <QtGui>
#include <QGraphicsView>

#include "tmui_global.h"

class TMUI_EXPORT ComposerView : public QGraphicsView
{
	Q_OBJECT

public:
	/**Current tool*/
	enum Tool
	{
		Select = 0,      // Select/Move item
		AddArrow,         //add arrow
		AddMap,          // add new map
		AddLegend, // add vector legend
		AddLabel,        // add label
		AddScalebar,     // add scalebar
		AddPicture,       // add raster/vector picture
		AddShape, //add shape item (ellipse, rectangle, triangle)
		AddTable, //add attribute table
		MoveItemContent, //move content of item (e.g. content of map)
		AddGLW,
		MapZoomIn,
		MapZoomOut
	};

	ComposerView(QWidget *parent = 0);
	~ComposerView();
	void AddGraphicsPixmapItem(QPixmap pixmap);
	void CurrentTool(Tool currentTool);
protected:
	void mousePressEvent( QMouseEvent* );
	void mouseReleaseEvent( QMouseEvent* );
	void mouseMoveEvent( QMouseEvent* );
	void mouseDoubleClickEvent( QMouseEvent* e );

	void keyPressEvent( QKeyEvent * e );
	void keyReleaseEvent( QKeyEvent * e );

	void wheelEvent( QWheelEvent* event );

	//void paintEvent( QPaintEvent* event );
	void	resizeEvent ( QResizeEvent * event );
private:
	void setRenderGLWidget(QGLWidget* widget);
	void drawBackground(QPainter* painter , QRectF& rectf);
private:
	QImage m_bgkImage;
	ComposerView::Tool m_currentTool;
};

#endif // COMPOSERVIEW_H
