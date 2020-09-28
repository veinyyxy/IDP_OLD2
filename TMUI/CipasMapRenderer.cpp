#include <QPainter>

#include "CipasMapToPixel.h"
#include "CipasMapRenderer.h"


CCipasMapRenderer::CCipasMapRenderer()
{
	//mScaleCalculator = new QgsScaleCalculator;
	//mDistArea = new QgsDistanceArea;

	mDrawing = false;
	mOverview = false;

	// set default map units - we use WGS 84 thus use degrees
	setMapUnits( UnitType::Degrees );

	mSize = QSize( 0, 0 );

	mProjectionsEnabled = false;
	//mDestCRS = new QgsCoordinateReferenceSystem( GEOCRS_ID, QgsCoordinateReferenceSystem::InternalCrsId ); //WGS 84
	
	mOutputUnits = CCipasMapRenderer::Millimeters;

	//mLabelingEngine = NULL;
}


CCipasMapRenderer::~CCipasMapRenderer(void)
{
	//delete mScaleCalculator;
	//delete mDistArea;
	//delete mDestCRS;
	//delete mLabelingEngine;
}


CCipasRectangle CCipasMapRenderer::extent() const
{
	return mExtent;
}

void CCipasMapRenderer::updateScale()
{
	//mScale = mScaleCalculator->calculate( mExtent, mSize.width() );
}

bool CCipasMapRenderer::setExtent( const CCipasRectangle& extent )
{
	//remember the previous extent
	mLastExtent = mExtent;

	// Don't allow zooms where the current extent is so small that it
	// can't be accurately represented using a double (which is what
	// currentExtent uses). Excluding 0 avoids a divide by zero and an
	// infinite loop when rendering to a new canvas. Excluding extents
	// greater than 1 avoids doing unnecessary calculations.

	// The scheme is to compare the width against the mean x coordinate
	// (and height against mean y coordinate) and only allow zooms where
	// the ratio indicates that there is more than about 12 significant
	// figures (there are about 16 significant figures in a double).

	if ( extent.width()  > 0 &&
		extent.height() > 0 &&
		extent.width()  < 1 &&
		extent.height() < 1 )
	{
		// Use abs() on the extent to avoid the case where the extent is
		// symmetrical about 0.
		double xMean = ( qAbs( extent.xMinimum() ) + qAbs( extent.xMaximum() ) ) * 0.5;
		double yMean = ( qAbs( extent.yMinimum() ) + qAbs( extent.yMaximum() ) ) * 0.5;

		double xRange = extent.width() / xMean;
		double yRange = extent.height() / yMean;

		static const double minProportion = 1e-12;
		if ( xRange < minProportion || yRange < minProportion )
			return false;
	}

	mExtent = extent;
	if ( !extent.isEmpty() )
		adjustExtentToSize();
	return true;
}



void CCipasMapRenderer::setOutputSize( QSize size, int dpi )
{
	mSize = QSizeF( size.width(), size.height() );
//	mScaleCalculator->setDpi( dpi );
	adjustExtentToSize();
}

void CCipasMapRenderer::setOutputSize( QSizeF size, double dpi )
{
	mSize = size;
//	mScaleCalculator->setDpi( dpi );
	adjustExtentToSize();
}

double CCipasMapRenderer::outputDpi()
{
	//return mScaleCalculator->dpi();
	return 96.0;
}

QSize CCipasMapRenderer::outputSize()
{
	return mSize.toSize();
}

QSizeF CCipasMapRenderer::outputSizeF()
{
	return mSize;
}

void CCipasMapRenderer::adjustExtentToSize()
{
	double myHeight = mSize.height();
	double myWidth = mSize.width();

	CCipasMapToPixel newCoordXForm;

	if ( !myWidth || !myHeight )
	{
		mScale = 1;
		newCoordXForm.setParameters( 0, 0, 0, 0 );
		return;
	}

	// calculate the translation and scaling parameters
	// mapUnitsPerPixel = map units per pixel
	double mapUnitsPerPixelY = mExtent.height() / myHeight;
	double mapUnitsPerPixelX = mExtent.width() / myWidth;
	mMapUnitsPerPixel = mapUnitsPerPixelY > mapUnitsPerPixelX ? mapUnitsPerPixelY : mapUnitsPerPixelX;

	// calculate the actual extent of the mapCanvas
	double dxmin, dxmax, dymin, dymax, whitespace;

	if ( mapUnitsPerPixelY > mapUnitsPerPixelX )
	{
		dymin = mExtent.yMinimum();
		dymax = mExtent.yMaximum();
		whitespace = (( myWidth * mMapUnitsPerPixel ) - mExtent.width() ) * 0.5;
		dxmin = mExtent.xMinimum() - whitespace;
		dxmax = mExtent.xMaximum() + whitespace;
	}
	else
	{
		dxmin = mExtent.xMinimum();
		dxmax = mExtent.xMaximum();
		whitespace = (( myHeight * mMapUnitsPerPixel ) - mExtent.height() ) * 0.5;
		dymin = mExtent.yMinimum() - whitespace;
		dymax = mExtent.yMaximum() + whitespace;
	}
	// update extent
	mExtent.setXMinimum( dxmin );
	mExtent.setXMaximum( dxmax );
	mExtent.setYMinimum( dymin );
	mExtent.setYMaximum( dymax );

	// update the scale
	updateScale();

	//QgsDebugMsg( QString( "Scale (assuming meters as map units) = 1:%1" ).arg( mScale ) );

	newCoordXForm.setParameters( mMapUnitsPerPixel, dxmin, dymin, myHeight );
	mRenderContext.setMapToPixel( newCoordXForm );
	mRenderContext.setExtent( mExtent );
}


void CCipasMapRenderer::render( QPainter* painter )
{

}

void CCipasMapRenderer::setMapUnits( UnitType u )
{
	emit mapUnitsChanged();
}

void CCipasMapRenderer::onDrawingProgress( int current, int total )
{
	emit updateMap();
}

bool CCipasMapRenderer::hasCrsTransformEnabled()
{
	return mProjectionsEnabled;
}

void CCipasMapRenderer::setLayerSet( const QStringList& layers )
{
	mLayerSet = layers;
}

QStringList& CCipasMapRenderer::layerSet()
{
	return mLayerSet;
}
bool CCipasMapRenderer::mDrawing = false;
