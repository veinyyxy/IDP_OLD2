
#include <QPainter>
#include "CipasRenderContext.h"

CCipasRenderContext::CCipasRenderContext() : mPainter( 0 ),
	mCoordTransform( 0 ),
	mDrawEditingInformation( true ),
	mForceVectorOutput( false ),
	mRenderingStopped( false ),
	mScaleFactor( 1.0 ),
	mRasterScaleFactor( 1.0 )
	//mLabelingEngine( NULL )
{

}


CCipasRenderContext::~CCipasRenderContext()
{
	 delete mCoordTransform;
}


void CCipasRenderContext::setCoordinateTransform(CoordinateTransform* t )
{
	delete mCoordTransform;
	mCoordTransform = t;
}
