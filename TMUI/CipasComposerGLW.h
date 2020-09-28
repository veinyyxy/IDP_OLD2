#pragma once

#include <osgGA/GUIEventHandler>
#include <QtGui>
#include <QFile>
#include <QImage>
#include <QtCore>

#include "CipasRectangle.h"
#include "GraticuleCoordinates.h"
#include "tmui_global.h"
#include "composeritem.h"
#include "AdapterWidget.h"

class PrintImageToQImage;

class TMUI_EXPORT CCipasComposerGLW :	public CComposerItem
{
	Q_OBJECT
public:
	enum GridAnnotationDirection
	{
		Horizontal = 0,
		Vertical,
		HorizontalAndVertical,
		BoundaryDirection
	};

	enum GridStyle
	{
		Solid = 0, //solid lines  
		Cross //only draw line crossings
	};

	enum AnnotationStyle
	{
		NoStyle = 0,
		Degree,
		DegreeMinuteSecond
	};

	/**Enum for different frame borders*/
	enum Border
	{
		Left,
		Right,
		Bottom,
		Top
	};
	bool LabelAxesTop;
	bool LabelAxesLeft;
	bool LabelAxesBottom;
	bool LabelAxesRight;


	enum GridAnnotationPosition
	{
		InsideMapFrame = 0,
		OutsideMapFrame
	};

	enum ImageKeepRadio
	{
		Keep = 0,
		NoKeep
	};

	CCipasComposerGLW(CCipasComposition *composition);
	CCipasComposerGLW(qreal x, qreal y, qreal width, qreal height,CCipasComposition *composition);
	~CCipasComposerGLW(void);

	/**Unique identifier*/
	int mId;
	/** \brief Get identification number*/
	int id() const {return mId;}
	/** return correct graphics item type. Added in v1.7 */
	virtual int type() const { return ComposerGLW; }
	void setImage(QImage *img);
	/**设置 glview 的范围，增加（初始化时候）使用**/
	void setGLExtent( double xmin = 0, double ymin = 0, double xmax = 0, double ymax = 0 );
	void setGLExtent2( double xmin = 0, double ymin = 0, double xmax = 0, double ymax = 0 ,ImageKeepRadio b=Keep);
	/**转化比例**/
	double Ratio(){return mRatio;}
	/**刷新一下**/
	void cache();
	/**Reimplementation of QCanvasItem::paint*/
	void paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget );
	void paintCommon( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget );
	void paintAdjustExtent( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget );
    /**Sets this items bound in scene coordinates such that 1 item size units
       corresponds to 1 scene size unit and resizes the svg symbol / image*/
    void setSceneRect( const QRectF& rectangle );
	/**Move content of glv
    @param dx move in x-direction (item and canvas coordinates)
    @param dy move in y-direction (item and canvas coordinates)*/
    void moveContent( double dx, double dy );
	/**Zoom content of map
     @param delta value from wheel event that describes magnitude and direction (positive /negative number)
    @param x x-coordinate of mouse position in item coordinates
    @param y y-coordinate of mouse position in item coordinates*/
    void zoomContent( int delta, double x, double y );
	/**Sets offset values to shift image (useful for live updates when moving item content)*/
	void setOffset( double xOffset, double yOffset );
	 /** \brief Draw to paint device
        @param painter painter
        @param extent map extent
        @param size size in scene coordinates
        @param dpi scene dpi*/
    void draw( QPainter *painter, const CCipasRectangle& extent, const QSizeF& size, double dpi );
	/**Sets this items bound in scene coordinates such that 1 item size units
     corresponds to 1 scene size unit. Also, the shape is scaled*/
	inline double getWidth(){return mGLWidth;}
	inline double getHeight(){return mGLHeight;}
	/*inline void GLScene(CIPScene* pCS){m_pScene = pCS;}*/

	void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
	/** stores state in Dom node
     * @param elem is Dom element corresponding to 'Composer' tag
     * @param doc Dom document
     */
    bool writeXML( QDomElement& elem, QDomDocument & doc ) const;
	bool readXML( const QDomElement& itemElem, const QDomDocument& doc );
	inline void setGridPen( const QPen& p ) { mGridPen = p; }
	void drawCoordinateAnnotations( QPainter* p, const QList< QPair< double, QLineF > >& hLines, 
		const QList< QPair< double, QLineF > >& vLines );
	void drawCoordinateAnnotation( QPainter* p, const QPointF& pos, QString annotationString ,Qt::Orientation orient=Qt::Horizontal);
	void drawAnnotation( QPainter* p, const QPointF& pos, int rotation, const QString& annotationText );
	QPolygonF transformedMapPolygon() const;
	
	 /**Sets flag if grid annotation should be shown
    @note this function was added in version 1.4*/
    inline void setShowGridAnnotation( bool show ) {mShowGridAnnotation = show;}
    inline bool showGridAnnotation() const {return mShowGridAnnotation;}
	 /**Sets coordinate interval in x-direction for composergrid.
        @note this function was added in version 1.4*/
    inline void setGridIntervalX( double interval ) { mGridIntervalX = interval;}
    inline double gridIntervalX() const { return mGridIntervalX; }
	  /**Sets coordinate interval in y-direction for composergrid.
    @note this function was added in version 1.4*/
    inline void setGridIntervalY( double interval ) { mGridIntervalY = interval;}
    inline double gridIntervalY() const { return mGridIntervalY; }
	/**Sets length of the cros segments (if grid style is cross)
    @note this function was added in version 1.4*/
    inline void setCrossLength( double l ) {mCrossLength = l;}
    inline double crossLength() {return mCrossLength;}
	 /**Scales a composer map shift (in MM) and rotates it by mRotation
        @param xShift in: shift in x direction (in item units), out: xShift in map units
        @param yShift in: shift in y direction (in item units), out: yShift in map units*/
    void transformShift( double& xShift, double& yShift ) const;
	/**Returns the conversion factor map units -> mm*/
	double mapUnitsToMM() const;
	 /**Returns the polygon of the map extent. If rotation == 0, the result is the same as mExtent
    @param poly out: the result polygon with the four corner points. The points are clockwise, starting at the top-left point
    @return true in case of success*/
    void mapPolygon( QPolygonF& poly ) const;
	/**Sets x-coordinate offset for composer grid
    @note this function was added in version 1.4*/
    inline void setGridOffsetX( double offset ) { mGridOffsetX = offset; }
    inline double gridOffsetX() const { return mGridOffsetX; }
	/**Sets y-coordinate offset for composer grid
    @note this function was added in version 1.4*/
    inline void setGridOffsetY( double offset ) { mGridOffsetY = offset; }
    inline double gridOffsetY() const { return mGridOffsetY; }
	/**Returns the item border of a point (in item coordinates)*/
	CCipasComposerGLW::Border borderForLineCoord( const QPointF& p ,Qt::Orientation orient=Qt::Horizontal) const;
	 /**Sets font for grid annotations
    @note this function was added in version 1.4*/
    inline void setGridAnnotationFont( const QFont& f ) { mGridAnnotationFont = f; }
    inline QFont gridAnnotationFont() const { return mGridAnnotationFont; }
	 /**Sets position of grid annotations. Possibilities are inside or outside of the map frame
    @note this function was added in version 1.4*/
    inline void setGridAnnotationPosition( GridAnnotationPosition p ) {mGridAnnotationPosition = p;}
    inline GridAnnotationPosition gridAnnotationPosition() const {return mGridAnnotationPosition;}
	inline void setGridPosition(GridAnnotationPosition p ){mGridPosition = p;}
	inline GridAnnotationPosition gridPosition(){return mGridPosition;}
	/**Transforms map coordinates to item coordinates (considering rotation and move offset)*/
	QPointF mapToItemCoords( const QPointF& mapCoords ) const;
	QPointF mapToItemCoords2( const QPointF& mapCoords ) const;
	 /**Sets distance between map frame and annotations
    @note this function was added in version 1.4*/
    inline void setAnnotationFrameDistance( double d ) {mAnnotationFrameDistance = d;}
    inline double annotationFrameDistance() const {return mAnnotationFrameDistance;}
	/**Returns extent that considers mOffsetX / mOffsetY (during content move)*/
	CCipasRectangle transformedExtent() const;
	 /**Enables a coordinate grid that is shown on top of this composermap.
        @note this function was added in version 1.4*/
    inline void setGridEnabled( bool enabled ) {mGridEnabled = enabled;}
    inline bool gridEnabled() const { return mGridEnabled; }
	 /**Sets grid annotation direction. Can be horizontal, vertical, direction of axis and horizontal and vertical
    @note this function was added in version 1.4*/
    void setGridAnnotationDirection( GridAnnotationDirection d ) {mGridAnnotationDirection = d;}
    GridAnnotationDirection gridAnnotationDirection() const {return mGridAnnotationDirection;}
	 /**Sets coordinate precision for grid annotations
    @note this function was added in version 1.4*/
    void setGridAnnotationPrecision( int p ) {mGridAnnotationPrecision = p;}
    int gridAnnotationPrecision() const {return mGridAnnotationPrecision;}
	/**Sets with of grid pen
    @note this function was added in version 1.4*/
    void setGridPenWidth( double w );
    /**Sets the color of the grid pen
    @note this function was added in version 1.4*/
	/**Sets the pen to draw composer grid
    @note this function was added in version 1.4*/
    //inline void setGridPen( const QPen& p ) { mGridPen = p; }
    QPen gridPen() const { return mGridPen; }
	inline CCipasRectangle extent() const {return mExtent;}
	 /**Sets coordinate grid style to solid or cross
        @note this function was added in version 1.4*/
    inline void setGridStyle( GridStyle style ) {mGridStyle = style;}
    inline GridStyle gridStyle() const { return mGridStyle; }
	inline void setAnnotationStyle ( AnnotationStyle style) {mAnnotionStyle = style;}
	inline AnnotationStyle annotionStyle() const { return mAnnotionStyle ;}

	void updateComposition();

	void setInOrOutSideFrame(int i=0);
	int getInOrOutSideFrame();
	void setAdjustExtent(bool b);
	void setInputExtent(double xmin = 0, double ymin = 0, double xmax = 0, double ymax = 0 ,ImageKeepRadio b=Keep );

	inline void setGridAnnotationColor( const QColor& c  ){mAnntationColor=c;}
	inline QColor getGridAnnotationColor() const { return mAnntationColor; }
	void setReCalcGrid(bool b) ;		/*设置是否重新计算格网，如果是 进行计算，并将标志 置为 否*/
	void SetOsgViewer(ViewerQT* pViewer){
		m_pViewerImage = pViewer;
	}
	inline ViewerQT* GetViewerQT(){return m_pViewerImage;}
	unsigned long long GetMapScale();
signals:
	void extentChanged();
	void myImages(float fWidth, float fHeight);
public slots:
	/***/
	void updateImage(QImage *img);
	void updateFuckImage();
private:
	double miGraduationLineLength;
	  //default constructor is forbidden
	  CCipasComposerGLW();
	  void SetGLWImg();
	  QString formateAnnotionString(QString inputs);
	  /**Calculates bounding rect for image such that aspect ratio is correct*/
	  QRectF boundedImageRect( double deviceWidth, double deviceHeight );
	  void updateBoundingRect();
	  double maxExtension() const;
	  QImage mImage;
	  QImage mGLWImg;
	 
	  QFile mSourceFile;

	  CCipasRectangle mExtent;

	  // Cache used in composer preview
	  QImage mCacheImage;

	  // Is cache up to date
	  bool mCacheUpdated;

	  /** \brief Preview style  */
//	  PreviewMode mPreviewMode;

	  double mRatio;

	  /**Width of the picture (in mm)*/
	  double mPictureWidth;
	  /**Height of the picture (in mm)*/
	  double mPictureHeight;
	  /** \brief set to true if in state of drawing. Concurrent requests to draw method are returned if set to true */
	  bool mDrawing;

	  double mGLWidth;
	  double mGLHeight;
	  /**Offset in x direction for showing map cache image*/
	  double mXOffset;
	  /**Offset in y direction for showing map cache image*/
	  double mYOffset;
	  bool mGridEnabled;
	  QPen mGridPen;
	  /**Digits after the dot*/
	  int mGridAnnotationPrecision;
	  /**True if coordinate values should be drawn*/
	  bool mShowGridAnnotation;
	  /**Annotation can be horizontal / vertical or different for axes*/
	  GridAnnotationDirection mGridAnnotationDirection;
	  /**Solid or crosses  由于都来自图层 实际使用的是0 实线 1 虚线*/
	  GridStyle mGridStyle;
	  /*不带，或者 度分秒*/
	  AnnotationStyle mAnnotionStyle;
	  /**Grid line interval in x-direction (map units)*/
	  double mGridIntervalX;
	  /**The length of the cross sides for mGridStyle Cross*/
	  /**Grid line interval in y-direction (map units)*/
	  double mGridIntervalY;
	  double mCrossLength;
	  /**Grid line offset in x-direction*/
	  double mGridOffsetX;
	  /**Grid line offset in y-direction*/
	  double mGridOffsetY;
	  /**Font for grid line annotation*/
	  QFont mGridAnnotationFont;
	  /**Annotation position inside or outside of map frame*/
	  GridAnnotationPosition mGridAnnotationPosition;
	  GridAnnotationPosition mGridPosition;
	  /**Distance between map frame and annotation*/
	  double mAnnotationFrameDistance;
	  double mDisToFrame;
	  /**Flag if layers to be displayed should be read from qgis canvas (true) or from stored list in mLayerSet (false)*/
	  bool mKeepLayerSet;
	  /**Current bounding rectangle. This is used to check if notification to the graphics scene is necessary*/
	  QRectF mCurrentRectangle;
	  /**Stored layer list (used if layer live-link mKeepLayerSet is disabled)*/
	  QStringList mLayerSet;
	 
	
	  //是否根据外部传入经纬度调整四至范围
	  bool isAdjustExtent;
	  QPointF mLeftTop;
	  QPointF mRightBottom;
	  /*格网标注颜色*/
	  QColor mAnntationColor;
	  bool mReCalcGrid;
	  /*格网线段*/
	  QList< QPair< double, QLineF > > verticalLines;
	  QList< QPair< double, QLineF > > horizontalLines;
	  bool isFirstCalcGrid;
	  double boundRectWidthMM  ;
	  double boundRectHeightMM  ;
	  ViewerQT* m_pViewerImage;
};

