#pragma once
#include <osg/Referenced>
#include <osg/Vec4>
#include <osg/Node>
#include <osgGA/GUIEventAdapter>
#include <osgViewer/Viewer>
#include "GraphicsControler_global.h"

class GRAPHICSCONTROLER_EXPORT DrawGeometryData : public osg::Referenced
{
public:
	virtual void setThreadSafeRefUnref( bool threadSafe ){}
	virtual bool handle(const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea) = 0;
	
	inline osg::Camera* GetCamera(){return m_pCamera;}
	inline void SetCamera(osg::Camera* pCamera){m_pCamera = pCamera;}
	inline virtual void SetColor(osg::Vec4& v4c){m_color = v4c;}
	inline osg::Node* GetDrawNode(){return m_DrawNode;}
	inline osgViewer::Viewer* GetViewer(){return m_pViewer;}
	inline void SetViewer(osgViewer::Viewer* pViewer){m_pViewer = pViewer;}

protected:
	osg::Vec4 m_color;
	osg::Node* m_DrawNode;
	osg::Camera* m_pCamera;
	osgViewer::Viewer* m_pViewer;
};