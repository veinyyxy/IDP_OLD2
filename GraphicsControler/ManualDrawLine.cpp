#include <osgGA/GUIEventHandler>
#include <osg/Geode>
#include <osg/Array>
#include <osg/Geometry>
#include "ManualDrawLine.h"
#include <gl/GLU.h>

ManualDrawLine::ManualDrawLine(osg::Geode* pG, double dZ) 
	: m_pLayerGeode(new osg::Geode)
	, m_dZValue(dZ)
	, m_bStart(false)
	, m_dStartX(0)
	, m_dStartY(0)
	, m_dEndX(0)
	, m_dEndY(0)
{
	m_pWorldPoint = new osg::Vec3Array;
	m_pScreenPoint = new osg::Vec3Array;
	m_pLine = new osg::Geometry;
	m_pLayerGeode->addDrawable(m_pLine);
}


ManualDrawLine::~ManualDrawLine(void)
{
}

bool ManualDrawLine::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
{
	osg::Camera* pC = 0;
	if(aa.asView())
		pC = aa.asView()->getCamera();
	else
		return false;

	switch( ea.getEventType() )
	{

	case osgGA::GUIEventAdapter::FRAME:
		break;

	case osgGA::GUIEventAdapter::RESIZE:
		break;

	default:
		break;
	}

	switch( ea.getEventType() )
	{
	case osgGA::GUIEventAdapter::MOVE:
		if(m_bStart)
		{
			osg::Vec3 TempPoint;
			m_dEndX = ea.getX();
			m_dEndY = ea.getY();

			ScreenToGL(pC, osg::Vec3(m_dEndX, m_dEndY, 0), TempPoint);
			m_pWorldPoint->pop_back();
			m_pWorldPoint->push_back(TempPoint);

			m_pScreenPoint->pop_back();
			m_pScreenPoint->push_back(osg::Vec3(m_dEndX, m_dEndY, 0));

			m_pLine->removePrimitiveSet(0);
			m_pLine->setVertexArray(m_pWorldPoint);
			m_pLine->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,2));
		}
		break;
	case osgGA::GUIEventAdapter::DRAG:
		break;

	case osgGA::GUIEventAdapter::PUSH:
		break;

	case osgGA::GUIEventAdapter::RELEASE:
		if(!m_bStart)
		{
			osg::Vec3 TempPoint;
			osg::Node* pNode;
			m_dStartX = ea.getX();
			m_dStartY = ea.getY();
			m_dEndX = m_dStartX;
			m_dEndY = m_dStartY;
			osg::View* pV = aa.asView();
			osg::Camera* pC = pV->getCamera();
			ScreenToGL(pC, osg::Vec3(m_dStartX, m_dStartY, 0), TempPoint);
			m_pWorldPoint->clear();
			m_pWorldPoint->push_back(TempPoint);

			ScreenToGL(pC, osg::Vec3(m_dEndX, m_dEndY, 0), TempPoint);
			m_pWorldPoint->push_back(TempPoint);

			m_pScreenPoint->clear();
			m_pScreenPoint->push_back(osg::Vec3(m_dStartX, m_dStartY, 0));
			m_pScreenPoint->push_back(osg::Vec3(m_dEndX, m_dEndY, 0));
			/*pNode->getParentalNodePaths();
			osg::computeLocalToWorld(.at(0));*/
			//aa.asView()->

			m_bStart = true;
		}
		else
		{
			m_pLine->removePrimitiveSet(0);
			m_pLine->setVertexArray(m_pWorldPoint);
			m_pLine->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,2));
			m_bStart = false;
		}
		break;

	case osgGA::GUIEventAdapter::KEYDOWN:
		break;

	case osgGA::GUIEventAdapter::KEYUP:
		break;
	default:
		return false;
	}

	return true;
}

void ManualDrawLine::ScreenToGL(osg::Camera* pC, const osg::Vec3& screenPoint, osg::Vec3& glPoint )
{
#if 0
	osg::Matrixd rot90Matrix;
	double* pBuff = 0;
	GLint    viewport[4];
	//GLdouble modelview[16];
	//GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0f;
	GLdouble posX, posY, posZ;

	osg::Matrixd projMatr = pC->getProjectionMatrix();
	osg::Matrixd viewMatr = pC->getViewMatrix();
	osg::Viewport* viewPort = pC->getViewport();
	viewport[0] = viewPort->x();
	viewport[1] = viewPort->y();
	viewport[2] = viewPort->width();
	viewport[3] = viewPort->height();
	/*glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);*/

	winX = screenPoint.x();
	winY = viewport[3] - screenPoint.y();
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	//winZ = 0.95;
	gluUnProject(winX, winY, winZ, viewMatr.ptr(), projMatr.ptr(), viewport, &posX, &posY, &posZ);

	rot90Matrix.makeRotate(M_PI/180 * 90, 1, 0, 0);
	glPoint.set(posX, posY, posZ);
	glPoint = glPoint * rot90Matrix;
#else
	//double dLeft, dRight, dTop, dBottom, dNear, dFar;
	//osg::Vec3d v3Eye, v3Center, v3Up;
	osg::Vec3d glSceenPoint;
	osg::Matrixd rot90Matrix;
	rot90Matrix.makeRotate(3.1415926/180 * 90, 1, 0, 0);
	osg::Matrix mtxModelView= pC->getViewMatrix();
	osg::Matrix mtxProject = pC->getProjectionMatrix();
	//pC->getViewMatrixAsLookAt(v3Eye, v3Center, v3Up);
	//pC->getProjectionMatrixAsFrustum(dLeft, dRight, dBottom, dTop, dNear, dFar);
	osg::Viewport* pV = pC->getViewport();
	osg::Matrix mtxWindow = pV->computeWindowMatrix();
	glSceenPoint.set(screenPoint.x(), pV->height() - screenPoint.y(), 1.0);
	//osg::Vec3 vecWindowPt(ea.getX(), ea.getY(), 0.0f);

	osg::Matrix mtxMVPW = mtxModelView * mtxProject * mtxWindow;
	osg::Matrix mtxInverseMVPW = osg::Matrix::inverse(mtxMVPW);

	glPoint = screenPoint * mtxInverseMVPW;
	//glPoint = glPoint * rot90Matrix;
	glPoint.set(glPoint.x(), m_dZValue, glPoint.z());
#endif
}
