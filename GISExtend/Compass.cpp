#include <osg/Drawable>
#include "Compass.h"


Compass::Compass(void)
{
}

Compass::~Compass(void)
{
}

void Compass::drawImplementation(osg::RenderInfo& renderInfo ) const
{
	osg::Matrix viewMatrix, projectionMatix;
	osg::Vec3 v3Eye, v3Up, v3Center;
	double dL, dR, dT, dM, dN, dF, dFovy;
	osg::View* pView = renderInfo.getView();
	
	pView->getCamera()->getProjectionMatrixAsFrustum(dL, dR, dT, dM, dN, dF);
	pView->getCamera()->getViewMatrixAsLookAt(v3Eye, v3Center, v3Up);

	osg::Viewport* pViewport = pView->getCamera()->getViewport();

	viewMatrix = pView->getCamera()->getViewMatrix();
	projectionMatix = pView->getCamera()->getProjectionMatrix();

	float iH = pViewport->height();
	float iW = pViewport->width();

	osg::Vec3 v3Screem(iW / 2, iH - iH / 2, 0);

	v3Screem = v3Screem * viewMatrix * projectionMatix;

	float fX1 = v3Screem.x();
	float fY1 = v3Screem.y();
	//pView->getCamera()->getProj
	//pView->getCamera()->getProjectionMatrixAsPerspective(dFovy, dR, dT, dM, dN, dF);
}

osg::Object* Compass::cloneType() const
{
	return 0;
}

osg::Object* Compass::clone( const osg::CopyOp& ) const
{
	return 0;
}
