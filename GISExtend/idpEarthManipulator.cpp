#include <osgEarthUtil/EarthManipulator>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osgText/Text>
#include "idpEarthManipulator.h"
#include "PlaneInfo.h"
#include "ClipBoard.h"
#include <Windows.h>


typedef struct PlaneABCD
{
	double a;
	double b;
	double c;
	double d;
} PLANE_ABCD, *P_PLANE_ABCD;

void CreatePlane(osg::Geometry* pG, const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, const osg::Vec3& v4, const osg::Vec4& color, P_PLANE_ABCD pAbcd = 0);
void GetPalneABCD(const osg::Vec3& p1, const osg::Vec3& p2, const osg::Vec3& p3, const osg::Vec3& p4, double* dA, double* dB, double* dC, double* dD);

idpEarthManipulator::idpEarthManipulator( osg::NodePath& nodePath, unsigned um /*= 0xFFFFFFFF*/ ) : m_NodePath(nodePath), m_uMark(um)
{

}


idpEarthManipulator::~idpEarthManipulator(void)
{
}

bool idpEarthManipulator::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	//us.asView();
	//osg::Drawable* pDraw = pickDrawable(, ea);
	osg::Vec3 moveX1(0, 0.1, 0);
	osg::Vec3 moveX2(0, -0.1, 0);
	osg::Vec3 moveY1(0.1, 0, 0);
	osg::Vec3 moveY2(-0.1, 0, 0);
	osg::Vec3 RealMove1, RealMove2;
	//static osg::MatrixTransform* pMT = 0;
	static osg::Drawable* pPickedDrawable = 0;
	ClipBoard* pClipBoard = 0;
	static osg::Vec3 prePos;
	static bool bPress = false;
	static bool bLockObject = false;
	double dCurXValue(0), dCurYValue(0);
	osg::Camera* pC = 0;
	osg::View* pOsgView = us.asView();
	osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(pOsgView);
	osg::Group* pSceneData = 0;
	osg::Vec3d v3WorldCoord, v3LUCoord;
	const osgEarth::SpatialReference* pSRS = getSRS();
	double dHeight = 0;

	screenToWorld(ea.getX(), ea.getY(), pViewer, v3WorldCoord);


	
	pSRS->transformFromWorld(v3WorldCoord, v3LUCoord, &dHeight);
	m_currentPosition = osg::Vec3(v3LUCoord.x(),v3LUCoord.y(),dHeight);

	if(pViewer)
	{
		pViewer->updateTraversal();
		pC = pViewer->getCamera();
		pSceneData = dynamic_cast<osg::Group*>(pViewer->getSceneData());
	}
	else
		return false;

	switch(ea.getEventType())
	{
	case osgGA::GUIEventAdapter::KEYDOWN:
		{
			if(ea.getModKeyMask() == osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL)
			{
				bPress = true;
			}
		}
		break;
	case osgGA::GUIEventAdapter::KEYUP:
		{
			bPress = false;
			/*if(ea.getKey() == 0xffffffee)
			{

			}*/
		}
		break;
	default:
		if(bPress)
		{
			osg::Matrix transMatrixUp, transMatrixDown;
				
			switch(ea.getEventType())
			{
			//case osgGA::GUIEventAdapter::MOVE:
			//	//getWorldCoordsUnderMouse(pViewer, ea.getX(), ea.getY(), v3WorldCoord);
			//	screenToWorld(ea.getX(), ea.getY(), pViewer, v3WorldCoord);
			//	pSRS->transformFromECEF(v3WorldCoord, v3LUCoord, &dHeight);
			//	break;
			case osgGA::GUIEventAdapter::PUSH:
				pPickedDrawable = pickDrawable(dynamic_cast<osgViewer::View*>(us.asView()), ea);
				if(!pPickedDrawable) return true;
				prePos.set(ea.getX(), ea.getY(), 0);
				break;
			case osgGA::GUIEventAdapter::RELEASE:
				pPickedDrawable = 0;
				break;
			case osgGA::GUIEventAdapter::DRAG:
				if(!pPickedDrawable) return true;
#if 0
				dCurXValue = ea.getX() - prePos.x();
				dCurYValue = ea.getY() - prePos.y();
				transMatrixUp.makeTranslate(0, 0, dCurXValue*500);
				transMatrixDown.makeTranslate(0, 0, dCurXValue*500);
				if(dCurXValue > 0)
					pMT->setMatrix(pMT->getMatrix() * transMatrixUp);
				else
					pMT->setMatrix(pMT->getMatrix() * transMatrixDown);
				/*std::stringstream strSteam;
				strSteam<<ea.getX()<<", "<<ea.getY();
				OutputDebugStringA(strSteam.str().data());*/
#else
#if 0
				dCurXValue = ea.getX() - prePos.x();
				dCurYValue = ea.getY() - prePos.y();

				if(pG->getName() == "Plane2")
				{
					RealMove1 = moveX1;
					RealMove2 = moveX2;
				}
				else if(pG->getName() == "Plane1")
				{
					RealMove1 = moveY1;
					RealMove2 = moveY2;
				}
				if(dCurXValue < 0)
					MoveGeometry(RealMove1, pG);
				else
					MoveGeometry(RealMove2, pG);

				prePos.set(ea.getX(), ea.getY(), 0);
#else
				double dScale = 0.07;
				dCurXValue = v3LUCoord.x();//ea.getX() - prePos.x();
				dCurYValue = v3LUCoord.y();//ea.getY() - prePos.y();

				osg::ShapeDrawable* pCtrlSphere = dynamic_cast<osg::ShapeDrawable*>(pPickedDrawable);
				if(pCtrlSphere)
				{
					std::string strName = pCtrlSphere->getName();

					if(strName == "ControlShape1")
					{
						pClipBoard = dynamic_cast<ClipBoard*>(pCtrlSphere->getUserData());
						if(pClipBoard)
							pClipBoard->MoveClipBoard(2, dCurXValue, dCurYValue);
					}
					else if(strName == "ControlShape2")
					{
						pClipBoard = dynamic_cast<ClipBoard*>(pCtrlSphere->getUserData());
						if(pClipBoard)
							pClipBoard->MoveClipBoard(0, dCurXValue, dCurYValue);
					}
					else if(strName == "ControlShape3")
					{
						pClipBoard = dynamic_cast<ClipBoard*>(pCtrlSphere->getUserData());
						if(pClipBoard)
							pClipBoard->MoveClipBoard(1, dCurXValue, dCurYValue);
					}
				}

				prePos.set(ea.getX(), ea.getY(), 0);
#endif
#endif
				break;
			}
			return true;
		}
		else
			return osgEarth::Util::EarthManipulator::handle(ea, us);
	}
	return false;
}

osg::MatrixTransform* idpEarthManipulator::pickTransformNode( osgViewer::View* view, const osgGA::GUIEventAdapter& ea )
{
	osg::MatrixTransform* pMT = 0;
	static osg::Vec4 color1(1, 0.5, 0.5, 0.5), color2(0.5, 0.5, 1, 0.5);

	osgUtil::LineSegmentIntersector::Intersections intersections;

	std::string gdlist="";
	float x = ea.getX();
	float y = ea.getY();

	if (view->computeIntersections(x,y,intersections))
	{
		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			std::string strClassName = hitr->drawable->className();
			if(strClassName == "Geometry")
			{
				osg::Geometry* pG = dynamic_cast<osg::Geometry*>(hitr->drawable.get());
				std::string strObjectName = pG->getName();
				
				if(strObjectName == "Plane1" || strObjectName == "Plane2")
				{
					pMT = dynamic_cast<osg::MatrixTransform*>(pG->getParent(0)->getParent(0));
					return pMT;
				}
#if 0
				if(pG != 0)
				{
					osg::Vec4Array* pV4Array = dynamic_cast<osg::Vec4Array*>(pG->getColorArray());

					osg::Vec4 color0 = pV4Array->at(0);
					
					/*osg::Vec4 color1 = pV4Array->at(1);
					osg::Vec4 color2 = pV4Array->at(2);
					osg::Vec4 color3 = pV4Array->at(3);*/

					pV4Array->clear();

					color0.set(1.0, 1.0, 1.0, 1.0);
					pV4Array->push_back(color0);
					pV4Array->push_back(color0);
					pV4Array->push_back(color0);
					pV4Array->push_back(color0);

					pG->setColorArray(pV4Array);
					pG->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
				}
#endif
			}
		}
	}
	return 0;
}

osg::Geometry* idpEarthManipulator::pickGeometry( osgViewer::View* view, const osgGA::GUIEventAdapter& ea )
{
		osg::MatrixTransform* pMT = 0;
	static osg::Vec4 color1(1, 0.5, 0.5, 0.5), color2(0.5, 0.5, 1, 0.5);

	osgUtil::LineSegmentIntersector::Intersections intersections;

	std::string gdlist="";
	float x = ea.getX();
	float y = ea.getY();

	if (view->computeIntersections(x,y,intersections))
	{
		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			std::string strClassName = hitr->drawable->className();
			if(strClassName == "Geometry")
			{
				osg::Geometry* pG = dynamic_cast<osg::Geometry*>(hitr->drawable.get());
				std::string strObjectName = pG->getName();
				
				if(strObjectName == "Plane1" || strObjectName == "Plane2")
				{
					return pG;
				}
			}
		}
	}
	return 0;
}
#if 0
void idpEarthManipulator::MoveGeometry(const osg::Vec3& Trans, osg::Geometry* pGeo )
{
	PlaneInfo* pPI = dynamic_cast<PlaneInfo*>(pGeo->getUserData());
	pPI->m_LeftPoint.set(pPI->m_LeftPoint.x() + Trans.x(), pPI->m_LeftPoint.y() + Trans.y(), pPI->m_LeftPoint.z() + Trans.z());
	pPI->m_RightPoint.set(pPI->m_RightPoint.x() + Trans.x(), pPI->m_RightPoint.y() + Trans.y(), pPI->m_RightPoint.z() + Trans.z());

	osg::Vec3d p11, p12, p13, p14;
	pPI->m_pSpatialRef->transformToECEF(osg::Vec3d(pPI->m_LeftPoint), p11);
	pPI->m_pSpatialRef->transformToECEF(osg::Vec3d(pPI->m_RightPoint), p12);
	pPI->m_pSpatialRef->transformToECEF(osg::Vec3d(pPI->m_RightPoint.x(), pPI->m_RightPoint.y(), pPI->m_RightPoint.z() + pPI->m_dHeight), p13);
	pPI->m_pSpatialRef->transformToECEF(osg::Vec3d(pPI->m_LeftPoint.x(), pPI->m_LeftPoint.y(), pPI->m_LeftPoint.z() + pPI->m_dHeight), p14);
	
	osg::Vec3Array* pVec3Array = dynamic_cast<osg::Vec3Array*>(pGeo->getVertexArray());
	pVec3Array->clear();
	pVec3Array->push_back(p11);
	pVec3Array->push_back(p12);
	pVec3Array->push_back(p13);
	pVec3Array->push_back(p14);

	pGeo->dirtyDisplayList();

	double dA, dB, dC, dD;
	//GetPalneABCD(p11, p12, p13, p14, &dA, &dB, &dC, &dD);
	pPI->m_pClipPline->setClipPlane(osg::Vec4d(dA, dB, dC, dD));
}
#endif
osg::Drawable* idpEarthManipulator::pickDrawable( osgViewer::View* view, const osgGA::GUIEventAdapter& ea )
{
	if(!view) return 0;
	osg::MatrixTransform* pMT = 0;
	static osg::Vec4 color1(1, 0.5, 0.5, 0.5), color2(0.5, 0.5, 1, 0.5);

	osgUtil::LineSegmentIntersector::Intersections intersections;

	std::string gdlist="";
	float x = ea.getX();
	float y = ea.getY();

	if (view->computeIntersections(x,y,intersections))
	{
		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			std::string strClassName = hitr->drawable->className();

			if(strClassName == "ShapeDrawable")
			{
				return hitr->drawable;
			}
		}
	}
	return 0;
}

bool idpEarthManipulator::getWorldCoordsUnderMouse(osg::View* view, float x, float y, osg::Vec3d& out_coords)
{
	osgViewer::View* view2 = dynamic_cast<osgViewer::View*>(view);
	/*if ( !view2 || !_graph.valid() )
	return false;*/

	osgUtil::LineSegmentIntersector::Intersections results;

	osg::NodePath path = m_NodePath;
	//path.push_back( _graph.get() );

	// fine but computeIntersections won't travers a masked Drawable, a la quadtree.
	unsigned mask = m_uMark/*_terrainOptions.secondaryTraversalMask().value()*/;

	if ( view2->computeIntersections( x, y, path, results, mask ) )
	{
		// find the first hit under the mouse:
		osgUtil::LineSegmentIntersector::Intersection first = *(results.begin());
		out_coords = first.getWorldIntersectPoint();
		return true;
	}
	return false;
}

void idpEarthManipulator::GetCurrentPosition( osg::Vec3& pos )
{
	pos = m_currentPosition;
}
