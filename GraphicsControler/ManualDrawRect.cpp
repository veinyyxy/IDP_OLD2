#include "ManualDrawRect.h"
#include <osgGA/GUIEventHandler>
#include <osgUtil/LineSegmentIntersector>
#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Array>
#include <osg/Geometry>
#include <osgUtil/SceneView>
#include <gl/GLU.h>

#define M_PI 3.1415926535897932384626433832795


ManualDrawRect::ManualDrawRect( osg::Geode* pG /*= 0*/, double dZ /*= 1.0*/ )
	: m_pLayerGeode(new osg::Geode)
	, m_dZValue(dZ)
	, m_bStart(false)
	, m_dStartX(0)
	, m_dStartY(0)
	, m_dEndX(0)
	, m_dEndY(0)
{
	m_pPointColor = new osg::Vec4Array;
	m_pWorldPoint = new osg::Vec3Array;
	m_pScreenPoint = new osg::Vec3Array;
	m_pRect = new osg::Geometry;
	m_pLayerGeode->addDrawable(m_pRect);

}

ManualDrawRect::~ManualDrawRect( void )
{

}

bool ManualDrawRect::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
{
	osgUtil::LineSegmentIntersector::Intersections intersections;
	osg::Camera* pC = 0;
	osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(aa.asView());

	if(pViewer)
		pC = pViewer->getCamera();
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

			m_pWorldPoint->pop_back();
			m_pWorldPoint->pop_back();
			m_pWorldPoint->pop_back();

			m_pPointColor->pop_back();
			m_pPointColor->pop_back();
			m_pPointColor->pop_back();

			ScreenToGL(pC, osg::Vec3(m_dEndX,m_dStartY , 0.8), TempPoint);
			m_pWorldPoint->push_back(osg::Vec3(TempPoint.x(),TempPoint.z(),TempPoint.y()));
			m_pPointColor->push_back(osg::Vec4(1,0,0,1));

			ScreenToGL(pC, osg::Vec3(m_dEndX,m_dEndY , 0.8), TempPoint);
			m_pWorldPoint->push_back(osg::Vec3(TempPoint.x(),TempPoint.z(),TempPoint.y()));
			m_pPointColor->push_back(osg::Vec4(1,0,0,1));

			ScreenToGL(pC, osg::Vec3(m_dStartX,m_dEndY , 0.8), TempPoint);
			m_pWorldPoint->push_back(osg::Vec3(TempPoint.x(),TempPoint.z(),TempPoint.y()));
			m_pPointColor->push_back(osg::Vec4(1,0,0,1));

			m_pRect->removePrimitiveSet(0);
			m_pRect->setVertexArray(m_pWorldPoint);
			m_pRect->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
			m_pRect->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP,0,4));
			m_pRect->setColorArray(m_pPointColor);
			m_rect.StartX = m_dStartX;
			m_rect.StartY = m_dStartY;
			m_rect.EndX = m_dEndX;
			m_rect.EndY = m_dEndY;
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
			m_dStartX = ea.getX();
			m_dStartY = ea.getY();
			m_dEndX = m_dStartX;
			m_dEndY = m_dStartY;
			osg::View* pV = aa.asView();
			osg::Camera* pC = pV->getCamera();
			ScreenToGL(pC, osg::Vec3(m_dStartX,m_dStartY , 0.8), TempPoint);
			m_pWorldPoint->clear();
			m_pPointColor->clear();
			m_pWorldPoint->push_back(osg::Vec3(TempPoint.x(),TempPoint.z(),TempPoint.y()));
			m_pPointColor->push_back(osg::Vec4(1,0,0,1));
			ScreenToGL(pC, osg::Vec3(m_dEndX,m_dStartY , 0.8), TempPoint);
			m_pWorldPoint->push_back(osg::Vec3(TempPoint.x(),TempPoint.z(),TempPoint.y()));
			m_pPointColor->push_back(osg::Vec4(1,0,0,1));
			ScreenToGL(pC, osg::Vec3(m_dEndX,m_dEndY ,0.8 ), TempPoint);
			m_pWorldPoint->push_back(osg::Vec3(TempPoint.x(),TempPoint.z(),TempPoint.y()));
			m_pPointColor->push_back(osg::Vec4(1,0,0,1));
			ScreenToGL(pC, osg::Vec3(m_dStartX,m_dEndY , 0.8), TempPoint);
			m_pWorldPoint->push_back(osg::Vec3(TempPoint.x(),TempPoint.z(),TempPoint.y()));
			m_pPointColor->push_back(osg::Vec4(1,0,0,1));
			m_bStart = true;
			m_rect.StartX = m_dStartX;
			m_rect.StartY = m_dStartY;
			m_rect.EndX = m_dEndX;
			m_rect.EndY = m_dEndY;
		}
		else
		{
			m_pRect->removePrimitiveSet(0);
			m_pRect->setVertexArray(m_pWorldPoint);
			m_pRect->setColorArray(m_pPointColor);
			m_pRect->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
			m_pRect->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP,0,4));
			m_bStart = false;

			m_rect.StartX = m_dStartX;
			m_rect.StartY = m_dStartY;
			m_rect.EndX = m_dEndX;
			m_rect.EndY = m_dEndY;
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

void ManualDrawRect::ScreenToGL( osg::Camera* pC, const osg::Vec3& screenPoint, osg::Vec3& glPoint )
{
	osg::Vec3d glSceenPoint;
	osg::Matrixd rot90Matrix;
	rot90Matrix.makeRotate(M_PI/180 * 90, 1, 0, 0);
	osg::Matrix mtxModelView= pC->getViewMatrix();
	osg::Matrix mtxProject = pC->getProjectionMatrix();

	osg::Viewport* pV = pC->getViewport();
	osg::Matrix mtxWindow = pV->computeWindowMatrix();
	glSceenPoint.set(screenPoint.x(), pV->height() - screenPoint.y(), 1.0);

	osg::Matrix mtxMVPW = mtxModelView * mtxProject;
	mtxMVPW.postMult(mtxWindow);
	osg::Matrix mtxInverseMVPW = osg::Matrix::inverse(mtxMVPW);

	glPoint = screenPoint * mtxInverseMVPW;
	glPoint.set(glPoint.x(), m_dZValue, glPoint.z());
}

void ManualDrawRect::RemoveRect()
{
	m_pLayerGeode->removeDrawable(m_pRect);
}

void ManualDrawRect::AddBlankRect()
{
	m_pRect = new osg::Geometry;
	m_pLayerGeode->addDrawable(m_pRect);
}

