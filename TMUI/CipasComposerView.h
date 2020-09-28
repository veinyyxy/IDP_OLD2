#pragma once
#include <osgViewer/View>
#include <QtGui/QGraphicsView>
#include "CipasAddRemoveItemCommand.h"
#include "tmui_global.h"
#include "AdapterWidget.h"

class QKeyEvent;
class QMainWindow;
class QMouseEvent;

class CComposerItem;
class CCipasComposition;
class CCipasPaperItem;
class CCipasComposerLabel;
class CCipasComposerPicture;
class CCipasComposerScaleBar;
class CCipasComposerMap;
class CCipasComposerShape;
class CCipasComposerArrow;
class CCipasComposerLegend;
class CCipasComposerGLW;

class TMUI_EXPORT CCipasComposerView :public QGraphicsView
{
	Q_OBJECT
public:
	typedef std::vector<osgViewer::View*> OSGVIEW_VECTOR, *P_OSGVIEW_VECTOR;
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
	CCipasComposerView(QWidget* parent = 0, const char* name = 0, Qt::WFlags f = 0);
	~CCipasComposerView(void);
	CCipasComposerView::Tool currentTool() const {return mCurrentTool;}
	void setCurrentTool( CCipasComposerView::Tool t ) {mCurrentTool = t;}
	/**Sets composition (derived from QGraphicsScene)*/
	void setComposition( CCipasComposition* c );
	/**Returns the composition or 0 in case of error*/
	CCipasComposition* composition();

	void addComposerLabel( CCipasComposerLabel* label );

	void addComposerScaleBar(CCipasComposerScaleBar* scaleBar);

	void addComposerPicture(CCipasComposerPicture* picture);
	void addComposerGLW(CCipasComposerGLW* glw);
	void addComposerShape( CCipasComposerShape* shape );
	void addComposerLegend( CCipasComposerLegend* legend );
	void addComposerArrow(CCipasComposerArrow* arrow);
	QMainWindow* composerWindow();

	void setPaintingEnabled( bool enabled ) { mPaintingEnabled = enabled; }
	bool paintingEnabled() const { return mPaintingEnabled; }

	void GetProductLayerName(QMap<QString, QString>& nameList);
	void GetGisLayerName(QMap<QString, QString>& nameList);
	void pushAddRemoveCommand(CComposerItem* item, const QString& text,  CCipasAddRemoveItemCommand::State state = CCipasAddRemoveItemCommand::State::Added );

	/**Groups the selected items*/
	void groupItems();

	/**Ungroups the selected items*/
	void ungroupItems();

	inline void SetOsgViewers(ViewerQT* pOV){m_osgViewer = pOV;}
protected:
	void mousePressEvent( QMouseEvent* );
	void mouseReleaseEvent( QMouseEvent* );
	void mouseMoveEvent( QMouseEvent* );
	void mouseDoubleClickEvent( QMouseEvent* e );

	void keyPressEvent( QKeyEvent * e );
	void keyReleaseEvent( QKeyEvent * e );

	void wheelEvent( QWheelEvent* event );

	void paintEvent( QPaintEvent* event );

private:
    /**Status of shift key (used for multiple selection)*/
    bool mShiftKeyPressed;
    /**Current composer tool*/
    CCipasComposerView::Tool mCurrentTool;
    /**Rubber band item*/
    QGraphicsRectItem* mRubberBandItem;
    /**Rubber band item for arrows*/
    QGraphicsLineItem* mRubberBandLineItem;
    /**Item to move content*/
    CComposerItem* mMoveContentItem;
    /**Start position of content move*/
    QPointF mMoveContentStartPos;
    /**Start of rubber band creation*/
    QPointF mRubberBandStartPos;

    bool mPaintingEnabled;
	QPoint m_beginPoint;
	QPoint m_endPoint;
	/**×ª»¯±ÈÀý**/
	double mXp;
	double mYp;
	  void connectAddRemoveCommandSignals( CCipasAddRemoveItemCommand* c );
	QImage m_MapImage;
	ViewerQT* m_osgViewer;
  public slots:
    /**Casts object to the proper subclass type and calls corresponding itemAdded signal*/
    void sendItemAddedSignal( CComposerItem* item );

  signals:
    /**Is emitted when selected item changed. If 0, no item is selected*/
    void selectedItemChanged( CComposerItem* selected );
    void composerLabelAdded( CCipasComposerLabel* label );
    ///**Is emitted when new composer map has been added to the view*/
    void composerMapAdded( CCipasComposerMap* map );
	void composerGLWAdded(CCipasComposerGLW* glw);
    //**Is emitted when a new composer picture has been added*/
    void composerPictureAdded( CCipasComposerPicture* picture );
	void composerScaleBarAdded( CCipasComposerScaleBar* scalebar );
	void composerShapeAdded( CCipasComposerShape* shape );
	void composerArrowAdded( CCipasComposerArrow* arrow );
	void composerLegendAdded( CCipasComposerLegend* legend );
    void itemRemoved( CComposerItem* );
    /**Current action (e.g. adding composer map) has been finished. The purpose of this signal is that
     QgsComposer may set the selection tool again*/
    void actionFinished();
	
	void emitMapFull();
	void emitMapZoonIn();
	void emitMapZoonOut();
	void emitMapMove(QPoint beginPoint , QPoint endPoint);
};

