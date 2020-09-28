#pragma once
#include "GraphicsControler_global.h"
class ObtainLongLatInterface;

class ObtainLongLatEventHandler :
	public osgGA::GUIEventHandler
{
public:
	ObtainLongLatEventHandler(void);
	~ObtainLongLatEventHandler(void);

	virtual bool handle( const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter& );

	virtual osg::Object* clone( const osg::CopyOp& ) const;
	bool ObtainLongLatEventHandler::pickGeometry( osgViewer::View* view, const osgGA::GUIEventAdapter& ea, osg::Vec3& v3LongLat);
	
	inline void SetObtainImplementation(ObtainLongLatInterface* pOLI){m_ObtainLongLat = pOLI;}

private:
	ObtainLongLatInterface* m_ObtainLongLat;
};
