#pragma once
#include <QtGui/QGraphicsScene>
#include <QLinkedList>
#include <QDomDocument>
#include <QDomElement>
#include <QUndoStack>

#include "tmui_global.h"
#include "CipasComposerItemCommand.h"


class CComposerItem;
class CCipasComposition;

class CCipasComposerGLW;
class CCipasComposerMap;
class CCipasComposerLabel;

class CCipasPaperItem;
class QDomElement;


class TMUI_EXPORT CCipasComposition :public QGraphicsScene
{
	Q_OBJECT
public:
    /** \brief Plot type */
    enum PlotStyle
    {
      Preview = 0, // Use cache etc
      Print,       // Render well
      Postscript   // Fonts need different scaling!
    };

    /**Style to draw the snapping grid*/
    enum GridStyle
    {
      Solid,
      //Dots,
      Crosses
    };

    CCipasComposition( );
    ~CCipasComposition();

    /**Changes size of paper item*/
    void setPaperSize( double width, double height );

    /**Returns height of paper item*/
    double paperHeight() const;

    /**Returns width of paper item*/
    double paperWidth() const;

    void setSnapToGridEnabled( bool b );
    bool snapToGridEnabled() const {return mSnapToGrid;}

    void setSnapGridResolution( double r );
    double snapGridResolution() const {return mSnapGridResolution;}

    void setSnapGridOffsetX( double offset );
    double snapGridOffsetX() const {return mSnapGridOffsetX;}

    void setSnapGridOffsetY( double offset );
    double snapGridOffsetY() const {return mSnapGridOffsetY;}

    void setGridPen( const QPen& p );
    const QPen& gridPen() const {return mGridPen;}

    void setGridStyle( GridStyle s );
    GridStyle gridStyle() const {return mGridStyle;}

    /**Returns pointer to undo/redo command storage*/
    QUndoStack* undoStack() { return &mUndoStack; }

    /**Returns the topmost composer item. Ignores mPaperItem*/
    CComposerItem* composerItemAt( const QPointF & position );

    QList<CComposerItem*> selectedComposerItems();

	QList<CComposerItem*> inputComposerGLWs();

    /**Returns pointers to all composer maps in the scene*/
    QList<const CCipasComposerMap*> composerMapItems() const;

    /**Returns the composer map with specified id
     @return id or 0 pointer if the composer map item does not exist*/
    const CCipasComposerMap* getComposerMapById( int id ) const;

	  /**Returns pointers to all composer maps in the scene*/
    QList<const CCipasComposerGLW*> composerGLWItems() const;

    /**Returns the composer map with specified id
     @return id or 0 pointer if the composer map item does not exist*/
    const CCipasComposerGLW* getComposerGLWById( int id ) const;

	
    QList<const CCipasComposerLabel*> composerLabelItems() const;
  
    const CCipasComposerLabel* getComposerLabelById(QString  id ) const;

    int printResolution() const {return mPrintResolution;}
    void setPrintResolution( int dpi ) {mPrintResolution = dpi;}

    bool printAsRaster() const {return mPrintAsRaster;}
    void setPrintAsRaster( bool enabled ) { mPrintAsRaster = enabled; }

    /**Returns pointer to map renderer of qgis map canvas*/
    //QgsMapRenderer* mapRenderer() {return mMapRenderer;}

    CCipasComposition::PlotStyle plotStyle() const {return mPlotStyle;}
    void setPlotStyle( CCipasComposition::PlotStyle style ) {mPlotStyle = style;}

    /**Returns the pixel font size for a font that has point size set.
     The result depends on the resolution (dpi) and of the preview mode. Each item that sets
    a font should call this function before drawing text*/
    int pixelFontSize( double pointSize ) const;

    /**Does the inverse calculation and returns points for pixels (equals to mm in CCipasComposition)*/
    double pointFontSize( int pixelSize ) const;

    /**Writes settings to xml (paper dimension)*/
    bool writeXML( QDomElement& composerElem, QDomDocument& doc );

    /**Reads settings from xml file*/
    bool readXML( const QDomElement& compositionElem, const QDomDocument& doc );

    /**Adds item to z list. Usually called from constructor of CComposerItem*/
    void addItemToZList( CComposerItem* item );
    /**Removes item from z list. Usually called from destructor of CComposerItem*/
    void removeItemFromZList( CComposerItem* item );

    //functions to move selected items in hierarchy
    void raiseSelectedItems();
    void raiseItem( CComposerItem* item );
    void lowerSelectedItems();
    void lowerItem( CComposerItem* item );
    void moveSelectedItemsToTop();
    void moveItemToTop( CComposerItem* item );
    void moveSelectedItemsToBottom();
    void moveItemToBottom( CComposerItem* item );

    //functions to align selected items
    void alignSelectedItemsLeft();
    void alignSelectedItemsHCenter();
    void alignSelectedItemsRight();
    void alignSelectedItemsTop();
    void alignSelectedItemsVCenter();
    void alignSelectedItemsBottom();

    /**Sorts the zList. The only time where this function needs to be called is from QgsComposer
     after reading all the items from xml file*/
    void sortZList();

    /**Snaps a scene coordinate point to grid*/
    QPointF snapPointToGrid( const QPointF& scenePoint ) const;

    /**Allocates new item command and saves initial state in it
      @param item target item
      @param commandText descriptive command text
      @param c context for merge commands (unknown for non-mergeable commands)*/
    void beginCommand( CComposerItem* item, const QString& commandText, CipasComposerMergeCommand::Context c = CipasComposerMergeCommand::Unknown );

    /**Saves end state of item and pushes command to the undo history*/
    void endCommand();
    /**Deletes current command*/
    void cancelCommand();

	inline QPen* BoundingPen() {return &mPaperBoundingPen;}
	inline void SetLineNum(int iNum){mLineNum = iNum;}
	inline int LineNum(){return mLineNum;}
	inline  void SetLineInv(double iInv){mLineInv = iInv;}
	inline  double LineInv(){return mLineInv;}
	inline void SetIsDrawBounding(bool bI){mbIsDrawBounding = bI;}
	inline bool IsDrawBounding(){return mbIsDrawBounding;}

	inline void setMarginX(double d){mdMarginX=d;}
	inline double getMarginX(){return mdMarginX;}

	inline void setMarginY(double d){mdMarginY=d;}
	inline double getMarginY(){return mdMarginY;}

	inline CComposerItem*  getCurrentItem(){return currentItem;};
	inline void setCurrentItem(CComposerItem* item ){currentItem=item;};

  private:
    CCipasComposition::PlotStyle mPlotStyle;
    CCipasPaperItem* mPaperItem;
	CComposerItem* currentItem ;
    /**Maintains z-Order of items. Starts with item at position 1 (position 0 is always paper item)*/
    QLinkedList<CComposerItem*> mItemZList;

    /**Dpi for printout*/
    int mPrintResolution;

    /**Flag if map should be printed as a raster (via QImage). False by default*/
    bool mPrintAsRaster;

    /**Parameters for snap to grid function*/
    bool mSnapToGrid;
    double mSnapGridResolution;
    double mSnapGridOffsetX;
    double mSnapGridOffsetY;
    QPen mGridPen;
    GridStyle mGridStyle;

    QUndoStack mUndoStack;

    CipasComposerItemCommand* mActiveCommand;

    //CCipasComposition(); //default constructor is forbidden

    /**Reset z-values of items based on position in z list*/
    void updateZValues();

    /**Returns the bounding rectangle of the selected items in scene coordinates
     @return 0 in case of success*/
    int boundingRectOfSelectedItems( QRectF& bRect );

    void loadGridAppearanceSettings();
    void saveGridAppearanceSettings();
	QPen mPaperBoundingPen;
	int mLineNum;
	double mLineInv;
	QColor mPaperLineColor;
	Qt::PenStyle mPenSytle;
	Qt::PenJoinStyle mPenJoinStyle;
	qreal mDashOffset;
	bool mbIsDrawBounding;
	double mdMarginX;
	double mdMarginY;
	QImage m_bgkImage;
  signals:
    void paperSizeChanged();
};

