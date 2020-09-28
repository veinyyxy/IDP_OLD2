#include <osgViewer/Viewer>
#include <osg/AutoTransform>
#include "DeleteSymbol.h"


DeleteSymbol::DeleteSymbol(void)
{
}


DeleteSymbol::~DeleteSymbol(void)
{
}

bool DeleteSymbol::handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea )
{
	bool m_bPickObject = false;
	osgUtil::LineSegmentIntersector::Intersections intersections;

	if(m_pViewer)
	{
		osgViewer::Viewer* pViewer = GetViewer();
		
		m_bPickObject = pViewer->computeIntersections(ea.getX(), ea.getY(), intersections);
	}
	switch( ea.getEventType() )
	{
	case osgGA::GUIEventAdapter::PUSH:
		if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			
			if(m_bPickObject)
			{
				for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
					hitr != intersections.end();
					++hitr)
				{
					std::string strName = hitr->drawable->getName();

					if(strName == m_strName)
					{
						osg::Node* pNode = hitr->drawable->getParent(0);

						if(pNode->getName() == "Line_Line")
						{
							osg::Node* pParentNode = pNode->getParent(0);
							if(pParentNode)
							{
								osg::Group* pGroup = pParentNode->asGroup();
								if(pGroup)
									pGroup->removeChild(pNode);
							}
						}
						else if(pNode->getName() == "Wind_Line")
						{
							osg::Node* pParentNode1 = pNode->getParent(0);
							if(pParentNode1)
							{
								osg::Node* pParentNode2 = pParentNode1->getParent(0);
								if(pParentNode2)
								{
									osg::Group* pGroup = pParentNode2->asGroup();
									if(pGroup)
									{
										pGroup->removeChild(pParentNode2);
										osg::AutoTransform* pNode = (osg::AutoTransform*)(hitr->drawable->getUserData());
										pGroup->removeChild(pNode);
									}
								}
							}
						}
						else
						{
							osg::Node* pParentNode = pNode->getParent(0);

							osg::Group* pGroup = dynamic_cast<osg::Group*>(m_pLayer->GetLayerNode());
							if(pGroup)
							{
								pGroup->removeChild(pParentNode);
							}
						}
					}
				}
			}
		}
		break;
	default:
		return false;
	}
	return true;
}
