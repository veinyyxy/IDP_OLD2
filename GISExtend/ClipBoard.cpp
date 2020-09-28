#include <osg/Geometry>
#include <osg/ClipPlane>
#include <osgText/Text>
#include <osg/Switch>
#include <osg/ShapeDrawable>
#include <osgViewer/View>
#include <osgSim/DOFTransform>
#include <osg/AutoTransform>
#include <osgEarth/SpatialReference>
#include<osgUtil/LineSegmentIntersector>
#include <vector>
#include "ClipBoard.h"
const int C_iHeight = 20000;

ClipBoard::ClipBoard(const osgEarth::SpatialReference* pSRS) : m_pSpatialRef(pSRS), m_pClipBoardMove(0)
{
	setName("ClipBoardGroup");
	m_ShapeDrawable1 = 0;
	m_ShapeDrawable2 = 0;
	m_ShapeDrawable3 = 0;
	m_pClipPlane = new osg::ClipPlane;
	m_pClipBoradSwitch = new osg::Switch;
	m_PlaneGeode = new osg::Geode;
	m_PlaneGeode->setName("PlaneGeode");
	m_BorderlineGeode = new osg::Geode;
	m_BorderlineGeode->setName("BorderlineGeode");
	m_PlaneGeometry = new osg::Geometry;
	m_BorderlineGeometry = new osg::Geometry;
	m_PlanePrimitiveset = new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4);
	m_BorderlinePrimitiveset = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4);

	/*
		4-------------------3
		|						  |
		|						  |
		1-------------------2
	*/
	m_Vec2PlaneTextrue = new osg::Vec2Array;
	m_Vec2PlaneTextrue->push_back(osg::Vec2(0, 1));//1
	m_Vec2PlaneTextrue->push_back(osg::Vec2(1, 1));//2
	m_Vec2PlaneTextrue->push_back(osg::Vec2(1, 0));//3
	m_Vec2PlaneTextrue->push_back(osg::Vec2(0, 0));//4

	m_Vec3PlaneVertexArray = new osg::Vec3Array;
	m_Vec3BorderlineVertexArray = new osg::Vec3Array;
	m_Vec4PlaneColorArray = new osg::Vec4Array;
	m_Vec4BorderlineColorArray = new osg::Vec4Array;

	m_ControlGeode0 = new osg::Geode;
	m_ControlGeode1 = new osg::Geode;
	m_ControlGeode2 = new osg::Geode;
	m_ControlGeode3 = new osg::Geode;

	//m_PlaneGeode->addDrawable(m_PlaneGeometry);
	m_BorderlineGeode->addDrawable(m_BorderlineGeometry);

	m_pClipBoradSwitch->addChild(m_PlaneGeode, true);
	m_pClipBoradSwitch->addChild(m_BorderlineGeode, true);
	m_pClipBoradSwitch->addChild(m_ControlGeode0, true);

	//pDofTransform = new osgSim::DOFTransform;
	
	/*pDofTransform->setMaxScale();
	pDofTransform->setMinScale();*/
	addChild(m_pClipBoradSwitch);
	//addChild(pDofTransform);

	m_AutoTransNode1 = new osg::AutoTransform;
	m_AutoTransNode2 = new osg::AutoTransform;
	m_AutoTransNode3 = new osg::AutoTransform;
	m_AutoTransNode1->setAutoScaleToScreen(true);
	m_AutoTransNode2->setAutoScaleToScreen(true);
	m_AutoTransNode3->setAutoScaleToScreen(true);
}


ClipBoard::~ClipBoard(void)
{
}

void ClipBoard::GetPalneABCD(const osg::Vec3& p1, const osg::Vec3& p2, const osg::Vec3& p3, const osg::Vec3& p4, double* dA, double* dB, double* dC, double* dD)
{
	osg::Vec3 vecPoint1;
	osg::Vec3 vecPoint2;
	vecPoint1 = p2 - p1;
	vecPoint2 = p3 - p1;
	float A,B,C,D;
	//osg::Vec3 parameterPoint;
	//parameterPoint.x = vecPoint1.y*vecPoint2.z - vecPoint1.z*vecPoint2.y;
	//parameterPoint.y = vecPoint1.x*vecPoint2.z -vecPoint1.z*vecPoint2.x;
	//parameterPoint.z = vecPoint1.x*vecPoint2.y - vecPoint1.y*vecPoint2.x;
	A = vecPoint1.y()*vecPoint2.z() - vecPoint1.z()*vecPoint2.y();
	B = vecPoint1.z()*vecPoint2.x() - vecPoint1.x()*vecPoint2.z();
	C = vecPoint1.x()*vecPoint2.y() - vecPoint1.y()*vecPoint2.x();
	//A(x-p1.x()) + B(y-p1.y()) + C(z-p1.z()) = 0;
	D = -(A*p1.x() + B * p1.y() + C * p1.z());
	*dA = A;
	*dB = B;
	*dC = C;
	*dD = D;
}

void ClipBoard::GetPlaneParm(double* dA, double* dB, double* dC, double* dD)
{
	if(m_Vec3PlaneVertexArray->size() != 4) return;
	osg::Vec3d p1, p2, p3, p4;
	m_pSpatialRef->transformToWorld(m_v3P1, p1);
	m_pSpatialRef->transformToWorld(m_v3P2, p2);
	m_pSpatialRef->transformToWorld(m_v3P3, p3);
	m_pSpatialRef->transformToWorld(m_v3P4, p4);
	GetPalneABCD(p1, p2, p3, p4, dA, dB, dC, dD);
}

void ClipBoard::SetFourPointAndInitClipBoard( const osg::Vec3& p1, const osg::Vec3& p2, const osg::Vec3& p3, const osg::Vec3& p4 )
{
#if 0
	static bool bFirst = true;
	if(bFirst)
	{
		m_v3P1 = p1;
		m_v3P2 = p2;
		m_v3P3 = p3;
		m_v3P4 = p4;
		bFirst = false;
	}
#else
	m_v3P1 = p1;
	m_v3P2 = p2;
	m_v3P3 = p3;
	m_v3P4 = p4;
#endif
	if(m_pSpatialRef == 0)
		return;
	float fCtrlSphereRadius = 10;
	m_Vec3PlaneVertexArray->clear();
	osg::Vec3d v3Wp1, v3Wp2, v3Wp3, v3Wp4, v3CtrlSphereCenter;
	m_pSpatialRef->transformToWorld(p1, v3Wp1);
	m_pSpatialRef->transformToWorld(p2, v3Wp2);
	m_pSpatialRef->transformToWorld(p3, v3Wp3);
	m_pSpatialRef->transformToWorld(p4, v3Wp4);

	m_Vec3PlaneVertexArray->push_back(v3Wp1);
	m_Vec3PlaneVertexArray->push_back(v3Wp2);
	m_Vec3PlaneVertexArray->push_back(v3Wp3);
	m_Vec3PlaneVertexArray->push_back(v3Wp4);

	/*m_Vec3BorderlineVertexArray->push_back(v3Wp1);
	m_Vec3BorderlineVertexArray->push_back(v3Wp2);
	m_Vec3BorderlineVertexArray->push_back(v3Wp3);
	m_Vec3BorderlineVertexArray->push_back(v3Wp4);*/

	
	m_PlaneGeometry->setVertexArray(m_Vec3PlaneVertexArray);
	m_PlaneGeometry->addPrimitiveSet(m_PlanePrimitiveset);
	m_PlaneGeometry->setTexCoordArray(0, m_Vec2PlaneTextrue);

	int iPointCount = ComputeBorderline();
	m_BorderlinePrimitiveset->set(osg::PrimitiveSet::LINE_LOOP, 0, iPointCount);
	m_BorderlineGeometry->setVertexArray(m_Vec3BorderlineVertexArray);
	m_BorderlineGeometry->addPrimitiveSet(m_BorderlinePrimitiveset);

	SetPlaneColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SetBorderlineColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	////////////////////////////////// 设置裁剪板节点状态////////////////////////////////////////
	osg::StateSet* pSS = m_PlaneGeometry->getOrCreateStateSet();
	pSS->setMode(GL_BLEND,osg::StateAttribute::ON); //Alpha混合开启 
	//pSS->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF  ); //取消深度测试 
	m_Texture2D = new osg::Texture2D;
	pSS->setTextureAttributeAndModes(0, m_Texture2D, osg::StateAttribute::ON);
	////////////////////////////控制球/////////////////////////////////////
	m_Contrl_Sphere1 = new osg::Sphere(osg::Vec3(0, 0, 0), fCtrlSphereRadius);
	m_Contrl_Sphere2 = new osg::Sphere(osg::Vec3(0, 0, 0), fCtrlSphereRadius);
	m_AutoTransNode1->setPosition(v3Wp1);
	m_AutoTransNode2->setPosition(v3Wp2);

	osg::Vec3 controlSphere3Center;
	controlSphere3Center.set(p1.x() - (p1.x() - p2.x()) / 2, p1.y() - (p1.y() - p2.y()) / 2, p1.z());
	m_pSpatialRef->transformToWorld(controlSphere3Center, v3CtrlSphereCenter);
	m_Contrl_Sphere3 = new osg::Sphere(osg::Vec3(0, 0, 0), fCtrlSphereRadius);
	m_AutoTransNode3->setPosition(v3CtrlSphereCenter);
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	hints->setDetailRatio(0.5f);

	//if(m_ShapeDrawable1) bool bR = m_ControlGeode1->removeDrawable(m_ShapeDrawable1);
	
	m_ShapeDrawable1 = new osg::ShapeDrawable(m_Contrl_Sphere1, hints);
	m_ShapeDrawable1->setName("ControlShape1");
	m_ShapeDrawable1->setColor(osg::Vec4(1, 1, 0, 1));
	m_ShapeDrawable1->setUserData(this);
	//m_ControlGeode1->setDrawable(0, m_ShapeDrawable1);
	m_ControlGeode1->addDrawable(m_ShapeDrawable1);

	//if(m_ShapeDrawable2) m_ControlGeode1->removeDrawable(m_ShapeDrawable2);
	m_ShapeDrawable2 = new osg::ShapeDrawable(m_Contrl_Sphere2, hints);
	m_ShapeDrawable2->setName("ControlShape2");
	m_ShapeDrawable2->setColor(osg::Vec4(1, 1, 0, 1));
	m_ShapeDrawable2->setUserData(this);
	//m_ControlGeode1->setDrawable(1, m_ShapeDrawable2);
	m_ControlGeode2->addDrawable(m_ShapeDrawable2);
	
	//if(m_ShapeDrawable3) m_ControlGeode1->removeDrawable(m_ShapeDrawable3);
	m_ShapeDrawable3 = new osg::ShapeDrawable(m_Contrl_Sphere3, hints);
	m_ShapeDrawable3->setName("ControlShape3");
	m_ShapeDrawable3->setColor(osg::Vec4(1, 0, 0, 1));
	m_ShapeDrawable3->setUserData(this);
	//m_ControlGeode1->setDrawable(2, m_ShapeDrawable3);
	m_ControlGeode3->addDrawable(m_ShapeDrawable3);

	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////

	////////////////////////////////////创建线条////////////////////////////
	CreateLine();
	//////////////////////////////////////////////////////////////////////////

	m_AutoTransNode1->addChild(m_ControlGeode1);
	m_AutoTransNode2->addChild(m_ControlGeode2);
	m_AutoTransNode3->addChild(m_ControlGeode3);

	addChild(m_AutoTransNode1);
	addChild(m_AutoTransNode2);
	addChild(m_AutoTransNode3);
}

void ClipBoard::SetPlaneColor( const osg::Vec4 v4Color )
{
	m_Vec4PlaneColorArray->clear();

	size_t iVecCount = m_Vec3PlaneVertexArray->size();
	m_Vec4PlaneColorArray->push_back(v4Color);
	m_PlaneGeometry->setColorArray(m_Vec4PlaneColorArray);
	m_PlaneGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
}

void ClipBoard::SetBorderlineColor( const osg::Vec4 v4Color )
{
	m_Vec4BorderlineColorArray->clear();

	size_t iVecCount = m_Vec3BorderlineVertexArray->size();
	for(size_t i = 0; i < iVecCount; i++)
	{
		m_Vec4BorderlineColorArray->push_back(v4Color);
	}

	m_BorderlineGeometry->setColorArray(m_Vec4BorderlineColorArray);
	m_BorderlineGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
}

osg::ClipPlane* ClipBoard::GetClipPlane()
{
	double dA, dB, dC,  dD;
	GetPlaneParm(&dA, &dB, &dC, &dD);

	m_pClipPlane->setClipPlane(dA, dB, dC, dD);
	return m_pClipPlane;
}

void ClipBoard::SetPlaneVisible( bool bV )
{
	m_pClipBoradSwitch->setChildValue(m_PlaneGeode, bV);
}

void ClipBoard::SetBoardlineVisible( bool bV )
{
	m_pClipBoradSwitch->setChildValue(m_BorderlineGeode, bV);
}

void ClipBoard::MoveClipBoard(int iPosID, double dXDist, double dYDist )
{
#if 0
	switch(iPosID)
	{
		case 0:
			m_v3P1.set(m_v3P1.x()+dXDist, m_v3P1.y() + dYDist, m_v3P1.z());
			m_v3P2.set(m_v3P2.x(), m_v3P2.y(), m_v3P2.z());
			m_v3P3.set(m_v3P3.x(), m_v3P3.y(), m_v3P3.z());
			m_v3P4.set(m_v3P4.x()+dXDist, m_v3P4.y() + dYDist, m_v3P4.z());
			break;
		case 1:
			m_v3P1.set(m_v3P1.x()+dXDist, m_v3P1.y() + dYDist, m_v3P1.z());
			m_v3P2.set(m_v3P2.x()+dXDist, m_v3P2.y() + dYDist, m_v3P2.z());
			m_v3P3.set(m_v3P3.x()+dXDist, m_v3P3.y() + dYDist, m_v3P3.z());
			m_v3P4.set(m_v3P4.x()+dXDist, m_v3P4.y() + dYDist, m_v3P4.z());
			break;
		case 2:
			m_v3P1.set(m_v3P1.x(), m_v3P1.y(), m_v3P1.z());
			m_v3P2.set(m_v3P2.x()+dXDist, m_v3P2.y() + dYDist, m_v3P2.z());
			m_v3P3.set(m_v3P3.x()+dXDist, m_v3P3.y() + dYDist, m_v3P3.z());
			m_v3P4.set(m_v3P4.x(), m_v3P4.y(), m_v3P4.z());
			break;
		default:
			return;
	}
#else
	double dXdis1 = 0, dYdis1 = 0;
	double dXdisM = 0, dYdisM = 0;
	double dXdis2 = 0, dYdis2 = 0;
	double dVM = 0;
	switch(iPosID)
	{
	case 0:
		m_v3P2.set(dXDist, dYDist, m_v3P2.z());
		m_v3P3.set(dXDist, dYDist, m_v3P3.z());
		break;
	case 1:
		dXdisM = m_v3P1.x() - m_v3P2.x();
		dYdisM = m_v3P1.y() - m_v3P2.y();
		if(fabs(dXdisM) > 0.0000001 && fabs(dYdisM) > 0.0000001 )
		{
			dVM = abs((m_v3P1.x() - m_v3P2.x()) / 2);
			float fK = 0;
			
			fK = ComputeKOfLine(m_v3P1, m_v3P2);
			dXdis1 = dXDist - dVM;
			dXdis2 = dXDist + dVM;
			osg::Vec3 p(dXDist, dYDist, 0);
			ComputeKPointLine(p, fK, dXdis1, &dYdis1);
			ComputeKPointLine(p, fK, dXdis2, &dYdis2);
			m_v3P1.set(dXdis1, dYdis1, m_v3P1.z());
			m_v3P2.set(dXdis2, dYdis2, m_v3P2.z());
			m_v3P3.set(dXdis2, dYdis2, m_v3P3.z());
			m_v3P4.set(dXdis1, dYdis1, m_v3P4.z());
		}
		else
		{
			if(fabs(dXdisM) - 0 < 0.0000001)
			{
				dVM = abs((m_v3P1.y() - m_v3P2.y()) / 2);
				dYdis1 = dYDist - dVM;
				dYdis2 = dYDist + dVM;

				m_v3P1.set(dXDist, dYdis1, m_v3P1.z());
				m_v3P2.set(dXDist, dYdis2, m_v3P2.z());
				m_v3P3.set(dXDist, dYdis2, m_v3P3.z());
				m_v3P4.set(dXDist, dYdis1, m_v3P4.z());
			}
			else
			{
				dVM = abs((m_v3P1.x() - m_v3P2.x()) / 2);
				dXdis1 = dXDist - dVM;
				dXdis2 = dXDist + dVM;

				m_v3P1.set(dXdis1, dYDist, m_v3P1.z());
				m_v3P2.set(dXdis2, dYDist, m_v3P2.z());
				m_v3P3.set(dXdis2, dYDist, m_v3P3.z());
				m_v3P4.set(dXdis1, dYDist, m_v3P4.z());
			}
		}
		break;
	case 2:
		m_v3P1.set(dXDist, dYDist, m_v3P1.z());
		m_v3P4.set(dXDist, dYDist, m_v3P4.z());
		break;
	default:
		return;
	}
#endif
	if(m_pSpatialRef == 0)
		return;
	float fCtrlSphereRadius = 10;
	m_Vec3PlaneVertexArray->clear();
	m_Vec3BorderlineVertexArray->clear();
	osg::Vec3d v3Wp1, v3Wp2, v3Wp3, v3Wp4, v3CtrlSphereCenter;
	m_pSpatialRef->transformToWorld(m_v3P1, v3Wp1);
	m_pSpatialRef->transformToWorld(m_v3P2, v3Wp2);
	m_pSpatialRef->transformToWorld(m_v3P3, v3Wp3);
	m_pSpatialRef->transformToWorld(m_v3P4, v3Wp4);

	m_Vec3PlaneVertexArray->push_back(v3Wp1);
	m_Vec3PlaneVertexArray->push_back(v3Wp2);
	m_Vec3PlaneVertexArray->push_back(v3Wp3);
	m_Vec3PlaneVertexArray->push_back(v3Wp4);

	m_PlaneGeometry->setVertexArray(m_Vec3PlaneVertexArray);
	m_PlaneGeometry->addPrimitiveSet(m_PlanePrimitiveset);


	int iPointCount = ComputeBorderline();
	m_BorderlinePrimitiveset->set(osg::PrimitiveSet::LINE_LOOP, 0, iPointCount);
	m_BorderlineGeometry->setVertexArray(m_Vec3BorderlineVertexArray);
	m_BorderlineGeometry->addPrimitiveSet(m_BorderlinePrimitiveset);

	////////////////////////////控制球/////////////////////////////////////
	m_Contrl_Sphere1->set(osg::Vec3(0, 0, 0), fCtrlSphereRadius);
	m_Contrl_Sphere2->set(osg::Vec3(0, 0, 0), fCtrlSphereRadius);

	m_AutoTransNode1->setPosition(v3Wp1);
	m_AutoTransNode2->setPosition(v3Wp2);
	
	osg::Vec3 controlSphere3Center;
	double dx(0), dy(0);
	
	if(fabs(m_v3P1.x() - m_v3P2.x()) > 0.0000001 && fabs(m_v3P1.y() - m_v3P2.y()) > 0.0000001 )
	{
		dx = m_v3P1.x() - (m_v3P1.x() - m_v3P2.x()) / 2;
		ComputePointOnLine(m_v3P1, m_v3P2, dx, &dy);
		controlSphere3Center.set(dx, dy, m_v3P1.z());
	}
	else
		controlSphere3Center.set(m_v3P1.x() - (m_v3P1.x() - m_v3P2.x()) / 2, m_v3P1.y() - (m_v3P1.y() - m_v3P2.y()) / 2, m_v3P1.z());
	m_pSpatialRef->transformToWorld(controlSphere3Center, v3CtrlSphereCenter);
	m_Contrl_Sphere3->set(osg::Vec3(0, 0, 0), fCtrlSphereRadius);
	m_AutoTransNode3->setPosition(v3CtrlSphereCenter);
	//SetFourPointAndInitClipBoard(m_v3P1, m_v3P2, m_v3P3, m_v3P4);
	
	m_ShapeDrawable1->dirtyDisplayList();
	m_ShapeDrawable1->dirtyBound();
	m_ShapeDrawable2->dirtyDisplayList();
	m_ShapeDrawable2->dirtyBound();
	m_ShapeDrawable3->dirtyDisplayList();
	m_ShapeDrawable3->dirtyBound();

	m_PlaneGeometry->dirtyDisplayList();
	m_PlaneGeometry->dirtyBound();
	m_BorderlineGeometry->dirtyDisplayList();
	m_BorderlineGeometry->dirtyBound();
	ReComputeLine();
	GetClipPlane();

	if(m_pClipBoardMove)
	{
		osg::Vec3 v3LT1, v3LT2, v3W1, v3W2;

		v3LT1 = m_v3P1;
		v3LT2 = m_v3P2;

		v3W1 = v3Wp1;
		v3W2 = v3Wp2;

		osg::Image* pImage = 0;
		m_pClipBoardMove->MoveCompleted(v3LT1, v3LT2, v3W1, v3W2, m_PlaneGeode, &pImage);
		//SetPlaneImage(pImage);
	}
}

osg::Geometry* ClipBoard::MakeLine( const osg::Vec3& p1, const osg::Vec3& p2 )
{
	osg::Vec4 v4Color;
	osg::Geometry* pLine = new osg::Geometry;
	osg::Vec3Array* pLinePoint = new osg::Vec3Array;
	osg::Vec4Array* pLineColor = new osg::Vec4Array;
	pLinePoint->push_back(p1);
	pLinePoint->push_back(p2);
	if(m_Vec4BorderlineColorArray->size() > 0)
		v4Color = m_Vec4BorderlineColorArray->at(0);
	else
		v4Color.set(1, 1, 1, 1);

	pLineColor->push_back(v4Color);
	pLineColor->push_back(v4Color);

	pLine->setVertexArray(pLinePoint);
	pLine->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));
	
	pLine->setColorArray(pLineColor);
	pLine->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	return pLine;
}

void ClipBoard::ComputePlane()
{
	
}

int ClipBoard::ComputeBorderline()
{
	int iPointCount = 0;
	int iSegmentCount = 30;
	m_Vec3BorderlineVertexArray->clear();
	osg::Vec3d v3Wp1, v3Wp2, v3Wp3, v3Wp4, v3TempP, v3CtrlSphereCenter;
	m_pSpatialRef->transformToWorld(m_v3P1, v3Wp1);
	m_pSpatialRef->transformToWorld(m_v3P2, v3Wp2);
	m_pSpatialRef->transformToWorld(m_v3P3, v3Wp3);
	m_pSpatialRef->transformToWorld(m_v3P4, v3Wp4);

	m_Vec3BorderlineVertexArray->push_back(v3Wp1);
	double dXCount = fabs(m_v3P1.x() - m_v3P2.x()) / iSegmentCount;
	double dYCount = fabs(m_v3P1.y() - m_v3P2.y()) / iSegmentCount;
	for(int i = 0; i < iSegmentCount; i++)
	{
		double xp = 0;
		double yp = 0;
		if(fabs(dXCount - 0) > 0.0000001 && fabs(dYCount - 0) > 0.0000001)
		{
			xp = m_v3P1.x() - i * ((m_v3P1.x() - m_v3P2.x()) / iSegmentCount);
			yp = 0;
			ComputePointOnLine(m_v3P1, m_v3P2, xp, &yp);
			v3TempP.set(xp, yp, m_v3P1.z());
		}
		else
		{
			v3TempP.set(m_v3P1.x() + i * dXCount, m_v3P1.y() + i * dYCount, m_v3P1.z());
		}
		m_pSpatialRef->transformToWorld(v3TempP, v3TempP);
		m_Vec3BorderlineVertexArray->push_back(v3TempP);
		iPointCount++;
	}
	
	m_Vec3BorderlineVertexArray->push_back(v3Wp2);
	m_Vec3BorderlineVertexArray->push_back(v3Wp3);
	m_Vec3BorderlineVertexArray->push_back(v3Wp4);
	iPointCount += 4;
	return iPointCount;
}

osgText::Text* ClipBoard::MakeHgtTxt(const osg::Vec3& p1, const std::string& strHgtTxt)
{
	osgText::Text* pHgtTxt = new osgText::Text();
	pHgtTxt->setFont("Fonts/simhei.ttf");
	pHgtTxt->setText(strHgtTxt); 
	pHgtTxt->setPosition(p1);
	pHgtTxt->setAutoRotateToScreen(true);
	pHgtTxt->setCharacterSize(15);
	pHgtTxt->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
	osg::Vec4 m_markColor(.0, .0,.0,1);
	pHgtTxt->setColor(m_markColor);
	pHgtTxt->setAlignment(osgText::Text::CENTER_TOP);
	return pHgtTxt;
}

void ClipBoard::ReComputeLine()
{
	size_t uiSize = m_LineGeo.size();
	int dHeight = C_iHeight / uiSize;

	for(unsigned i = 0; i < uiSize; i++)
	{
		osg::Vec3d LineP1, LineP2;
		LineP1.set(m_v3P1.x(), m_v3P1.y(), m_v3P1.z() + (i + 1) * dHeight);
		LineP2.set(m_v3P2.x(), m_v3P2.y(), m_v3P2.z() + (i + 1) * dHeight);

		m_pSpatialRef->transformToWorld(LineP1, LineP1);
		m_pSpatialRef->transformToWorld(LineP2, LineP2);

		osg::Vec3Array* pVec3Array = dynamic_cast<osg::Vec3Array*>(m_LineGeo.at(i)->getVertexArray());
		if(pVec3Array)
		{
			pVec3Array->clear();
			pVec3Array->push_back(LineP1);
			pVec3Array->push_back(LineP2);
		}
		m_LineGeo.at(i)->dirtyBound();
		m_LineGeo.at(i)->dirtyDisplayList();

		m_TextVector.at(i)->setPosition(LineP2);
		m_TextVector.at(i)->dirtyBound();
		m_TextVector.at(i)->dirtyDisplayList();

	}
}

void ClipBoard::CreateLine()
{
    int dHeight = C_iHeight/4;
	osg::Vec3d LineP1, LineP2;
	char chHgt[32];
	for(int i = 1; i < 5; i++)
	{
		memset(chHgt, 0x0, sizeof(char)*32);
		LineP1.set(m_v3P1.x(), m_v3P1.y(), m_v3P1.z() + i * dHeight);
		LineP2.set(m_v3P2.x(), m_v3P2.y(), m_v3P2.z() + i * dHeight);

		m_pSpatialRef->transformToWorld(LineP1, LineP1);
		m_pSpatialRef->transformToWorld(LineP2, LineP2);

		osg::Geometry* pLine = MakeLine(LineP1, LineP2);
		itoa(5*i, chHgt, 10);
		osgText::Text* pHgtTxt = MakeHgtTxt(LineP2, std::string(chHgt)+"KM");
		m_ControlGeode0->addDrawable(pLine);
		m_ControlGeode0->addDrawable(pHgtTxt);
		m_LineGeo.push_back(pLine);
		m_TextVector.push_back(pHgtTxt);
	}
}

void ClipBoard::ComputePointOnLine( osg::Vec3& p1, osg::Vec3 p2, double dX, double* outDY )
{
	double s1 = (dX-p1.x())/(p2.x()-p1.x());
	double s2 = p1.y()/(p2.y() - p1.y());
	double s3 = p2.y() - p1.y();
	*outDY = (s1 + s2 ) *s3;
}

float ClipBoard::ComputeKOfLine( osg::Vec3& p1, osg::Vec3 p2)
{
	float fK = 0;
	fK = (p1.y() - p2.y())/(p1.x() - p2.x());

	return fK;
}

void ClipBoard::ComputeKPointLine( osg::Vec3& p, float fK, double dX, double* outY )
{
	*outY = fK * (dX - p.x()) + p.y();
}

void ClipBoard::SetPlaneImage( osg::Image* pImage )
{
	if(pImage)
	{
		m_Texture2D->setImage(pImage);
		//stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
		//m_PlaneGeometry->setStateSet(stateset);
	}

	m_PlaneGeometry->dirtyDisplayList();
}
