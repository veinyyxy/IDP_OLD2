#pragma once
#include <QObject>
#include <QtGui>
#include "idp_control_global.h"

#include "goConfig.h"
#include "Map.h"
#include "Layer.h"
#include "IDPdb.h"
#include "OsgEarthTransform.h"

class GISMapManager;
class AdapterWidget;
class QGLWidget;
class OsgEarthBridge;
class DefaultManipulator;
class QThread;
class AnimaticControlerInterface;
class OsgEarthBridge;
class DrawGeometryManipulator;
using namespace goto_gis;

class OsgThread : public QThread
{
public:
	void run()
	{
		if(m_pViewer && m_pGLWidget)
		{
			m_pGLWidget->makeCurrent();
			m_pViewer->realize();
			while(!m_pViewer->done())
				m_pViewer->frame();
		}
	}
	osgViewer::Viewer* m_pViewer;
	QGLWidget* m_pGLWidget;
};

class QGraphicsPixmapItemEX : public QGraphicsPixmapItem
{
public:
	QGraphicsPixmapItemEX( int x, int y);
	virtual ~QGraphicsPixmapItemEX();
protected:
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

class IDP_CONTROL_EXPORT ViewManager : public QObject
{
public:
	ViewManager(void);
	~ViewManager(void);

public:
	QWidget* CreateViewport(int wdt , bool basicMap = true ,QWidget* parentWidget = 0 );
	inline Map* GetMap2D() {return m_p2DBasicMap;}
	void Init2DBasicNode(QString filename);
	void Init3DBasicNode(QString filename);

	//inline OsgEarthBridge* GetOsgEarthBridge() { return m_pOEB;}
	void ShowRaderAttributeInfo(Layer* layer,QWidget* w);
private:
	void CreateRadarViewport(int wdt , QGLWidget* pGLWidget);
	boost::any GetAttributeValue(int index ,Layer* layer);
	
private:
	osg::Matrixd m_preViewMatrix, m_preViewMatrix3D;
	osg::Matrixd m_preProjectionMatrix, m_preProjectionMatrix3D;

	Map* m_p2DBasicMap;
	//GISMapManager* m_pGM;

	//osg::ref_ptr<osg::Node> m_p3DBasicNode;
	//OsgEarthBridge* m_pOEB;
	//OsgEarthTransform* m_pProjEarth;
	AnimaticControlerInterface* m_pRadarAnimatic;
	
};

