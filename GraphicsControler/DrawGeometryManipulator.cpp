#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Material>
#include <osgDB/ReadFile>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/TrackballManipulator>
#include <osgUtil/SmoothingVisitor>
#include "DrawGeometryData.h"
#include "DrawGeometryManipulator.h"


DrawGeometryManipulator::DrawGeometryManipulator(DrawGeometryData* pDGD) : m_bStart(false), m_Data(pDGD)
{
	
}

DrawGeometryManipulator::~DrawGeometryManipulator()
{

}

void DrawGeometryManipulator::setByMatrix( const osg::Matrixd& matrix )
{
	m_Matrix = matrix;
	m_InverseMatrix = osg::Matrixd::inverse(matrix);
}

void DrawGeometryManipulator::setByInverseMatrix( const osg::Matrixd& matrix )
{
	m_InverseMatrix = matrix;
}

osg::Matrixd DrawGeometryManipulator::getMatrix() const
{
	return m_Matrix;
}

osg::Matrixd DrawGeometryManipulator::getInverseMatrix() const
{
	return m_InverseMatrix;
}

bool DrawGeometryManipulator::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	int iEvent = -1;
	bool bIntersention = false;
	int backspaceKey = -1;
	osg::Vec3 CanvasPoint;
	osg::Vec3 inPoint, outPoint;
	osgUtil::LineSegmentIntersector::Intersections lIntersentions;
	osg::Camera* pC = 0;
	if(!m_Data) return false;
	osg::View* pView = us.asView();
	osgViewer::Viewer* pViewer = 0;
	if(pView)
	{
		pViewer = dynamic_cast<osgViewer::Viewer*>(pView);
		pC = pView->getCamera();
	}
	else
		return false;

	if(!pViewer) return false;

	float fx = ea.getX();
	float fy = ea.getY();
	//pViewer->computeIntersections(fx, fy, m_nodePath, lIntersentions, 0x4D01F68)
#if 0
	if (pViewer->computeIntersections(fx, fy, lIntersentions, 0x4D01F68))
	{
		osgUtil::LineSegmentIntersector::Intersection first = *(lIntersentions.begin());
		CanvasPoint = first.getLocalIntersectPoint();
		//CanvasPoint.set(CanvasPoint.x(), CanvasPoint.z(), -0.001);

		return m_Data->handle(CanvasPoint, ea);
	}
#else
	ScreenToWorld(pViewer->getCamera(), osg::Vec3(fx, fy, 0), CanvasPoint, 0);
	m_Data->SetCamera(pViewer->getCamera());
	m_Data->SetViewer(pViewer);
	m_Data->handle(CanvasPoint, ea);
	
#endif
	return true;
}

void DrawGeometryManipulator::ScreenToWorld( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out, float fZ)
{
	osg::Matrix VPW = pCamera->getViewMatrix() *
		pCamera->getProjectionMatrix() *
		pCamera->getViewport()->computeWindowMatrix();
	osg::Matrix inverseVPW;
	inverseVPW.invert(VPW);
	v3Out = v3In  * inverseVPW;
	v3Out.set(v3Out.x(), fZ, v3Out.z());
	//v3Out = v3Out * m_m4Rotate;
}

void DrawGeometryManipulator::WorldToScreen( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out )
{
	osg::Vec3 v3localIn;
	v3localIn.set(v3In.x(), v3In.y(), v3In.z());
	osg::Matrix VPW = pCamera->getViewMatrix() * 
		pCamera->getProjectionMatrix() *
		pCamera->getViewport()->computeWindowMatrix();

	v3Out = v3localIn * VPW;
	v3Out.set(v3Out.x(), v3Out.y(), 1);
}

osg::Node* DrawGeometryManipulator::CreateCanvas( float fr, osg::Vec3 center, const osg::Vec4& v4C /*= osg::Vec4(1, 1, 1, 1)*/ )
{
	m_pCanvasGeode = new osg::Geode;
	m_pCanvasGeode->setNodeMask(0x4D01F68);
	m_pCanvas = new osg::Geometry;
	m_pCanvas->setName("Draw_Geometry_Canvas");
	osg::Vec3Array* pVertex = new osg::Vec3Array;

	pVertex->push_back(osg::Vec3(center.x()-fr, center.y()+fr,  0));
	pVertex->push_back(osg::Vec3(center.x()-fr, center.y()-fr,   0));
	pVertex->push_back(osg::Vec3(center.x()+fr, center.y()-fr,  0));
	pVertex->push_back(osg::Vec3(center.x()+fr, center.y()+fr, 0));

	osg::Vec4Array* pColors = new osg::Vec4Array;
	pColors->push_back(v4C/*osg::Vec4(0.5, 0.5, 1, 1)*/);

	m_pCanvas->setVertexArray(pVertex);
	m_pCanvas->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	m_pCanvas->setColorArray(pColors);
	m_pCanvas->setColorBinding(osg::Geometry::BIND_OVERALL);

	osgUtil::SmoothingVisitor::smooth(*m_pCanvas);
	m_pCanvasGeode->addDrawable(m_pCanvas);

	return m_pCanvasGeode;
}

void DrawGeometryManipulator::SetCanvasColor( const osg::Vec4& v4c )
{
	osg::Vec4Array* pColors = dynamic_cast<osg::Vec4Array*>(m_pCanvas->getColorArray());
	pColors->at(0).set(v4c.x(), v4c.y(), v4c.z(), v4c.w());

	m_pCanvas->dirtyDisplayList();
	m_pCanvas->dirtyBound();
}

void DrawGeometryManipulator::SetCanvasZ( float fZ )
{
	osg::Vec3Array* pVertexs = dynamic_cast<osg::Vec3Array*>(m_pCanvas->getVertexArray());

	for(osg::Vec3Array::size_type i = 0; i < pVertexs->size(); i++)
	{
		osg::Vec3 pp = pVertexs->at(i);
		pp.set(pp.x(), pp.y(), fZ);
		(*pVertexs)[i] = pp;
	}
	m_pCanvas->dirtyDisplayList();
	m_pCanvas->dirtyBound();
}
