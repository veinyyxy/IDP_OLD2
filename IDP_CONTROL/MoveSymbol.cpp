#include "MoveSymbol.h"

MoveSymbol::MoveSymbol(void) : m_pLockNode(0), m_bMoveLockNode(false)
{
}


MoveSymbol::~MoveSymbol(void)
{
}

bool MoveSymbol::handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea )
{
	if(!m_bMoveLockNode)
	{
		osgViewer::Viewer* pViewer = GetViewer();
		osgUtil::LineSegmentIntersector::Intersections intersections;
		pViewer->computeIntersections(ea.getX(), ea.getY(), intersections);

		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			std::string strName = hitr->drawable->getName();

			if(strName == m_strName)
			{
				osg::Node* pNode = hitr->drawable->getParent(0);

				osg::Node* pParentNode = pNode->getParent(0);

				m_pLockNode = dynamic_cast<osg::AutoTransform*>(pParentNode);
			}
		}
	}

	switch( ea.getEventType() )
	{
	case osgGA::GUIEventAdapter::DRAG:
		if(m_pLockNode && m_bMoveLockNode)
			m_pLockNode->setPosition(pos);
		break;
	case osgGA::GUIEventAdapter::PUSH:
		{
			if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				m_bMoveLockNode = true;
			}
		}
		break;

	case osgGA::GUIEventAdapter::RELEASE:
		if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			m_bMoveLockNode = false;
			m_pLockNode = 0;
		}
		break;
	default:
		
		return false;
	}
	return true;
}
