#pragma once
#include "tmui_global.h"
#include "CipasMapToPixel.h"
#include "CipasRectangle.h"
#include "CoordinateTransform.h"

using namespace goto_gis;
class QPainter;


class TMUI_EXPORT  CCipasRenderContext
{
public:
	CCipasRenderContext();
	~CCipasRenderContext();

	//getters

	QPainter* painter() {return mPainter;}
	const QPainter* constPainter() const { return mPainter; }

	const CoordinateTransform* coordinateTransform() const {return mCoordTransform;}

	const CCipasRectangle& extent() const {return mExtent;}

	const CCipasMapToPixel& mapToPixel() const {return mMapToPixel;}

	double scaleFactor() const {return mScaleFactor;}

	double rasterScaleFactor() const {return mRasterScaleFactor;}

	bool renderingStopped() const {return mRenderingStopped;}

	bool forceVectorOutput() const {return mForceVectorOutput;}

	bool drawEditingInformation() const {return mDrawEditingInformation;}

	double rendererScale() const {return mRendererScale;}

	//! Added in QGIS v1.4
	//QgsLabelingEngineInterface* labelingEngine() const { return mLabelingEngine; }

	//setters

	/**Sets coordinate transformation. QgsRenderContext takes ownership and deletes if necessary*/
	void setCoordinateTransform( CoordinateTransform* t );
	void setMapToPixel( const CCipasMapToPixel& mtp ) {mMapToPixel = mtp;}
	void setExtent( const CCipasRectangle& extent ) {mExtent = extent;}
	void setDrawEditingInformation( bool b ) {mDrawEditingInformation = b;}
	void setRenderingStopped( bool stopped ) {mRenderingStopped = stopped;}
	void setScaleFactor( double factor ) {mScaleFactor = factor;}
	void setRasterScaleFactor( double factor ) {mRasterScaleFactor = factor;}
	void setRendererScale( double scale ) {mRendererScale = scale;}
	void setPainter( QPainter* p ) {mPainter = p;}
	//! Added in QGIS v1.5
	void setForceVectorOutput( bool force ) {mForceVectorOutput = force;}
	//! Added in QGIS v1.4
	//void setLabelingEngine( QgsLabelingEngineInterface* iface ) { mLabelingEngine = iface; }

private:

	/**Painter for rendering operations*/
	QPainter* mPainter;

	/**For transformation between coordinate systems. Can be 0 if on-the-fly reprojection is not used*/
	CoordinateTransform* mCoordTransform;

	/**True if vertex markers for editing should be drawn*/
	bool mDrawEditingInformation;

	CCipasRectangle mExtent;

	/**If true then no rendered vector elements should be cached as image*/
	bool mForceVectorOutput;

	CCipasMapToPixel mMapToPixel;

	/**True if the rendering has been canceled*/
	bool mRenderingStopped;

	/**Factor to scale line widths and point marker sizes*/
	double mScaleFactor;

	/**Factor to scale rasters*/
	double mRasterScaleFactor;

	/**Map scale*/
	double mRendererScale;


};

