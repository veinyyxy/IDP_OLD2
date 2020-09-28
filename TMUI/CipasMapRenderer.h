#pragma once

#include <QObject>
#include <QSize>
#include <QStringList>
#include <QVector>

#include "tmui_global.h"
#include "CipasRenderContext.h"
#include "CipasRectangle.h"

class QPainter;
class CCipasMapToPixel;


class TMUI_EXPORT CCipasMapRenderer :public QObject
{
	Q_OBJECT
public:
	 /** Map units that qgis supports
     * @note that QGIS < 1.4 api had only Meters, Feet, Degrees and UnknownUnit
     */
	enum UnitType
	{
		Meters = 0,
		Feet = 1,
		Degrees = 2, //for 1.0 api backwards compatibility
		DecimalDegrees = 2,
		DegreesMinutesSeconds = 4,
		DegreesDecimalMinutes = 5,
		UnknownUnit = 3
	} ;
	/**Output units for pen width and point marker width/height*/
	enum OutputUnits
	{
		Millimeters,
		Pixels
		//MAP_UNITS probably supported in future versions
	};


	CCipasMapRenderer();
	~CCipasMapRenderer();


	 //! starts rendering
    void render( QPainter* painter );

    //! sets extent and checks whether suitable (returns false if not)
    bool setExtent( const CCipasRectangle& extent );

    //! returns current extent
    CCipasRectangle extent() const;

    const CCipasMapToPixel* coordinateTransform() { return &( mRenderContext.mapToPixel() ); }

    double scale() const { return mScale; }
    /**Sets scale for scale based visibility. Normally, the scale is calculated automatically. This
     function is only used to force a preview scale (e.g. for print composer)*/
    void setScale( double scale ) {mScale = scale;}
    double mapUnitsPerPixel() const { return mMapUnitsPerPixel; }

    int width() const { return mSize.width(); };
    int height() const { return mSize.height(); };

    //! Recalculate the map scale
    void updateScale();

    ////! Return the measuring object
    ////! @deprecated
    //Q_DECL_DEPRECATED CCipasDistanceArea *distanceArea() { return mDistArea; }
    //UnitType mapUnits() const;
    void setMapUnits( UnitType u );

    //! sets whether map image will be for overview
    void enableOverviewMode( bool isOverview = true ) { mOverview = isOverview; }

    void setOutputSize( QSize size, int dpi );
    void setOutputSize( QSizeF size, double dpi );

    //!accessor for output dpi
    double outputDpi();
    //!accessor for output size
    QSize outputSize();
    QSizeF outputSizeF();
	/*
    //! transform extent in layer's CRS to extent in output CRS
    CCipasRectangle layerExtentToOutputExtent( CCipasMapLayer* theLayer, CCipasRectangle extent );

    //! transform coordinates from layer's CRS to output CRS
    CCipasPoint layerToMapCoordinates( CCipasMapLayer* theLayer, CCipasPoint point );

    //! transform coordinates from output CRS to layer's CRS
    CCipasPoint mapToLayerCoordinates( CCipasMapLayer* theLayer, CCipasPoint point );

    //! transform rect's coordinates from output CRS to layer's CRS
    CCipasRectangle mapToLayerCoordinates( CCipasMapLayer* theLayer, CCipasRectangle rect );

    //! sets whether to use projections for this layer set
    void setProjectionsEnabled( bool enabled );
	*/
    //! returns true if projections are enabled for this layer set
    bool hasCrsTransformEnabled();

    /** sets destination coordinate reference system
     * @note deprecated by qgis 1.7
     * @see setDestinationCrs
     */
    //Q_DECL_DEPRECATED void setDestinationSrs( const CipasCoordinateReferenceSystem& srs ) { setDestinationCrs( srs ); };

    /** returns CRS of destination coordinate reference system
     * @note deprecated by qgis 1.7
     * @see destinationCrs
     */
    //Q_DECL_DEPRECATED const CipasCoordinateReferenceSystem& destinationSrs() { return destinationCrs(); };

    //! sets destination coordinate reference system
   // void setDestinationCrs( const CipasCoordinateReferenceSystem& crs );

    //! returns CRS of destination coordinate reference system
    //const CCipasCoordinateReferenceSystem& destinationCrs();

    void setOutputUnits( OutputUnits u ) {mOutputUnits = u;}

    OutputUnits outputUnits() const {return mOutputUnits;}

    //! returns current extent of layer set
    CCipasRectangle fullExtent();

    //! returns current layer set
    QStringList& layerSet();

    //! change current layer set
    void setLayerSet( const QStringList& layers );

    //! updates extent of the layer set
    void updateFullExtent();

    //! read settings
   // bool readXML( QDomNode & theNode );

    //! write settings
    //bool writeXML( QDomNode & theNode, QDomDocument & theDoc );

    //! Accessor for render context
    CCipasRenderContext* rendererContext() {return &mRenderContext;}

    //! Labeling engine (NULL if there's no custom engine)
    //! \note Added in QGIS v1.4
    //CCipasLabelingEngineInterface* labelingEngine() { return mLabelingEngine; }

    //! Set labeling engine. Previous engine (if any) is deleted.
    //! Takes ownership of the engine.
    //! Added in QGIS v1.4
//    void setLabelingEngine( CCipasLabelingEngineInterface* iface );

  signals:

    void drawingProgress( int current, int total );

    void hasCrsTransformEnabled( bool flag );

    void destinationSrsChanged();

    void updateMap();

    void mapUnitsChanged();

    //! emitted when layer's draw() returned false
    //void drawError( CCipasMapLayer* );

  public slots:

    //! called by signal from layer current being drawn
    void onDrawingProgress( int current, int total );

  protected:

    //! adjust extent to fit the pixmap size
    void adjustExtentToSize();

    /** Convenience function to project an extent into the layer source
     * CRS, but also split it into two extents if it crosses
     * the +/- 180 degree line. Modifies the given extent to be in the
     * source CRS coordinates, and if it was split, returns true, and
     * also sets the contents of the r2 parameter
     */
   // bool splitLayersExtent( CCipasMapLayer* layer, CCipasRectangle& extent, CCipasRectangle& r2 );

    /**Creates an overlay object position manager subclass according to the current settings
    @note this method was added in version 1.1*/
    //CCipasOverlayObjectPositionManager* overlayManagerFromSettings();
	//重叠管理 很重要哦

  protected:

    //! indicates drawing in progress
    static bool mDrawing;

    //! map units per pixel
    double mMapUnitsPerPixel;

    //! Map scale at its current zool level
    double mScale;

    //! scale calculator
    //CCipasScaleCalculator * mScaleCalculator;

    //! current extent to be drawn
    CCipasRectangle mExtent;
    //
    /** Last extent to we drew so we know if we can
        used layer render caching or not. Note there are no
        accessors for this as it is intended to internal
        use only.
        @note added in QGIS 1.4 */
    CCipasRectangle mLastExtent;

    //! indicates whether it's map image for overview
    bool mOverview;

    QSizeF mSize;

    //! detemines whether on the fly projection support is enabled
    bool mProjectionsEnabled;

    //! destination spatial reference system of the projection
    //CCipasCoordinateReferenceSystem* mDestCRS;

    //! stores array of layers to be rendered (identified by string)
    QStringList mLayerSet;

    //! full extent of the layer set
    CCipasRectangle mFullExtent;

    //! tool for measuring
   // CCipasDistanceArea* mDistArea;

    //!Encapsulates context of rendering
    CCipasRenderContext mRenderContext;

    //!Output units
    OutputUnits mOutputUnits;
};

