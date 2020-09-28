#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osg/Node>

#include "ObtainLongLatInterface.h"
#include "ObtainLongLatEventHandler.h"


ObtainLongLatEventHandler::ObtainLongLatEventHandler(void) : m_ObtainLongLat(0)
{
}


ObtainLongLatEventHandler::~ObtainLongLatEventHandler(void)
{
}

bool ObtainLongLatEventHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osg::Vec3 v3LongLat;
	if(m_ObtainLongLat)
	{
		osgViewer::View* pViewerView = dynamic_cast<osgViewer::View*>(aa.asView());
		if(pickGeometry(pViewerView, ea, v3LongLat))
		{
			m_ObtainLongLat->OutLongLatHeight(v3LongLat.x(), v3LongLat.y(), v3LongLat.z());
			return true;
		}
		else return false;
	}
	else
		return false;
}

osg::Object* ObtainLongLatEventHandler::clone( const osg::CopyOp& co) const
{
	return 0;
}

bool ObtainLongLatEventHandler::pickGeometry( osgViewer::View* view, const osgGA::GUIEventAdapter& ea, osg::Vec3& v3LongLat)
{
	osg::MatrixTransform* pMT = 0;
	static osg::Vec4 color1(1, 0.5, 0.5, 0.5), color2(0.5, 0.5, 1, 0.5);

	osgUtil::LineSegmentIntersector::Intersections intersections;

	std::string gdlist="";
	float x = ea.getX();
	float y = ea.getY();
	osg::Vec3d v3IntersectPoint;
	osg::NodePath localNodePath;
	//goto_gis::CoordinateTransform* pCT;
	ObtainLongLatInterface::OBTAIN_OBJECT_TYPE iPickUpObjectType;
	std::string strObjectName;
	m_ObtainLongLat->GetObjectName(iPickUpObjectType, strObjectName);
	std::string strObtainName;
	//m_ObtainLongLat->GetCoordinateReference(&pCT);

	//if(!pCT) return false;

	if (view->computeIntersections(x,y,intersections))
	{
		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			switch(iPickUpObjectType)
			{
			case ObtainLongLatInterface::FROM_NODE:
				localNodePath = hitr->nodePath;
				for(int iNodeNum = 0; iNodeNum < localNodePath.size(); iNodeNum++)
				{
					strObtainName = localNodePath.at(iNodeNum)->getName();
					if(strObtainName == strObjectName)
					{
						v3IntersectPoint = hitr->getWorldIntersectPoint();
						//v3LongLat = pCT->Transform(v3IntersectPoint);
					}
				}
				
				break;
			case ObtainLongLatInterface::FROM_DRAWABLE:
				strObtainName = hitr->drawable->getName();
				if(strObtainName == strObjectName)
				{
					v3IntersectPoint = hitr->getWorldIntersectPoint();
					//v3LongLat = pCT->Transform(v3IntersectPoint);
				}
				break;
			default:
				return false;
			}
		}
	}
	return true;
}